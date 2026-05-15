/*
 * eeg_features.h
 *
 *  Created on: May 13, 2026
 *      Author: melvy
 */

#ifndef EEG_FEATURES_H
#define EEG_FEATURES_H

#include <stdint.h>

/* === Pipeline parameters === */
#define EEG_FS              333                              // sampling frequency (Hz)
#define EEG_N_CHANNELS      20                               // number of EEG channels
#define EEG_EPOCH_SEC       2                                // window length (seconds)
#define EEG_EPOCH_LEN       (EEG_FS * EEG_EPOCH_SEC)         // 666 samples per channel
#define EEG_HOP_SEC         0.5f                             // hop between predictions (seconds) -- doc only
#define EEG_HOP_LEN         (EEG_FS / 2)                     // 166 samples = ~0.5s at 333 Hz
#define EEG_NPERSEG         EEG_FS                           // 333 samples = 1 s segments
#define EEG_NOVERLAP        (EEG_FS / 2)                     // 166 samples = 50% overlap
#define EEG_FFT_LEN         512                              // smallest pow2 >= nperseg
#define EEG_N_BANDS         5                                // Delta, Theta, Alpha, Beta, Gamma
#define EEG_N_FEATURES      (EEG_N_CHANNELS * EEG_N_BANDS)   // 100 features
#define EEG_N_CLASSES       6                                // movements

/* === API === */
void  eeg_features_init(void);
void  eeg_extract_band_power(const float epoch[EEG_N_CHANNELS][EEG_EPOCH_LEN],
                              float features[EEG_N_FEATURES]);

#endif /* EEG_FEATURES_H */
