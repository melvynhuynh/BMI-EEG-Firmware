/*
 * eeg_model.h
 *
 *  Created on: May 13, 2026
 *      Author: melvy
 *
 *  Inference C-side: standardisation + OneVsRest LR (6 classes).
 *  Lit les parametres depuis eeg_model_params.h (auto-genere par
 *  tools/gen_model_coefs.py a partir du clf.pkl de Charles).
 */

#ifndef INC_EEG_MODEL_H_
#define INC_EEG_MODEL_H_

#include "eeg_features.h"

/* Resultat d'une prediction. */
typedef struct {
    int   class_idx;                      // 0..EEG_N_CLASSES-1, index dans model_coef
    int   class_label;                    // Label "Charles" (1..6), = class_idx+1
    float confidence;                     // sigmoid(score_winner), in [0,1]
    float all_scores[EEG_N_CLASSES];      // Raw w.x+b par classe, debug / inspection
} eeg_prediction_t;

/* Init du module (no-op pour l'instant, garde la coherence d'API). */
void eeg_model_init(void);

/* Prediction sur un vecteur de features (band power non standardisees, dans
 * le meme ordre que eeg_extract_band_power).
 *
 * Pipeline interne:
 *   1) standardise x = (features - scaler_mean) / scaler_scale
 *   2) pour chaque classe c: score_c = dot(model_coef[c], x) + model_intercept[c]
 *   3) class_idx = argmax(score_c)
 *   4) confidence = sigmoid(score_winner)
 *
 * Note: argmax sur les scores bruts est equivalent a argmax sur sigmoid(scores)
 *       car sigmoid est monotone -> on n'appelle sigmoid qu'une seule fois,
 *       sur le gagnant, pour gagner ~5 expf().
 */
eeg_prediction_t eeg_model_predict(const float features[EEG_N_FEATURES]);

#endif /* INC_EEG_MODEL_H_ */
