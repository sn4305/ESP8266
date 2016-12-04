/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComPolling/Src/main.c 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    06-May-2016
  * @brief   This sample code shows how to use UART HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          polling transfer. 
  *          The communication is done using 2 Boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_TwoBoards_ComPolling
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TRANSMITTER_BOARD

/* Private macro -------------------------------------------------------------*/
#define SSID "Tony"        //type your own SSID name
#define PASSWORD "yangdong"        //type your own WIFI password 

/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
//__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */

/* Buffer used for transmission */
//uint8_t aTxBuffer[] = " **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling ****  **** UART_TwoBoards_ComPolling **** ";
 uint8_t aTxBuffer[30] = "AT\r\n";

/* Buffer used for reception */
 uint8_t aRxBuffer[28];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
//static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/
static int16_t strindex(uint8_t *s,uint8_t *t)
{
  int16_t i,n;

  n=strlen(t);
  for(i=0;*(s+i); i++) 
  {
    if (strncmp(s+i,t,n) == 0)
      return i;
  }
  return -1;
}


static uint8_t WIFI_Init(void)
{
  while(1)
  {
//strcpy(aTxBuffer,"AT+CWMODE=3\r\n");
//   if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer,13, 1000)!= HAL_OK)
//  {
//    Error_Handler();   
//  } 
//  HAL_Delay(500);
//  
//memset(aTxBuffer,0x00,30);  
//strcpy(aTxBuffer,"AT+CWJAP=\"Tony\",\"yangdong\"\r\n");
//   if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer,28, 1000)!= HAL_OK)
//  {
//    Error_Handler();   
//  } 
//   HAL_Delay(500);

  memset(aTxBuffer,0x00,30);  
strcpy(aTxBuffer,"AT+CIPMUX=1\r\n");
   if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer,13, 1000)!= HAL_OK)
  {
    Error_Handler();   
  }
   HAL_Delay(1500);
  
   memset(aTxBuffer,0x00,30);  
strcpy(aTxBuffer,"AT+CIPSERVER=1,8080\r\n");
   if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer,21, 1000)!= HAL_OK)
  {
    Error_Handler();   
  } 

   memset(aRxBuffer,0x00,28);
      if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, 28, 1000)==HAL_OK && strindex(aRxBuffer,"OK")>=0x0000)
      {
        return 0x01;
      }
      
     else if(strindex(aRxBuffer,"ERROR")>=0x0000)
     {
       /* Error: Toggle LED3*/
      BSP_LED_Toggle(LED3); 
      HAL_Delay(1000);
     }
    
     
  }
}


static uint8_t Client_Detection(void)
{
  while(1)
  { memset(aRxBuffer,0x00,28);
    HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, 24, 300);
  
  if(strindex(aRxBuffer,"CONNECT")>=0x0000)
    {
    /* CONNECT: Toggle LED2*/
    BSP_LED_Toggle(LED2); 
    HAL_Delay(500);
    return 0x01; 
    }
  else  
    {   
      BSP_LED_Toggle(LED2); 
       HAL_Delay(200);
    }      
  }
}


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization: global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
  
  /* Configure LED1, LED2, LED3 and LED4 */
//  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate     = 115200;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef TRANSMITTER_BOARD

  /* Configure User push-button in Interrupt mode */
//  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
//  
//  /* Wait for User push-button press before starting the Communication.
//     In the meantime, LED2 is blinking */
//  while(UserButtonStatus == 0)
//  {
//      /* Toggle LED2*/
//      BSP_LED_Toggle(LED2); 
//      HAL_Delay(100);
//  }
  
//  BSP_LED_Off(LED2);
  
  /* The board sends the message and expects to receive it back */
  
  /*##-2- Start the transmission process #####################################*/  
  /* While the UART in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, 4, 5000)!= HAL_OK)
  {
    Error_Handler();   
  }
 // HAL_Delay(10);
//  /* Turn LED4 on: Transfer in transmission process is correct */
//  BSP_LED_On(LED4);
  
  /*##-3- Put UART peripheral in reception process ###########################*/
  //memset(aRxBuffer,0xFF,28);  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, 28, 5000) != HAL_OK)
  {
    Error_Handler();  
  }
   if(strindex(aRxBuffer,"OK")>=0)
   {
     /* Turn LED3 on: Transfer in reception process is correct */
     BSP_LED_On(LED2);
     HAL_Delay(1000);
   }
 
#else
//  
//  /* The board receives the message and sends it back */
//
//  /*##-2- Put UART peripheral in reception process ###########################*/
//  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 0x1FFFFFF) != HAL_OK)
//  {
//    Error_Handler();
//  }
// 
//  /* Turn LED3 on: Transfer in reception process is correct */
//  BSP_LED_On(LED3);
//  
//  /*##-3- Start the transmission process #####################################*/  
//  /* While the UART in reception process, user can transmit data through 
//     "aTxBuffer" buffer */
//  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
//  {
//    Error_Handler();
//  }
//  
//  /* Turn LED4 on: Transfer in transmission process is correct */
//  BSP_LED_On(LED4);
  
#endif /* TRANSMITTER_BOARD */
  
  /*##-4- Compare the sent and received buffers ##############################*/
//  if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer,RXBUFFERSIZE))
//  {
//    Error_Handler();
//  }
   
  
  if(WIFI_Init()==0x01 && Client_Detection()==0x01)
  {
  /* Infinite loop */
    while(1)
  { memset(aRxBuffer,0x00,28);
    HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, 24, 500);
    if(strindex(aRxBuffer,"6:LED_ON")>=0x0008)
    {
      BSP_LED_On(LED4);
//      BSP_LED_Toggle(LED2);
//      HAL_Delay(10); 
    }
    else if(strindex(aRxBuffer,"7:LED_OFF")>=0x0008)
   {
    BSP_LED_Off(LED4);            
    // HAL_Delay(10);
   }
  else if(strindex(aRxBuffer,"CLOSE")>=0x0000 || strindex(aRxBuffer,"ERROR")>=0x0000)
   {
      /* Toggle RED_LED3_*/
      BSP_LED_Toggle(LED3); 
      HAL_Delay(100); 
      BSP_LED_Toggle(LED3); 
      HAL_Delay(300); 
      BSP_LED_Toggle(LED3); 
      HAL_Delay(100); 
      BSP_LED_Toggle(LED3); 
      HAL_Delay(300); 
      BSP_LED_Toggle(LED3); 
      HAL_Delay(100); 
      BSP_LED_Toggle(LED3); 
      HAL_Delay(300); 
   }
   
  }        
  }
}           
           
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
#if defined(USE_STM32469I_DISCO_REVA)
  RCC_OscInitStruct.PLL.PLLM = 25;
#else
  RCC_OscInitStruct.PLL.PLLM = 8;
#endif /* USE_STM32469I_DISCO_REVA */
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 180 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED1 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED1); 
}


/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//  if(GPIO_Pin == USER_BUTTON_PIN)
//  {  
//    UserButtonStatus = 1;
//  }
//}
/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
//static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
//{
//  while (BufferLength--)
//  {
//    if ((*pBuffer1) != *pBuffer2)
//    {
//      return BufferLength;
//    }
//    pBuffer1++;
//    pBuffer2++;
//  }
//
//  return 0;
//}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
