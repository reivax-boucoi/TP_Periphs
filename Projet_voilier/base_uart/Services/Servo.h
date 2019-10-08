#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 

//initialisation du timer(4) en Output PWM
void ServoPWM_Conf(TIM_TypeDef * Timer, uint32_t Channel);

//loi de cmd du servo moteur
void CommandeServo(TIM_TypeDef * Timer, int alpha);

//void MyITFunction(void);


#endif