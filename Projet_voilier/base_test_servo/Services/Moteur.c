#include "Moteur.h"
#include "MyUart.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h" 

//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	

void MoteurPWM_Conf(TIM_TypeDef * Timer, uint32_t Channel){
	
	LL_TIM_OC_InitTypeDef MyTimOC;
	
	//configurationo initiale du Timer
	LL_TIM_InitTypeDef My_LL_Tim_Init_Struct;	
	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	// chargement structure Arr, Psc, Up Count
	My_LL_Tim_Init_Struct.Autoreload=255;
	My_LL_Tim_Init_Struct.Prescaler=5624;
	My_LL_Tim_Init_Struct.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;
	My_LL_Tim_Init_Struct.CounterMode=LL_TIM_COUNTERMODE_UP;
	My_LL_Tim_Init_Struct.RepetitionCounter=0;	
	LL_TIM_Init(Timer,&My_LL_Tim_Init_Struct);
	// Blocage IT
	LL_TIM_DisableIT_UPDATE(Timer);	
	// Blocage Timer
	LL_TIM_DisableCounter(Timer);
	
	//configuration initiale du mode PWM
	MyTimOC.OCMode = LL_TIM_OCMODE_PWM1;
	MyTimOC.OCState = LL_TIM_OCSTATE_ENABLE;
	MyTimOC.OCNState = LL_TIM_OCSTATE_DISABLE;
	MyTimOC.CompareValue = 0;
	MyTimOC.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	MyTimOC.OCNPolarity = LL_TIM_OCPOLARITY_LOW;
	MyTimOC.OCIdleState = LL_TIM_OCPOLARITY_LOW;
	MyTimOC.OCNIdleState = LL_TIM_OCPOLARITY_HIGH;
	
	LL_TIM_OC_Init(Timer, Channel, &MyTimOC);
	
	//configuration de la broche de sortie sur GPIOA
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_1,LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_2,LL_GPIO_MODE_OUTPUT);
	
	
	/*
	//configuration de l'interruption globale
	MyTimer_IT_Conf(Timer, MyITFunction, 0x25);
	MyTimer_IT_Enable(Timer);
	*/
	
	//activation du compteur
	LL_TIM_EnableCounter(Timer);
}