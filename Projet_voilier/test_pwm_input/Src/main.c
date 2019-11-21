

/**
  ******************************************************************************
  * @file    Templates_LL/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body through the LL API
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config

#include "MyUart.h"
#include "Servo.h"
#include "Remote.h"
#include "Moteur.h"
#include "RTC.h"
#include "MyGirouette.h"
#include "Accelerometre.h"

void delay(int d){
	for(int i=0;i<d;i++){for(int j=0;j<10000;j++);}
	
}
void  SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
int main(void)
{
	

  SystemClock_Config();
	RTC_init();
	MyUart_Conf(USART1,9600);
	MyUart_sendString("Hello World ! ");
	MyUart_sendNumber(123,0);
	MyUart_sendString("\r\n");
	
	ServoPWM_Conf(TIM1,LL_TIM_CHANNEL_CH1);
	
	Remote_init();
	
	Girouette_init();
	
	MoteurPWM_Conf(TIM2,LL_TIM_CHANNEL_CH2);	
	
  while (1){
		
  }
}


//Commande de la voile en fonction de l'angle du vent
void TIM1_UP_IRQHandler(){
  LL_TIM_ClearFlag_UPDATE(TIM1);
	CommandeServo(TIM1,getGirouetteAngle());
	Test_critique();
}

//Commande du plateau en fonction de la commande-télécommande
void TIM4_IRQHandler(void){
  LL_TIM_ClearFlag_CC1(TIM4);

unsigned int remote_dc;
//unsigned int remote_freq;	

  unsigned int IC1Value = LL_TIM_IC_GetCaptureCH1(TIM4);

  if (IC1Value != 0){
    remote_dc = (LL_TIM_IC_GetCaptureCH2(TIM4) - 0x830)*512/(0xBFF-0x830) -256;	//0.08 à 0.15 -> 0 à 255
    //remote_freq = 72000000 / TIM_PSC / IC1Value;
		CommandeMoteur(TIM2,remote_dc);
  }else{
    remote_dc = 0;
    //remote_freq = 0;
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucléo*********************************
  //LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000); // utile lorsqu'on utilise la fonction LL_mDelay

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}



/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

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
