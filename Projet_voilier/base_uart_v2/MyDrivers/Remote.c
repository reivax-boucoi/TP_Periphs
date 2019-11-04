
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "Remote.h"


void Remote_init(void){

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	
	LL_GPIO_InitTypeDef remote_rx;
	remote_rx.Pin=PinRemoteRx;
	remote_rx.Mode=LL_GPIO_MODE_FLOATING;
	remote_rx.Pull=LL_GPIO_PULL_DOWN;
	remote_rx.Speed=LL_GPIO_SPEED_FREQ_LOW;
	remote_rx.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	LL_GPIO_Init(Remote_Pin, &remote_rx);
	
	LL_TIM_InitTypeDef timer;
	timer.Autoreload=9999;
	timer.Prescaler=143;
	timer.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;	
	LL_TIM_Init(TIM4,&timer);
	

	
//Activation du front montant
	
	LL_TIM_IC_InitTypeDef myTimIC;
	
	myTimIC.ICPolarity=LL_TIM_IC_POLARITY_RISING;
	myTimIC.ICFilter=LL_TIM_IC_FILTER_FDIV1;
	myTimIC.ICActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	myTimIC.ICPrescaler=LL_TIM_ICPSC_DIV4;
	
	LL_TIM_IC_Init(TIM4,LL_TIM_CHANNEL_CH1,&myTimIC);
	
	
//Activation du front descendant
	
	LL_TIM_IC_InitTypeDef myTimIC2;	

	myTimIC2.ICPolarity=LL_TIM_IC_POLARITY_FALLING;
	myTimIC2.ICFilter=LL_TIM_IC_FILTER_FDIV1;
	myTimIC2.ICActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	myTimIC2.ICPrescaler=LL_TIM_ICPSC_DIV4;
	
	LL_TIM_IC_Init(TIM4,LL_TIM_CHANNEL_CH1,&myTimIC2);
	
	TIM4->SMCR|=TIM_SMCR_TS_0|TIM_SMCR_TS_2;
	LL_TIM_SetSlaveMode(TIM4,LL_TIM_SLAVEMODE_RESET);
	
	LL_TIM_EnableCounter(TIM4);
	
}


/*

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

  // Select the TIM2 Input Trigger: TI2FP2 
  TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);

  // Select the slave Mode: Reset Mode 
  TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);

  // Enable the Master/Slave Mode 
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);

  // TIM enable counter
  TIM_Cmd(TIM2, ENABLE);

  // Enable the CC2 Interrupt Request 
  TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
  
  void TIM2_IRQHandler(void)
{
  // Clear TIM2 Capture compare interrupt pending bit 
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

  // Get the Input Capture value 
  IC2Value = TIM_GetCapture2(TIM2);

  if (IC2Value != 0)
  {
    // Duty cycle computation 
    DutyCycle = (TIM_GetCapture1(TIM2) * 100) / IC2Value;

    // Frequency computation 
    Frequency = 72000000 / IC2Value;
  }
  else
  {
    DutyCycle = 0;
    Frequency = 0;
  }
}
*/
