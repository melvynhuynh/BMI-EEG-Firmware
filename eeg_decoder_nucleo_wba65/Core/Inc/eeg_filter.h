/*
 * eeg_filter.h
 *
 *  Created on: May 13, 2026
 *      Author: melvy
 */

#ifndef EEG_FILTER_H
#define EEG_FILTER_H

#include "eeg_features.h"

/* Initialise les biquads pour les EEG_N_CHANNELS canaux */
void eeg_filter_init(void);

/* Filtre un bloc de samples in-place (un canal à la fois)
   - ch     : index du canal (0 .. EEG_N_CHANNELS-1)
   - block  : buffer de samples float, modifié en place
   - n      : nombre de samples dans le bloc */
void eeg_filter_apply(int ch, float *block, int n);

#endif /* EEG_FILTER_H */
