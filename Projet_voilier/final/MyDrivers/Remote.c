
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "Remote.h"
#include "Servo.h"


void Remote_init(void){

	// Enable les horloges des périphériques
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	
	// Activation des pins sur le GPIOB qui seront utilisés 
	LL_GPIO_SetPinMode(Remote_Pin,LL_GPIO_PIN_6,LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(Remote_Pin,LL_GPIO_PIN_7,LL_GPIO_MODE_FLOATING);
	
	// Configuration de TIM4
	LL_TIM_InitTypeDef timer;
	timer.Autoreload=TIM_ARR;
	timer.Prescaler=TIM_PSC;
	timer.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;	
	timer.CounterMode=LL_TIM_COUNTERMODE_UP;
	LL_TIM_Init(TIM4,&timer);

	
//Activation du front descendant pour la capture sur le channel 1
	
	LL_TIM_IC_InitTypeDef myTimIC;
	
	myTimIC.ICPolarity=LL_TIM_IC_POLARITY_FALLING;
	myTimIC.ICFilter=LL_TIM_IC_FILTER_FDIV1;
	myTimIC.ICActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	myTimIC.ICPrescaler=LL_TIM_ICPSC_DIV1;
	
	LL_TIM_IC_Init(TIM4,LL_TIM_CHANNEL_CH1,&myTimIC);
	
	
//Activation du front montant pour la capture sur le channel 2
	
	LL_TIM_IC_InitTypeDef myTimIC2;	

	myTimIC2.ICPolarity=LL_TIM_IC_POLARITY_RISING;
	myTimIC2.ICFilter=LL_TIM_IC_FILTER_FDIV1;
	myTimIC2.ICActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	myTimIC2.ICPrescaler=LL_TIM_ICPSC_DIV1;
	
	LL_TIM_IC_Init(TIM4,LL_TIM_CHANNEL_CH2,&myTimIC2);
	
	//Choix du trigger qui sera utilisé pour synchroniser le compteur (ici (TI1FP1))
	TIM4->SMCR|=TIM_SMCR_TS_0|TIM_SMCR_TS_2;
	LL_TIM_SetSlaveMode(TIM4,LL_TIM_SLAVEMODE_RESET);
	
	LL_TIM_EnableCounter(TIM4);
	
	LL_TIM_EnableIT_CC1(TIM4);
	
	//Configuration du handler du TIM4 avec son degré de priorité
	NVIC_SetPriority(TIM4_IRQn,0x30);
	NVIC_EnableIRQ(TIM4_IRQn);
	
}

