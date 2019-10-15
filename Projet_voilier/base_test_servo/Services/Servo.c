#include "Servo.h"
#include "MyTimer.h"
#include "MyUart.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h" 

//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	

void ServoPWM_Conf(TIM_TypeDef * Timer, uint32_t Channel){
	
	LL_TIM_OC_InitTypeDef MyTimOC;
	
	//configurationo initiale du Timer
	MyTimer_Conf(Timer, 60000-1, 24-1);
	
	//configuration initiale du mode PWM
	MyTimOC.OCMode = LL_TIM_OCMODE_PWM1;
	MyTimOC.OCState = LL_TIM_OCSTATE_ENABLE;
	MyTimOC.OCNState = LL_TIM_OCSTATE_DISABLE;
	MyTimOC.CompareValue = 3000; //angle de voile à 0
	MyTimOC.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	MyTimOC.OCNPolarity = LL_TIM_OCPOLARITY_LOW;
	MyTimOC.OCIdleState = LL_TIM_OCPOLARITY_LOW;
	MyTimOC.OCNIdleState = LL_TIM_OCPOLARITY_HIGH;
	
	LL_TIM_OC_Init(Timer, Channel, &MyTimOC);
	
	//configuration de la broche de sortie sur GPIOB.8
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
	
	/*
	//configuration de l'interruption globale
	MyTimer_IT_Conf(Timer, MyITFunction, 0x25);
	MyTimer_IT_Enable(Timer);
	*/
	
	//activation du compteur
	LL_TIM_EnableCounter(Timer);
}


void CommandeServo(TIM_TypeDef * Timer, int alpha){
	int theta;
	if((alpha<45) && (alpha>180)){
		theta = 0;
	}else{
		theta = 2*alpha/3 - 30;
	}
	/*
	theta = 100*theta/3 + 3000; //theta ~ tempsPWM
	LL_TIM_OC_SetCompareCH3(Timer,theta);	//modification du CCR du timer
	*/
	setTheta(theta);
}

void setTheta(int theta){
	
}
/*
void MyITFunction(void){
	int a;
}
*/