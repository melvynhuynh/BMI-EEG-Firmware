/*
 * eeg_filter.c
 *
 *  Created on: May 13, 2026
 *      Author: melvy
 */

#include "eeg_filter.h"
#include "arm_math.h"

/* === Coefficients générés par tools/gen_filter_coefs.py (fs=333 Hz) === */

static const float bp_coefs[20] = {
    +0.0174405667f, +0.0348811334f, +0.0174405667f, +0.7028726986f, -0.1565292883f,
    +1.0000000000f, +2.0000000000f, +1.0000000000f, +0.9088537593f, -0.5394917155f,
    +1.0000000000f, -2.0000000000f, +1.0000000000f, +1.9644669264f, -0.9648399561f,
    +1.0000000000f, -2.0000000000f, +1.0000000000f, +1.9856847139f, -0.9860418661f,
};
#define NUM_BP_STAGES 4

static const float n50_coefs[5] = {
    +0.9845184640f, -1.1558674974f, +0.9845184640f, +1.1558674974f, -0.9690369280f,
};

static const float n100_coefs[5] = {
    +0.9695016643f, +0.6026634415f, +0.9695016643f, -0.6026634415f, -0.9390033287f,
};

/* === Biquad instances + state buffers (un par canal) === */
static arm_biquad_cascade_df2T_instance_f32 bp[EEG_N_CHANNELS];
static arm_biquad_cascade_df2T_instance_f32 n50[EEG_N_CHANNELS];
static arm_biquad_cascade_df2T_instance_f32 n100[EEG_N_CHANNELS];

static float bp_state  [EEG_N_CHANNELS][2 * NUM_BP_STAGES];
static float n50_state [EEG_N_CHANNELS][2];
static float n100_state[EEG_N_CHANNELS][2];

void eeg_filter_init(void) {
    for (int c = 0; c < EEG_N_CHANNELS; c++) {
        arm_biquad_cascade_df2T_init_f32(&bp[c],   NUM_BP_STAGES, (float*)bp_coefs,   bp_state[c]);
        arm_biquad_cascade_df2T_init_f32(&n50[c],  1,             (float*)n50_coefs,  n50_state[c]);
        arm_biquad_cascade_df2T_init_f32(&n100[c], 1,             (float*)n100_coefs, n100_state[c]);
    }
}

void eeg_filter_apply(int ch, float *block, int n) {
    arm_biquad_cascade_df2T_f32(&bp[ch],   block, block, n);
    arm_biquad_cascade_df2T_f32(&n50[ch],  block, block, n);
    arm_biquad_cascade_df2T_f32(&n100[ch], block, block, n);
}


