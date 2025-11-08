/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define FINGERPRINT_OK 0x00               //!< Command execution is complete
#define FINGERPRINT_PACKETRECIEVEERR 0x01 //!< Error when receiving data package
#define FINGERPRINT_NOFINGER 0x02         //!< No finger on the sensor
#define FINGERPRINT_IMAGEFAIL 0x03        //!< Failed to enroll the finger
#define FINGERPRINT_IMAGEMESS                                                  \
  0x06 //!< Failed to generate character file due to overly disorderly
       //!< fingerprint image
#define FINGERPRINT_FEATUREFAIL                                                \
  0x07 //!< Failed to generate character file due to the lack of character point
       //!< or small fingerprint image
#define FINGERPRINT_NOMATCH 0x08  //!< Finger doesn't match
#define FINGERPRINT_NOTFOUND 0x09 //!< Failed to find matching finger
#define FINGERPRINT_ENROLLMISMATCH                                             \
  0x0A //!< Failed to combine the character files
#define FINGERPRINT_BADLOCATION                                                \
  0x0B //!< Addressed PageID is beyond the finger library
#define FINGERPRINT_DBRANGEFAIL                                                \
  0x0C //!< Error when reading template from library or invalid template
#define FINGERPRINT_UPLOADFEATUREFAIL 0x0D //!< Error when uploading template
#define FINGERPRINT_PACKETRESPONSEFAIL                                         \
  0x0E //!< Module failed to receive the following data packages
#define FINGERPRINT_UPLOADFAIL 0x0F  //!< Error when uploading image
#define FINGERPRINT_DELETEFAIL 0x10  //!< Failed to delete the template
#define FINGERPRINT_DBCLEARFAIL 0x11 //!< Failed to clear finger library
#define FINGERPRINT_PASSFAIL                                                   \
  0x13 //!< Find whether the fingerprint passed or failed
#define FINGERPRINT_INVALIDIMAGE                                               \
  0x15 //!< Failed to generate image because of lac of valid primary image
#define FINGERPRINT_FLASHERR 0x18   //!< Error when writing flash
#define FINGERPRINT_INVALIDREG 0x1A //!< Invalid register number
#define FINGERPRINT_ADDRCODE 0x20   //!< Address code
#define FINGERPRINT_PASSVERIFY 0x21 //!< Verify the fingerprint passed
#define FINGERPRINT_STARTCODE                                                  \
  0xEF01 //!< Fixed falue of EF01H; High byte transferred first

#define FINGERPRINT_COMMANDPACKET 0x1 //!< Command packet
#define FINGERPRINT_DATAPACKET                                                 \
  0x2 //!< Data packet, must follow command packet or acknowledge packet
#define FINGERPRINT_ACKPACKET 0x7     //!< Acknowledge packet
#define FINGERPRINT_ENDDATAPACKET 0x8 //!< End of data packet

#define FINGERPRINT_TIMEOUT 0xFF   //!< Timeout was reached
#define FINGERPRINT_BADPACKET 0xFE //!< Bad packet was sent

#define FINGERPRINT_GETIMAGE 0x01 //!< Collect finger image
#define FINGERPRINT_IMAGE2TZ 0x02 //!< Generate character file from image
#define FINGERPRINT_SEARCH 0x04   //!< Search for fingerprint in slot
#define FINGERPRINT_REGMODEL                                                   \
  0x05 //!< Combine character files and generate template
#define FINGERPRINT_STORE 0x06          //!< Store template
#define FINGERPRINT_LOAD 0x07           //!< Read/load template
#define FINGERPRINT_UPLOAD 0x08         //!< Upload template
#define FINGERPRINT_DELETE 0x0C         //!< Delete templates
#define FINGERPRINT_EMPTY 0x0D          //!< Empty library
#define FINGERPRINT_READSYSPARAM 0x0F   //!< Read system parameters
#define FINGERPRINT_SETPASSWORD 0x12    //!< Sets passwords
#define FINGERPRINT_VERIFYPASSWORD 0x13 //!< Verifies the password
#define FINGERPRINT_HISPEEDSEARCH                                              \
  0x1B //!< Asks the sensor to search for a matching fingerprint template to the
       //!< last model generated
#define FINGERPRINT_TEMPLATECOUNT 0x1D //!< Read finger template numbers
#define FINGERPRINT_AURALEDCONFIG 0x35 //!< Aura LED control
#define FINGERPRINT_LEDON 0x50         //!< Turn on the onboard LED
#define FINGERPRINT_LEDOFF 0x51        //!< Turn off the onboard LED

#define FINGERPRINT_LED_BREATHING 0x01   //!< Breathing light
#define FINGERPRINT_LED_FLASHING 0x02    //!< Flashing light
#define FINGERPRINT_LED_ON 0x03          //!< Always on
#define FINGERPRINT_LED_OFF 0x04         //!< Always off
#define FINGERPRINT_LED_GRADUAL_ON 0x05  //!< Gradually on
#define FINGERPRINT_LED_GRADUAL_OFF 0x06 //!< Gradually off
#define FINGERPRINT_LED_RED 0x01         //!< Red LED
#define FINGERPRINT_LED_BLUE 0x02        //!< Blue LED
#define FINGERPRINT_LED_PURPLE 0x03      //!< Purple LEDpassword

#define FINGERPRINT_REG_ADDR_ERROR 0x1A //!< shows register address error
#define FINGERPRINT_WRITE_REG 0x0E      //!< Write system register instruction

#define FINGERPRINT_BAUD_REG_ADDR 0x4   //!< BAUDRATE register address
#define FINGERPRINT_BAUDRATE_9600 0x1   //!< UART baud 9600
#define FINGERPRINT_BAUDRATE_19200 0x2  //!< UART baud 19200
#define FINGERPRINT_BAUDRATE_28800 0x3  //!< UART baud 28800
#define FINGERPRINT_BAUDRATE_38400 0x4  //!< UART baud 38400
#define FINGERPRINT_BAUDRATE_48000 0x5  //!< UART baud 48000
#define FINGERPRINT_BAUDRATE_57600 0x6  //!< UART baud 57600
#define FINGERPRINT_BAUDRATE_67200 0x7  //!< UART baud 67200
#define FINGERPRINT_BAUDRATE_76800 0x8  //!< UART baud 76800
#define FINGERPRINT_BAUDRATE_86400 0x9  //!< UART baud 86400
#define FINGERPRINT_BAUDRATE_96000 0xA  //!< UART baud 96000
#define FINGERPRINT_BAUDRATE_105600 0xB //!< UART baud 105600
#define FINGERPRINT_BAUDRATE_115200 0xC //!< UART baud 115200

#define FINGERPRINT_SECURITY_REG_ADDR 0x5 //!< Security level register address
// The safety level is 1 The highest rate of false recognition , The rejection
// rate is the lowest . The safety level is 5 The lowest tate of false
// recognition, The rejection rate is the highest .
#define FINGERPRINT_SECURITY_LEVEL_1 0X1 //!< Security level 1
#define FINGERPRINT_SECURITY_LEVEL_2 0X2 //!< Security level 2
#define FINGERPRINT_SECURITY_LEVEL_3 0X3 //!< Security level 3
#define FINGERPRINT_SECURITY_LEVEL_4 0X4 //!< Security level 4
#define FINGERPRINT_SECURITY_LEVEL_5 0X5 //!< Security level 5

#define FINGERPRINT_PACKET_REG_ADDR 0x6 //!< Packet size register address
#define FINGERPRINT_PACKET_SIZE_32 0X0  //!< Packet size is 32 Byte
#define FINGERPRINT_PACKET_SIZE_64 0X1  //!< Packet size is 64 Byte
#define FINGERPRINT_PACKET_SIZE_128 0X2 //!< Packet size is 128 Byte
#define FINGERPRINT_PACKET_SIZE_256 0X3 //!< Packet size is 256 Byte

#define DEFAULTTIMEOUT 1000  // UART reading timeout in milliseconds


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

  uint8_t UART1_rxBuffer[18] = {0};

  uint8_t paquete_comando[] = {
      0xEF, 0x01,             // Start code
      0xFF, 0xFF, 0xFF, 0xFF, // Dirección del dispositivo
      0x01,                   // Tipo de paquete: comando
      0x00, 0x07,             // Longitud del paquete (payload + checksum)
      0x13,                   // Comando: VerifyPassword (ejemplo)
      0x00, 0x00, 0x00, 0x00, // Contraseña (32-bit)
      0x00, 0x1B              // Checksum
  };

  volatile uint8_t flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, UART1_rxBuffer, 18);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  uint8_t sent[] = "Sent:\r\n";
	  HAL_UART_Transmit(&huart3, sent, 8, 100);
	  HAL_UART_Transmit(&huart3, paquete_comando, 16, 100);

	  HAL_UART_Transmit(&huart1, paquete_comando, 16, 100);
	  HAL_UART_Transmit(&huart3, UART1_rxBuffer, 18, 100);
	  HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the programming delay
  */
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_1);
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_BTN_Pin */
  GPIO_InitStruct.Pin = USER_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USER_LED_Pin */
  GPIO_InitStruct.Pin = USER_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(USER_LED_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI13_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t received[] = "Received:\r\n";
    HAL_UART_Transmit(&huart3, received, 12, 100);

    HAL_UART_Transmit(&huart3, UART1_rxBuffer, 18, 100);
    HAL_UART_Receive_IT(&huart1, UART1_rxBuffer, 18);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
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
  * @param  line: assert_param error line source number
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
