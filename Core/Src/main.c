/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */
PID_InitTypeDef PID_Init = {13.904, 0.5, 100.90, 0};                  
                            // 14.2,0.0005,140.1,7.4    13.904,0,0,0
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void LCD_DISPLAY(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t aADCxConvertedData[4];
uint8_t KEY, Mode = 0;
char datatemp1[10][20];
// uint16_t Time_2;
float Vsum, Vall, VO, R_NOW, P_LAST, I_NOW, R_Want = 10.9, WANT;
float Isum, IO, Isum2, Vsum2;
uint16_t Out_Duty = 500;
int updown;
char datatemp2[100]={0};
uint8_t Flag_LCD = 0, Flag_MATH = 0, Flag_ADC = 0, Duty_Flag = 0;
  int i=0,j=0,k=0,num1[5],num2[5];
  float num3[5];
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
 HAL_TIM_Base_Start_IT(&htim2);
 HAL_ADC_ConvCpltCallback(&hadc1);
 HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&aADCxConvertedData, 4);
 Lcd_Init();
 delay_init(80); //��ʱ������ʼ��
 KEY_Init();     //������ʼ��
 Lcd_Clear(RED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		// TIM1->CCR1 = 500;
   Vsum = First_order_lag_filtering(0.01, aADCxConvertedData[0]);
   Vsum = Vsum * 3.3 / 4095.0;
   Isum = (Vsum - 2.36) / 0.28; /*Isum*/ // 0.28

   Vsum2 = First_order_lag_filtering1(0.01, aADCxConvertedData[3]);
   Vsum2 = Vsum2 * 3.3 / 4095.0;
   Isum2 = (Vsum2 - 2.36) / 0.33; /*Isum2*/ //	0.33

   //				WANT=Isum*10.9256 ;		/*Ŀ���������?*/
   Vall = First_order_lag_filtering2(0.01, aADCxConvertedData[1]); /*V*/
   Vall = (Vall * 3.3 / 4095.0) * 19.71;                           //	DCDC��ʱ���?25.36//20.24

   I_NOW = Isum2 + Isum;

   KEY = KEY_Scan(0);
   if (KEY == 4)
   {
     Mode = !Mode;
   }
   if (Mode == 1)
     WANT = I_NOW / 3.0;
   else
     WANT = I_NOW / 2.0;

   Out_Duty = PID_Error_Deal(Vall, 600, 300, Out_Duty, 10.0, &PID_Init); /*��ѹ��*/
   TIM1->CCR1 = Out_Duty;
    
//    Usart1_Printf("%.3f,%d,%f,%f,%f,%f,%f,%f\r\n", Vall, Out_Duty,num3[0],num3[1],num3[2],PID_Init.P,PID_Init.I,PID_Init.D);
    // memset(datatemp2, 0, sizeof(datatemp2));
    //				Out_Duty = PID_Error_Deal(Isum2,600,300,Out_Duty,WANT);
    //        TIM1->CCR1 = Out_Duty;
    //				printf("%6.3f,%8.3f,%6.3f,%8.3f,%6.3f,%d,%6.3f\r\n",Vsum,Isum,Vsum2,Isum2,Vall,Out_Duty,WANT,&PID_Init);
    //        if(Flag_LCD==1)
    //        {
    // //					memset(datatemp2,1,sizeof(datatemp2));
    // ////		LCD_DISPLAY();
    //            Flag_LCD=0;
    //        }

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//     static uint16_t time=0;
//     if (htim->Instance == TIM2)
//     {
//         time++;
//
//         if(time>=100)
//         {
//
//             Flag_LCD=1;
//             time=0;
//         }

//    }

//}
void LCD_DISPLAY(void)
{
  //				printf("%.3f,%d,%.3f\r\n",VL,Out_Duty,R_NOW);

  sprintf(datatemp1[2], "Duty:%d", Out_Duty);
  sprintf(datatemp1[3], "VH:%6.3f", Vsum);
  sprintf(datatemp1[4], "IH:%6.3f", Isum);
  sprintf(datatemp1[5], "VO:%6.3f", VO);
  sprintf(datatemp1[6], "IO:%6.3f", IO);
  sprintf(datatemp1[7], "Vall:%6.3f", Vall);
  sprintf(datatemp1[8], "RN:%6.3f", R_NOW);
  //    sprintf(datatemp1[9],"PN:%6.3f,PL:%6.3f",P_NOW,P_LAST);

  Show_Str(0, 12, BLUE, GREEN, datatemp1[2], 12, 1);
  Show_Str(0, 24, BLUE, GREEN, datatemp1[3], 12, 1);
  Show_Str(0, 36, BLUE, GREEN, datatemp1[4], 12, 1);
  Show_Str(0, 48, BLUE, GREEN, datatemp1[5], 12, 1);
  Show_Str(0, 60, BLUE, GREEN, datatemp1[6], 12, 1);
  Show_Str(0, 72, BLUE, GREEN, datatemp1[7], 12, 1);
  Show_Str(0, 84, BLUE, GREEN, datatemp1[8], 12, 1);
  Show_Str(0, 96, BLUE, GREEN, datatemp1[9], 12, 1);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
