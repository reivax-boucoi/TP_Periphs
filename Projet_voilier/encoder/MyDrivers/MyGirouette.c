#include "MyGirouette.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_exti.h"


void INITIALISATIONDELAGIROUETTE(void){
//Démarrer clock GPIOA
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);	
//Configurer les broches entrée sortie
/*	INDEX input floating
	CHA CHB alternate fonction*/
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_5,LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_6,LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_7,LL_GPIO_MODE_FLOATING);
//Démarrer la clock TIM3
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
//Configurer TIM3
	LL_TIM_InitTypeDef My_LL_Tim_Init_Struct;
	
	My_LL_Tim_Init_Struct.Autoreload=1440;
	My_LL_Tim_Init_Struct.Prescaler=0;
	My_LL_Tim_Init_Struct.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;
	My_LL_Tim_Init_Struct.CounterMode=LL_TIM_COUNTERMODE_UP;
	My_LL_Tim_Init_Struct.RepetitionCounter=0;
	
	LL_TIM_Init(TIM3,&My_LL_Tim_Init_Struct);
//Configurer le mode encodeur
	LL_TIM_ENCODER_InitTypeDef Enc;
	
	Enc.EncoderMode=LL_TIM_ENCODERMODE_X4_TI12;
	Enc.IC1Polarity=LL_TIM_IC_POLARITY_RISING;
	Enc.IC1ActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	Enc.IC1Prescaler=LL_TIM_ICPSC_DIV1;
	Enc.IC1Filter=LL_TIM_IC_FILTER_FDIV1;
	Enc.IC2Polarity=LL_TIM_IC_POLARITY_RISING;
	Enc.IC2ActiveInput=LL_TIM_ACTIVEINPUT_DIRECTTI;
	Enc.IC2Prescaler=LL_TIM_ICPSC_DIV1;
	Enc.IC2Filter=LL_TIM_IC_FILTER_FDIV1;
	
	LL_TIM_ENCODER_Init(TIM3,&Enc);
//Valeur ARR jusqu'ou l'on compte:
//	LL_TIM_WriteReg(TIM3,ARR,0x1000);
 

//Démarrer la clock AFIO
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
//Configuration AFIO
	LL_GPIO_AF_ConfigEventout(LL_GPIO_AF_EVENTOUT_PORT_A,LL_GPIO_AF_EVENTOUT_PIN_5);
	LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTA,LL_GPIO_AF_EXTI_LINE5);
	LL_GPIO_AF_EnableEventout();
	
//Configuration d'EXTI	
	LL_EXTI_InitTypeDef Exti;
	
	Exti.Line_0_31=LL_EXTI_LINE_5;
	Exti.LineCommand=ENABLE;
	Exti.Mode=LL_EXTI_MODE_IT_EVENT;
	Exti.Trigger=LL_EXTI_TRIGGER_RISING_FALLING;
	
	LL_EXTI_Init(&Exti);
	
//accepter l'interruption front montant de INDEX
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_5);	
	NVIC_SetPriority(EXTI9_5_IRQn, 4);
	NVIC_EnableIRQ(EXTI9_5_IRQn);


//Activer le compteur	
	LL_TIM_EnableCounter(TIM3);
}

void EXTI9_5_IRQHandler(){
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_5);
	LL_TIM_SetCounter(TIM3,0);
}
