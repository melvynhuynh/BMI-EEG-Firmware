/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "eeg_window.h"
#include "eeg_features.h"
#include "eeg_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;

UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */
/* Buffers tailles "epoch" -> en BSS (static), JAMAIS sur la stack (default 16kB). */
static float epoch_buf[EEG_N_CHANNELS][EEG_EPOCH_LEN];  /* ~52 kB */
static float features [EEG_N_FEATURES];                 /* ~0.4 kB */
static float sample   [EEG_N_CHANNELS];                 /* ~0.08 kB */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Generateur N(0,1) par Box-Muller.
 * Sert a ajouter un petit bruit aleatoire au signal EEG synthetique. */
static float randnf(void)
{
    float u1 = ((float)rand() + 1.0f) / ((float)RAND_MAX + 1.0f);
    float u2 = (float)rand() / (float)RAND_MAX;
    return sqrtf(-2.0f * logf(u1)) * cosf(2.0f * 3.14159265f * u2);
}

/* Genere un faux signal EEG brut pour une classe cible.
 *
 * target_label:
 *   1 = rest
 *   2 = hand open
 *   3 = hand close
 *   4 = index finger
 *   5 = pinch
 *   6 = wrist movement
 *
 * Attention:
 * target_label ne garantit pas que le modele va predire cette classe.
 * La fonction cree seulement des patterns differents de band-power
 * pour tester si le pipeline reagit.
 */
static void generate_target_class_eeg(float *sample, int n, int target_label)
{
    float t = (float)n / (float)EEG_FS;

    for (int c = 0; c < EEG_N_CHANNELS; c++) {
        float phase = 0.13f * c;

        float A_delta = 0.2f;
        float A_theta = 0.2f;
        float A_alpha = 0.2f;
        float A_beta  = 0.2f;
        float A_gamma = 0.2f;

        if (target_label == 1) {
            /* Rest: mixed baseline, rather calm signal */
            A_delta = 0.25f;
            A_theta = 0.35f;
            A_alpha = 1.00f;
            A_beta  = 0.60f;
            A_gamma = 0.15f;
        }
        else if (target_label == 2) {
            /* Hand open: theta/beta forts sur channels 7..13 */
            if (c >= 7 && c < 14) {
                A_theta = 2.5f;
                A_beta  = 1.5f;
            }
        }
        else if (target_label == 3) {
            /* Hand close: delta/theta forts sur channels 14..19 */
            A_delta = 0.05f;
            A_theta = 0.05f;
            A_alpha = 0.05f;
            A_beta  = 0.05f;
            A_gamma = 0.05f;

            if (c >= 14) {
                A_delta = 5.0f;
                A_theta = 3.0f;
            }
        }
        else if (target_label == 4) {
            /* Index finger: beta fort sur channels 0..9 */
            if (c < 10) {
                A_beta = 3.0f;
            }
        }
        else if (target_label == 5) {
            /* Pinch: gamma/alpha forts sur channels 10..19 */
            if (c >= 10) {
                A_gamma = 3.0f;
                A_alpha = 1.2f;
            }
        }
        else if (target_label == 6) {
            /* Wrist movement: signal mixte type baseline/movement */
            A_delta = 0.3f;
            A_theta = 0.4f;
            A_alpha = 1.0f;
            A_beta  = 0.7f;
            A_gamma = 0.2f;
        }

        sample[c] =
            A_delta * sinf(2.0f * 3.14159265f * 2.0f  * t + phase) +
            A_theta * sinf(2.0f * 3.14159265f * 6.0f  * t + phase) +
            A_alpha * sinf(2.0f * 3.14159265f * 10.0f * t + phase) +
            A_beta  * sinf(2.0f * 3.14159265f * 20.0f * t + phase) +
            A_gamma * sinf(2.0f * 3.14159265f * 40.0f * t + phase) +
            0.05f * randnf();
    }
}

/* Return a random label between 1 and 6, different from current_label. */
static int random_next_label(int current_label)
{
    int new_label = current_label;

    while (new_label == current_label) {
        new_label = (rand() % 6) + 1;
    }

    return new_label;
}

/* Optional helper for readable terminal output. */
static const char* movement_name(int label)
{
    switch (label) {
        case 1: return "rest";
        case 2: return "hand_open";
        case 3: return "hand_close";
        case 4: return "index_finger";
        case 5: return "pinch";
        case 6: return "wrist_movement";
        default: return "unknown";
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  MX_GPIO_Init();
  MX_LPUART1_UART_Init();

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_RED);

  BSP_PB_Init(B1, BUTTON_MODE_EXTI);
  BSP_PB_Init(B2, BUTTON_MODE_EXTI);
  BSP_PB_Init(B3, BUTTON_MODE_EXTI);

  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;

  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN WHILE */

  printf("\r\n=== EEG decoder boot ===\r\n");
  printf("FS=%d Hz | EPOCH=%d samples (%d s) | HOP=%d samples (~0.5 s)\r\n",
         EEG_FS, EEG_EPOCH_LEN, EEG_EPOCH_SEC, EEG_HOP_LEN);
  printf("Feeding randomized synthetic EEG-like movement signals.\r\n");
  printf("Each synthetic movement is kept for a random duration before switching.\r\n\r\n");

  eeg_window_init();
  eeg_model_init();

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    static int n = 0;
    static int target_label = 1;
    static int pred_count = 0;

    /*
     * Number of predictions before switching movement.
     * With HAL_Delay(500), 4..10 predictions ~= 2..5 seconds.
     */
    static int switch_after = 8;

    /* 1) Push EEG_HOP_LEN synthetic samples into the EEG window */
    for (int i = 0; i < EEG_HOP_LEN; i++) {
        generate_target_class_eeg(sample, n, target_label);
        eeg_window_push_sample(sample);
        n++;
    }

    /* 2) If epoch is ready: extract band power + predict + print */
    if (eeg_window_ready()) {
        eeg_window_get_epoch(epoch_buf);
        eeg_extract_band_power(epoch_buf, features);
        eeg_prediction_t pred = eeg_model_predict(features);

        int conf_milli = (int)(pred.confidence * 1000.0f);
        int s[EEG_N_CLASSES];

        for (int k = 0; k < EEG_N_CLASSES; k++) {
            s[k] = (int)(pred.all_scores[k] * 100.0f);
        }

        printf("target=%d(%s) -> pred=%d(%s) idx=%d conf=%d.%03d  scores(x0.01)=[%+5d %+5d %+5d %+5d %+5d %+5d]\r\n",
               target_label,
               movement_name(target_label),
               pred.class_label,
               movement_name(pred.class_label),
               pred.class_idx,
               conf_milli / 1000, conf_milli % 1000,
               s[0], s[1], s[2], s[3], s[4], s[5]);

        pred_count++;

        /* Randomized movement switching */
        if (pred_count >= switch_after) {
            int old_label = target_label;

            target_label = random_next_label(old_label);
            pred_count = 0;

            /*
             * Random duration between 4 and 10 predictions.
             * With HAL_Delay(500), this is roughly 2 to 5 seconds.
             */
            switch_after = 4 + (rand() % 7);

            printf("\r\n--- random switch: target %d(%s) -> target %d(%s), next duration=%d predictions ---\r\n\r\n",
                   old_label,
                   movement_name(old_label),
                   target_label,
                   movement_name(target_label),
                   switch_after);

            /*
             * Reset window to avoid mixing old and new synthetic movements
             * inside the same 2-second EEG epoch.
             */
            eeg_window_init();
        }
    }

    HAL_Delay(500);

    /* USER CODE END 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL1.PLLM = 2;
  RCC_OscInitStruct.PLL1.PLLN = 16;
  RCC_OscInitStruct.PLL1.PLLP = 2;
  RCC_OscInitStruct.PLL1.PLLQ = 2;
  RCC_OscInitStruct.PLL1.PLLR = 2;
  RCC_OscInitStruct.PLL1.PLLFractional = 4096;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                              | RCC_CLOCKTYPE_PCLK7 | RCC_CLOCKTYPE_HCLK5;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB7CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHB5_PLL1_CLKDivider = RCC_SYSCLK_PLL1_DIV4;
  RCC_ClkInitStruct.AHB5_HSEHSI_CLKDivider = RCC_SYSCLK_HSEHSI_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{
  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */

  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;

  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = USB_FS_N_Pin | USB_FS_P_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = VCP2_RTS_Pin | VCP2_TX_Pin | VCP2_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = VCP2_CTS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_USART2;
  HAL_GPIO_Init(VCP2_CTS_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: source line number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
