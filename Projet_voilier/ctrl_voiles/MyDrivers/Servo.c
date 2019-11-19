#include "Servo.h"
#include "MyUart.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h" 

//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	

void ServoPWM_Conf(TIM_TypeDef * Timer, uint32_t Channel){
	
	//configurationo initiale du Timer
	LL_TIM_InitTypeDef My_LL_Tim_Init_Struct;	
	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	// chargement structure Arr, Psc, Up Count
	My_LL_Tim_Init_Struct.Autoreload=59999;
	My_LL_Tim_Init_Struct.Prescaler=23;
	My_LL_Tim_Init_Struct.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;
	My_LL_Tim_Init_Struct.CounterMode=LL_TIM_COUNTERMODE_UP;
	My_LL_Tim_Init_Struct.RepetitionCounter=0;	
	LL_TIM_Init(Timer,&My_LL_Tim_Init_Struct);
	// Blocage IT
	LL_TIM_DisableIT_UPDATE(Timer);	
	// Blocage Timer
	LL_TIM_DisableCounter(Timer);
	
	//configuration initiale du mode PWM
	
	LL_TIM_OC_InitTypeDef MyTimOC;
	
	MyTimOC.OCMode = LL_TIM_OCMODE_PWM1;
	MyTimOC.OCState = LL_TIM_OCSTATE_ENABLE;
	MyTimOC.OCNState = LL_TIM_OCSTATE_DISABLE;
	MyTimOC.CompareValue = 3000; //init angle de voile � 0
	MyTimOC.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	MyTimOC.OCNPolarity = LL_TIM_OCPOLARITY_LOW;
	MyTimOC.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
	MyTimOC.OCNIdleState = LL_TIM_OCIDLESTATE_HIGH;
	
	LL_TIM_OC_Init(Timer, Channel, &MyTimOC);
	
	//configuration de la broche de sortie sur GPIOB.8
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA|LL_APB2_GRP1_PERIPH_GPIOB);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8,LL_GPIO_MODE_ALTERNATE);
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
	setTheta(Timer, theta);
}


void setTheta(TIM_TypeDef * Timer, int theta){
	theta = 100*theta/3 + 3000; //theta ~ tempsPWM
	LL_TIM_OC_SetCompareCH1(Timer,theta);	//modification du CCR du timer	
	LL_TIM_OC_SetCompareCH2(Timer,theta);	//modification du CCR du timer	
	LL_TIM_OC_SetCompareCH3(Timer,theta);	//modification du CCR du timer	
	LL_TIM_OC_SetCompareCH4(Timer,theta);	//modification du CCR du timer	
}
