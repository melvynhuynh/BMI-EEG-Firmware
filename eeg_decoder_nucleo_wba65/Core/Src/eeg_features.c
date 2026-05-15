/*
 * eeg_features.c
 *
 *  Created on: May 13, 2026
 *      Author: melvy
 */


#include "eeg_features.h"
#include "arm_math.h"
#include <math.h>
#include <string.h>

#define EEG_PI 3.14159265358979f

/* === Limites des 5 bandes de fréquence (Hz) === */
static const float band_lo[EEG_N_BANDS] = {0.5f, 4.f,  8.f, 12.f, 30.f};
static const float band_hi[EEG_N_BANDS] = {4.f,  8.f, 12.f, 30.f, 45.f};

/* === Instance FFT + fenêtre de Hann (calculée une fois) === */
static arm_rfft_fast_instance_f32 fft;
static float hann[EEG_NPERSEG];
static float hann_sum_sq;     // sum(hann[n]^2) -- pour la normalisation density

void eeg_features_init(void) {
    arm_rfft_fast_init_f32(&fft, EEG_FFT_LEN);
    hann_sum_sq = 0.f;
    for (int n = 0; n < EEG_NPERSEG; n++) {
        hann[n] = 0.5f * (1.f - cosf(2.f * EEG_PI * (float)n / (float)(EEG_NPERSEG - 1)));
        hann_sum_sq += hann[n] * hann[n];
    }
}

/* Welch PSD pour 1 canal -> band power des 5 bandes */
static void welch_one_channel(const float *x, float *band_power_out) {
    float seg[EEG_FFT_LEN];
    float spec[EEG_FFT_LEN];
    float psd_avg[EEG_FFT_LEN/2 + 1];
    int   n_segments = 0;

    memset(psd_avg, 0, sizeof(psd_avg));

    /* Boucle sur les segments avec 50% overlap */
    for (int start = 0;
         start + EEG_NPERSEG <= EEG_EPOCH_LEN;
         start += (EEG_NPERSEG - EEG_NOVERLAP)) {

        /* Fenêtrage Hann + zero-padding à EEG_FFT_LEN */
        for (int n = 0; n < EEG_NPERSEG; n++) {
            seg[n] = x[start + n] * hann[n];
        }
        for (int n = EEG_NPERSEG; n < EEG_FFT_LEN; n++) {
            seg[n] = 0.f;
        }

        /* FFT réelle (out-of-place: seg en input, spec en output) */
        arm_rfft_fast_f32(&fft, seg, spec, 0);

        /* |X|² → PSD (spec packe DC..Nyquist en paires Re,Im sauf DC et Nyquist) */
        psd_avg[0]             += spec[0] * spec[0];                       // DC
        psd_avg[EEG_FFT_LEN/2] += spec[1] * spec[1];                       // Nyquist
        for (int k = 1; k < EEG_FFT_LEN/2; k++) {
            float re = spec[2*k];
            float im = spec[2*k + 1];
            psd_avg[k] += re*re + im*im;
        }
        n_segments++;
    }

    /* Moyenne sur les segments */
    float norm = 1.f / (float)n_segments;
    for (int k = 0; k <= EEG_FFT_LEN/2; k++) {
        psd_avg[k] *= norm;
    }

    /* === Normalisation scipy.signal.welch (scaling='density') ===
     *  psd /= (fs * sum(window^2))   pour DC et Nyquist
     *  psd *= 2 / (fs * sum(window^2))  pour les autres bins (spectre unilateral)
     *  -> sans ca, |FFT|^2 brute est ~20000x trop grosse, sigmoid sature. */
    const float scale_dc_nyq = 1.f / ((float)EEG_FS * hann_sum_sq);
    const float scale_other  = 2.f / ((float)EEG_FS * hann_sum_sq);
    psd_avg[0]             *= scale_dc_nyq;
    psd_avg[EEG_FFT_LEN/2] *= scale_dc_nyq;
    for (int k = 1; k < EEG_FFT_LEN/2; k++) {
        psd_avg[k] *= scale_other;
    }

    /* Somme des bins dans chaque bande (matche le np.sum de Charles) */
    float df = (float)EEG_FS / (float)EEG_FFT_LEN;
    for (int b = 0; b < EEG_N_BANDS; b++) {
        int k_lo = (int)ceilf (band_lo[b] / df);
        int k_hi = (int)floorf(band_hi[b] / df);
        float s = 0.f;
        for (int k = k_lo; k <= k_hi; k++) {
            s += psd_avg[k];
        }
        band_power_out[b] = s;
    }
}

void eeg_extract_band_power(const float epoch[EEG_N_CHANNELS][EEG_EPOCH_LEN],
                             float features[EEG_N_FEATURES]) {
    float bp_per_ch[EEG_N_BANDS];
    for (int c = 0; c < EEG_N_CHANNELS; c++) {
        welch_one_channel(epoch[c], bp_per_ch);
        /* Ordre band-major / channel-minor — DOIT matcher Charles */
        for (int b = 0; b < EEG_N_BANDS; b++) {
            features[b * EEG_N_CHANNELS + c] = bp_per_ch[b];
        }
    }
}

