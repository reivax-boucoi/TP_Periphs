
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
	remote_rx.Mode=LL_GPIO_MODE_ALTERNATE;
	remote_rx.Pull=LL_GPIO_PULL_DOWN;
	remote_rx.Speed=LL_GPIO_SPEED_FREQ_LOW;
	remote_rx.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	LL_GPIO_Init(Remote_Pin, &remote_rx);
	
LL_TIM_InitTypeDef timer;
	
	LL_TIM_Init(TIM4,&timer);
	
	LL_TIM_ENCODER_InitTypeDef encoder;
	encoder.EncoderMode=LL_TIM_ENCODERMODE_X2_TI1;
//	encoder.IC1ActiveInput=LL_TIM_
	/*
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
	
	LL_TIM_IC_Init(TIM4,LL_TIM_CHANNEL_CH2,&myTimIC2);
	*/
}



