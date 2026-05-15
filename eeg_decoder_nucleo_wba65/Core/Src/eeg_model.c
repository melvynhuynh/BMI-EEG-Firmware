/*
 * eeg_model.c
 *
 *  Created on: May 13, 2026
 *      Author: melvy
 *
 *  Inference du OneVsRestClassifier(LogisticRegression) entraine par Charles.
 *  Tous les parametres viennent de eeg_model_params.h (genere par
 *  tools/gen_model_coefs.py a partir de clf.pkl).
 *
 *  Equivalent Python:
 *     pred = clf.predict(features.reshape(1, -1))[0]
 *  ou:
 *     proba = clf.predict_proba(features.reshape(1, -1))[0]
 *     pred  = proba.argmax()
 *
 *  Cout CPU estime sur Cortex-M33 @ 100 MHz:
 *   - standardisation (sub + div, 100 floats)       ~  3 us
 *   - 6 x arm_dot_prod_f32(100)                     ~  3 us
 *   - 1 x expf (sigmoid sur winner)                 ~ <1 us
 *   total                                           ~  7 us  -- negligeable
 *  vs. Welch + FFT en amont (~quelques ms).
 */

#include "eeg_model.h"
#include "eeg_model_params.h"
#include "arm_math.h"
#include <math.h>

/* Buffer reutilise entre appels (400 octets en BSS, pas sur la stack). */
static float standardized[EEG_N_FEATURES];

void eeg_model_init(void)
{
    /* Rien a initialiser : tous les params sont des const arrays en Flash. */
}

/* Sigmoid numeriquement stable :
 *   pour x >= 0 : 1 / (1 + exp(-x))    (exp(-x) <= 1, pas d'overflow)
 *   pour x <  0 : exp(x) / (1 + exp(x))(exp(x)  <  1, pas d'overflow)
 */
static inline float sigmoidf(float x)
{
    if (x >= 0.0f) {
        return 1.0f / (1.0f + expf(-x));
    } else {
        float e = expf(x);
        return e / (1.0f + e);
    }
}

eeg_prediction_t eeg_model_predict(const float features[EEG_N_FEATURES])
{
    eeg_prediction_t pred = { 0 };
    int   best_idx   = 0;
    float best_score = -INFINITY;

    /* 1) Standardisation : x = (features - scaler_mean) / scaler_scale
     *    arm_sub_f32 fait la soustraction vectorisee ; la division element-
     *    wise n'existe pas en f32 dans CMSIS-DSP, on la fait en boucle (cout
     *    negligeable pour 100 floats).                                    */
    arm_sub_f32((float32_t *)features,
                (float32_t *)scaler_mean,
                standardized,
                EEG_N_FEATURES);
    for (int i = 0; i < EEG_N_FEATURES; i++) {
        standardized[i] /= scaler_scale[i];
    }

    /* 2) Score brut par classe : w_c . x + b_c                            */
    for (int c = 0; c < EEG_N_CLASSES; c++) {
        float32_t dot;
        arm_dot_prod_f32((float32_t *)model_coef[c],
                         standardized,
                         EEG_N_FEATURES,
                         &dot);
        float score = dot + model_intercept[c];
        pred.all_scores[c] = score;
        if (score > best_score) {
            best_score = score;
            best_idx   = c;
        }
    }

    /* 3) Resultat : on applique sigmoid UNIQUEMENT sur le gagnant         */
    pred.class_idx   = best_idx;
    pred.class_label = best_idx + 1;        // Charles utilise les labels 1..6
    pred.confidence  = sigmoidf(best_score);
    return pred;
}
