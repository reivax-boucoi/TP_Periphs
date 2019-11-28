
#ifndef REMOTE_H
#define REMOTE_H


#include "stm32f103xb.h" 
#include "Moteur.h"

#define PinRemoteRx LL_GPIO_PIN_7
#define Remote_Pin GPIOB

#define TIM_PSC 23
#define TIM_ARR 59999
void Remote_init(void);






#endif

