/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
					pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "MyTimer.h"
#include "stm32f103xb.h" 

void (* IRQ_TIM1_callback) (void);
void (* IRQ_TIM2_callback) (void);
void (* IRQ_TIM3_callback) (void);
void (* IRQ_TIM4_callback) (void);

	void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc){
		
		//TODO adapter en fonction de Timer
		if(Timer==TIM1){
			RCC->APB2ENR|=RCC_APB2ENR_TIM1EN;
		}else if(Timer==TIM2){
			RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;
		}else if(Timer==TIM3){
			RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;
		}else if(Timer==TIM4){
			RCC->APB1ENR|=RCC_APB1ENR_TIM4EN;
		}
			
		Timer->PSC=Psc;
		Timer->ARR=Arr;
	}
	
	
	void MyTimer_Start(TIM_TypeDef * Timer){
		
		Timer->CR1|=TIM_CR1_CEN;
	}
	
	void MyTimer_Stop(TIM_TypeDef * Timer){
		
		Timer->CR1&=~TIM_CR1_CEN;
		
	}
	
void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio){
	
	
			if(Timer==TIM1){
			NVIC_SetPriority (TIM1_UP_IRQn, Prio);
			NVIC_EnableIRQ(TIM1_UP_IRQn);
			IRQ_TIM1_callback=IT_function;
				
		}else if(Timer==TIM2){
			NVIC_SetPriority (TIM2_IRQn, Prio);
			NVIC_EnableIRQ(TIM2_IRQn);
			IRQ_TIM2_callback=IT_function;
			
		}else if(Timer==TIM3){
			NVIC_SetPriority (TIM3_IRQn, Prio);
			NVIC_EnableIRQ(TIM3_IRQn);
			IRQ_TIM3_callback=IT_function;
			
		}else if(Timer==TIM4){
			NVIC_SetPriority (TIM4_IRQn, Prio);
			NVIC_EnableIRQ(TIM4_IRQn);
			IRQ_TIM4_callback=IT_function;
		}
		
}
void MyTimer_IT_Enable(TIM_TypeDef * Timer){
		Timer->DIER|=TIM_DIER_UIE;
}

void MyTimer_IT_Disable(TIM_TypeDef * Timer){
		Timer->DIER&=~TIM_DIER_UIE;
}



void TIM1_IRQHandler(void){
	if (IRQ_TIM1_callback != 0)(*IRQ_TIM1_callback) ();
	
	TIM1->SR&=~TIM_SR_UIF;
}

void TIM2_IRQHandler(void){
	if (IRQ_TIM2_callback != 0)(*IRQ_TIM2_callback) ();
	
	TIM2->SR&=~TIM_SR_UIF;
}

void TIM3_IRQHandler(void){
	if (IRQ_TIM3_callback != 0)(*IRQ_TIM3_callback) ();
	
	TIM3->SR&=~TIM_SR_UIF;
}

void TIM4_IRQHandler(void){
	if (IRQ_TIM4_callback != 0)(*IRQ_TIM4_callback) ();
	
	TIM4->SR&=~TIM_SR_UIF;
}

