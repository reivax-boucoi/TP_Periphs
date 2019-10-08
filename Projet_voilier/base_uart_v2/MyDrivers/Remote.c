
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "Remote.h"


void Remote_init(void){

	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	
	LL_GPIO_InitTypeDef remote_rx;
	remote_rx.Pin=PinRemoteRx;
	remote_rx.Mode=LL_GPIO_MODE_ALTERNATE;
	remote_rx.Pull=LL_GPIO_PULL_DOWN;
	remote_rx.Speed=LL_GPIO_SPEED_FREQ_LOW;
	remote_rx.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	LL_GPIO_Init(Remote_Pin, &remote_rx);
	
	
}

void Remote_conf(void){

LL_TIM_IC_InitTypeDef myTim;	

//Entrée pur la capture sur front montant 
TIM4->CCR1|=TIM_CCMR1_CC1S_0;
TIM4->CCR1&=~TIM_CCMR1_CC1S_1;
	
//Activation du front montant

	myTim.ICPolarity=LL_TIM_IC_POLARITY_RISING;
	myTim.ICFilter=LL_TIM_IC_FILTER_FDIV1;
	myTim.ICActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	myTim.ICPrescaler=LL_TIM_ICPSC_DIV1;
	

	
}



