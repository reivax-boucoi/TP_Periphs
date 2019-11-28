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
	MyTimOC.CompareValue = 3000; //init angle de voile à 0
	MyTimOC.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	MyTimOC.OCNPolarity = LL_TIM_OCPOLARITY_LOW;
	MyTimOC.OCIdleState = LL_TIM_OCIDLESTATE_HIGH;
	MyTimOC.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
	
	LL_TIM_OC_Init(Timer, Channel, &MyTimOC);
	
	//configuration de la broche de sortie sur GPIOA.8

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(Servo_Port,Servo_Pin,LL_GPIO_MODE_ALTERNATE);
	
	LL_TIM_EnableAutomaticOutput(Timer);
	
	//configuration de l'interruption globale

	// Blocage IT (il faudra la débloquer voir fct suivante)
	LL_TIM_DisableIT_UPDATE(Timer);
	
	// validation du canal NVIC
	IRQn_Type TIM_irq;
	
	if (Timer==TIM1) TIM_irq=TIM1_UP_IRQn;
	else if (Timer==TIM2)	TIM_irq=TIM2_IRQn;
	else if (Timer==TIM3)	TIM_irq=TIM3_IRQn;
	else 	TIM_irq=TIM4_IRQn;
	
	NVIC_SetPriority(TIM_irq, 0x40);
	NVIC_EnableIRQ(TIM_irq);

		
	//activation du compteur
	LL_TIM_EnableCounter(Timer);
	
	LL_TIM_EnableIT_UPDATE(Timer);
}


void CommandeServo(TIM_TypeDef * Timer, int alpha){
	int theta;
	if((alpha>45) && (alpha<=180)){
		theta = 2*alpha/3 - 30; //angle du vent entre 45° et 180° où angle voile = 90° pour alpha180°
	}else if ((alpha>180) && (alpha<315)){
		theta = -2*alpha/3 + 210; //alpha entre 180° et 315°
	}else{	
		theta = 0;
	}
	setTheta(Timer, theta);
}


void setTheta(TIM_TypeDef * Timer, int theta){
	theta = 100*theta/3 + 3000; //theta ~ tempsPWM
	LL_TIM_OC_SetCompareCH1(Timer,theta);	//modification du CCR du timer	
}
