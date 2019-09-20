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
	
		Timer->DIER|=TIM_DIER_UIE;
		
			if(Timer==TIM1){
			NVIC_EnableIRQ(25);
		}else if(Timer==TIM2){
			NVIC_EnableIRQ();
		}else if(Timer==TIM3){
			NVIC_EnableIRQ(29);
		}else if(Timer==TIM4){
			NVIC_EnableIRQ(30);
		}
}
void MyTimer_IT_Enable(TIM_TypeDef * Timer){
	
}
void MyTimer_IT_Disable(TIM_TypeDef * Timer){
	
}