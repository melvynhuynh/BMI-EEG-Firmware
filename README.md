# BMI-EEG-Firmware

Embedded EEG movement decoder running on an **STM32 Nucleo-WBA65RI** board.
The firmware streams 20-channel EEG, filters it, extracts band power features, and classifies the current movement among 6 classes using a logistic-regression model trained offline in scikit-learn.

This is the on-board (firmware) side of the **n.pulse** BMI project.

---

## Repository layout

```
.
├── eeg_decoder_nucleo_wba65/   STM32CubeIDE project (firmware)
│   ├── Core/                   Application code (main.c, EEG pipeline)
│   │   ├── Inc/                Headers (eeg_window, eeg_features, eeg_model, ...)
│   │   └── Src/                Sources
│   ├── Drivers/                STM32 HAL + CMSIS + BSP (Nucleo-WBA65)
│   ├── tools/                  Python helpers to (re)generate C coefficients
│   │   ├── gen_filter_coefs.py     -> bandpass + 50/100 Hz notch SOS
│   │   └── gen_model_coefs.py      -> eeg_model_params.h from clf.pkl
│   ├── STM32WBA65RIVX_FLASH.ld
│   ├── STM32WBA65RIVX_RAM.ld
│   └── eeg_decoder_nucleo_wba65.ioc
└── model/
    └── clf.pkl                 Trained sklearn pipeline (StandardScaler + OvR LR)
```

---

## Pipeline

Defined in `eeg_decoder_nucleo_wba65/Core/Inc/eeg_features.h`:

| Parameter                | Value                  |
|--------------------------|------------------------|
| Sampling rate            | 333 Hz                 |
| Channels                 | 20                     |
| Window length            | 2 s (666 samples)      |
| Hop (btw 2 predictions)  | ~0.5 s (166 samples)   |
| FFT length               | 512                    |
| Features                 | 20 × 5 = 100           |
| Classes                  | 6                      |

**Stages**

1. **Filter** - per-channel Butterworth bandpass (1–50 Hz) + 50 Hz / 100 Hz notches, stateful biquads (`eeg_filter.c`).
2. **Sliding window** - shift-and-fill ring buffer producing 2 s epochs every ~0.5 s (`eeg_window.c`).
3. **Band power** - Welch-style PSD per channel, integrated over each band (`eeg_features.c`).
4. **Classifier** - `(x - mean) / scale` then `argmax` over `OneVsRest(LogisticRegression)`; sigmoid only on the winning score (`eeg_model.c`).

---
