
#ifndef REMOTE_H
#define REMOTE_H


#include "stm32f103xb.h" 


#define PinRemoteRx LL_GPIO_PIN_6
#define Remote_Pin GPIOB

#define TIM_PSC 143
#define TIM_ARR 9999UL
void Remote_init(void);






#endif

