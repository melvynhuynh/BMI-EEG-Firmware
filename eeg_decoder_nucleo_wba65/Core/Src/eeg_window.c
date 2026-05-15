/*
 * eeg_window.c
 *
 *  Created on: May 14, 2026
 *      Author: melvy
 *
 * ============================================================================
 *  Sliding window EEG -- strategie "shift-and-fill"
 * ============================================================================
 *
 *  ROLE
 *  ----
 *  Garder en permanence les EEG_EPOCH_LEN (= 666) derniers samples filtres
 *  par canal, ranges chronologiquement (plus vieux a gauche, plus recent a
 *  droite), pour fournir une fenetre prete a passer a la FFT a chaque hop.
 *
 *  STRATEGIE
 *  ---------
 *  Le buffer principal `ring[N_CH][EPOCH_LEN]` est de taille fixe. A chaque
 *  push, on filtre puis on accumule le sample dans un mini-buffer `pending`.
 *  Quand `pending` contient EEG_HOP_LEN (= 166) samples:
 *
 *     1. SHIFT : on decale tout le ring de HOP_LEN positions vers la gauche
 *                (les 166 plus vieux samples sont jetes).
 *     2. FILL  : on recopie les 166 samples de `pending` dans les 166 cases
 *                libres a droite du ring.
 *
 *  -> Le ring reste contigu chronologiquement. On peut passer son adresse
 *     directement a arm_rfft_fast_f32, sans wrap-around a gerer.
 *
 *  FILTRAGE
 *  --------
 *  Les samples sont filtres (BP 1-50 Hz + notch 50 + notch 100) AU MOMENT ou
 *  ils sont pousses, ici dans eeg_window_push_sample(). Le filtre biquad est
 *  stateful (memoire des 2 samples precedents), il DOIT voir le flux continu
 *  des samples, pas des epochs decoupes independamment, sinon discontinuites
 *  aux bords -> artefacts dans le PSD basses frequences (Delta).
 *  Le ring ne contient donc que des samples DEJA filtres, propres.
 *
 *  CHOIX vs CIRCULAR BUFFER
 *  ------------------------
 *  Une alternative classique est le circular buffer (curseur tournant, 0
 *  memmove). Plus efficace en CPU (~50us vs ~100us par hop) mais le contenu
 *  n'est plus contigu chronologiquement -> wrap-around a gerer avant la FFT,
 *  source de bugs silencieux. Shift-and-fill = code en ~10 lignes, debug
 *  trivial, contigu en permanence. Le surcout CPU est negligeable face a la
 *  FFT (~ms). On reverra ce choix si on monte a 64 canaux a 1 kHz.
 *
 *  WARMUP
 *  ------
 *  Au boot, le ring est rempli de zeros. Il faut ~5 hops (~2.5 s) avant que
 *  tous les zeros aient ete pousses dehors et que le ring contienne uniquement
 *  des samples reels. Le flag `epoch_ready` n'est leve qu'a partir de ce
 *  moment-la (compteur `warmup_samples`).
 * ============================================================================
 */

#include "eeg_window.h"
#include "eeg_filter.h"
#include <string.h>

/* === Buffers === */
static float ring   [EEG_N_CHANNELS][EEG_EPOCH_LEN];   // 20 x 666 floats = ~52 kB
static float pending[EEG_N_CHANNELS][EEG_HOP_LEN];     // 20 x 166 floats = ~13 kB

/* === Etat === */
static int  pending_idx;       // 0 .. EEG_HOP_LEN-1
static int  warmup_samples;    // samples recus depuis le boot, plafonne a EPOCH_LEN
static bool epoch_ready_flag;

void eeg_window_init(void)
{
    memset(ring,    0, sizeof(ring));
    memset(pending, 0, sizeof(pending));
    pending_idx      = 0;
    warmup_samples   = 0;
    epoch_ready_flag = false;

    eeg_filter_init();
    eeg_features_init();
}

void eeg_window_push_sample(const float sample[EEG_N_CHANNELS])
{
    /* 1) Filtre stateful canal par canal, 1 sample a la fois */
    for (int c = 0; c < EEG_N_CHANNELS; c++) {
        float x = sample[c];
        eeg_filter_apply(c, &x, 1);
        pending[c][pending_idx] = x;
    }
    pending_idx++;

    if (warmup_samples < EEG_EPOCH_LEN) {
        warmup_samples++;
    }

    /* 2) pending plein -> shift ring de HOP_LEN, fill tail avec pending */
    if (pending_idx == EEG_HOP_LEN) {
        for (int c = 0; c < EEG_N_CHANNELS; c++) {
            memmove(&ring[c][0],
                    &ring[c][EEG_HOP_LEN],
                    (EEG_EPOCH_LEN - EEG_HOP_LEN) * sizeof(float));
            memcpy(&ring[c][EEG_EPOCH_LEN - EEG_HOP_LEN],
                   pending[c],
                   EEG_HOP_LEN * sizeof(float));
        }
        pending_idx = 0;

        if (warmup_samples >= EEG_EPOCH_LEN) {
            epoch_ready_flag = true;
        }
    }
}

bool eeg_window_ready(void)
{
    return epoch_ready_flag;
}

void eeg_window_get_epoch(float out[EEG_N_CHANNELS][EEG_EPOCH_LEN])
{
    memcpy(out, ring, sizeof(ring));
    epoch_ready_flag = false;
}
