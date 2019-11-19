
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "Remote.h"
#include "Servo.h"


unsigned int remote_dc;
unsigned int remote_freq;


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
	timer.Autoreload=TIM_ARR;
	timer.Prescaler=TIM_PSC;
	timer.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;	
	timer.CounterMode=LL_TIM_COUNTERMODE_UP;
	LL_TIM_Init(TIM4,&timer);
	
// Blocage IT
//	LL_TIM_DisableIT_UPDATE(TIM4);	
	// Blocage Timer
//	LL_TIM_DisableCounter(TIM4);
	
//Activation du front montant
	
	LL_TIM_IC_InitTypeDef myTimIC;
	
	myTimIC.ICPolarity=LL_TIM_IC_POLARITY_FALLING;
	myTimIC.ICFilter=LL_TIM_IC_FILTER_FDIV1;
	myTimIC.ICActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	myTimIC.ICPrescaler=LL_TIM_ICPSC_DIV1;
	
	LL_TIM_IC_Init(TIM4,LL_TIM_CHANNEL_CH2,&myTimIC);
	
	
//Activation du front descendant
	
	LL_TIM_IC_InitTypeDef myTimIC2;	

	myTimIC2.ICPolarity=LL_TIM_IC_POLARITY_RISING;
	myTimIC2.ICFilter=LL_TIM_IC_FILTER_FDIV1;
	myTimIC2.ICActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	myTimIC2.ICPrescaler=LL_TIM_ICPSC_DIV1;
	
	LL_TIM_IC_Init(TIM4,LL_TIM_CHANNEL_CH1,&myTimIC2);
	
	TIM4->SMCR|=TIM_SMCR_TS_0|TIM_SMCR_TS_2;
	LL_TIM_SetSlaveMode(TIM4,LL_TIM_SLAVEMODE_RESET);
	
	LL_TIM_EnableCounter(TIM4);
	
	LL_TIM_EnableIT_CC1(TIM4);
	
	
	NVIC_SetPriority(TIM4_IRQn,0x30);
	NVIC_EnableIRQ(TIM4_IRQn);
	
}

void TIM4_IRQHandler(void){
  LL_TIM_ClearFlag_CC1(TIM4);

  unsigned int IC1Value = LL_TIM_IC_GetCaptureCH1(TIM4);

  if (IC1Value != 0){
    remote_dc = (LL_TIM_IC_GetCaptureCH2(TIM4) * 90) / IC1Value;
    remote_freq = 72000000 / TIM_PSC / IC1Value;
	//	setTheta(TIM1,90);
  }else{
    remote_dc = 0;
    remote_freq = 0;
  }
}

