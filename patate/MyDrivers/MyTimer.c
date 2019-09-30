/*
 indispensable pour pouvoir adresser les registres des p�riph�riques.
 Rem : OBLIGATION d'utiliser les d�finitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilit� du code.

 Pour les masques, utiliser �galement les d�finitions propos�e
 Rappel : pour mettre � 1  , reg = reg | Mask (ou Mask est le repr�sente le ou les bits � positionner � 1)
					pour mettre � 0  , reg = reg&~ Mask (ou Mask est le repr�sente le ou les bits � positionner � 0)
 
*/ 
#include "MyTimer.h"
#include "stm32f103xb.h" 

void (* IRQ_10ms_callback) (void);

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
				
		}else if(Timer==TIM2){
			NVIC_SetPriority (TIM2_IRQn, Prio);
			NVIC_EnableIRQ(TIM2_IRQn);
			
		}else if(Timer==TIM3){
			NVIC_SetPriority (TIM3_IRQn, Prio);
			NVIC_EnableIRQ(TIM3_IRQn);
			
		}else if(Timer==TIM4){
			NVIC_SetPriority (TIM4_IRQn, Prio);
			NVIC_EnableIRQ(TIM4_IRQn);
		}
		IRQ_10ms_callback=IT_function;
		
}
void MyTimer_IT_Enable(TIM_TypeDef * Timer){
		Timer->DIER|=TIM_DIER_UIE;
}

void MyTimer_IT_Disable(TIM_TypeDef * Timer){
		Timer->DIER&=~TIM_DIER_UIE;
}



void TIM1_IRQHandler(void){
	TIM1->SR&=~TIM_SR_UIF;
	if (IRQ_10ms_callback != 0)(*IRQ_10ms_callback) ();
	
}

void TIM2_IRQHandler(void){
	TIM2->SR&=~TIM_SR_UIF;
	if (IRQ_10ms_callback != 0)(*IRQ_10ms_callback) ();
	
}

void TIM3_IRQHandler(void){
	TIM3->SR&=~TIM_SR_UIF;
	if (IRQ_10ms_callback != 0)(*IRQ_10ms_callback) ();
	
}

void TIM4_IRQHandler(void){
	TIM4->SR&=~TIM_SR_UIF;
	if (IRQ_10ms_callback != 0)(*IRQ_10ms_callback) ();
	
}

