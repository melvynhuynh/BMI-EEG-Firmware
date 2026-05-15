/*
 * eeg_window.h
 *
 *  Created on: May 14, 2026
 *      Author: melvy
 *
 *  Sliding window for the EEG decoder pipeline.
 *  See eeg_window.c for the "shift-and-fill" strategy details.
 */

#ifndef EEG_WINDOW_H
#define EEG_WINDOW_H

#include <stdbool.h>
#include "eeg_features.h"

/* Initialise les buffers internes, l'etat des filtres biquads et les tables
 * FFT/Hann. A appeler une seule fois au boot, AVANT le premier push_sample. */
void eeg_window_init(void);

/* Pousse un nouveau sample par canal (a la frequence EEG_FS).
 * En interne:
 *   1) applique le filtre BP + notches (stateful, par canal)
 *   2) range dans le mini-buffer `pending`
 *   3) une fois EEG_HOP_LEN samples accumules, decale le ring principal
 *      vers la gauche de HOP_LEN positions et y colle le pending.
 *      Puis (si le ring est plein de samples reels) leve le flag "ready". */
void eeg_window_push_sample(const float sample[EEG_N_CHANNELS]);

/* True si un nouvel epoch (EEG_EPOCH_LEN samples filtres par canal) est
 * disponible et n'a pas encore ete consomme via eeg_window_get_epoch().
 * Devient true des que le ring a vu au moins EEG_EPOCH_LEN samples reels
 * ET qu'un hop frais a ete accumule depuis le dernier get_epoch. */
bool eeg_window_ready(void);

/* Copie les EEG_EPOCH_LEN samples les plus recents (filtres, ranges
 * chronologiquement) dans `out`, puis baisse le flag "ready". */
void eeg_window_get_epoch(float out[EEG_N_CHANNELS][EEG_EPOCH_LEN]);

#endif /* EEG_WINDOW_H */
