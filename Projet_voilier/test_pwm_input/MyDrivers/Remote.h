
#ifndef REMOTE_H
#define REMOTE_H


#include "stm32f103xb.h" 
#include "Moteur.h"

#define PinRemoteRx LL_GPIO_PIN_6
#define Remote_Pin GPIOB

#define TIM_PSC 32
#define TIM_ARR 65455
void Remote_init(void);






#endif

