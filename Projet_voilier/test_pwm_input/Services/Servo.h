#ifndef SERVO_H
#define SERVO_H

#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 



#define Servo_Pin LL_GPIO_PIN_8
#define Servo_Port GPIOA

//initialisation du timer(4) en Output PWM
void ServoPWM_Conf(TIM_TypeDef * Timer, uint32_t Channel);

//loi de cmd du servo moteur
void CommandeServo(TIM_TypeDef * Timer, int alpha);

//theta en PWM
void setTheta(TIM_TypeDef * Timer, int theta);


#endif

