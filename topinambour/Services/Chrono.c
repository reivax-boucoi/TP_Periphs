// A COMPLETER

/*
Service permettant de chornom�trer jusqu'� 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 � TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "MyUart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_bus.h"

//flag d'envoi des donees en uart
int FLAG_sendTime=0;

// variable priv�e de type Time qui m�morise la dur�e mesur�e
static Time Chrono_Time; // rem : static rend la visibilit� de la variable Chrono_Time limit�e � ce fichier 

// variable priv�e qui m�morise pour le module le timer utilis� par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par d�faut au cas o� l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// variable priv�e qui m�morise pour le module l'uart utilis� par le module
static USART_TypeDef * Chrono_Usart=USART2; // init par d�faut au cas o� l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.


static int oldStartState, oldStopState;

// d�claration callback appel� toute les 10ms
void Chrono_Task_10ms(void);



void Chrono_Conf_io(void){
	
}

/**
	* @brief  Configure le chronom�tre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void Chrono_Conf(TIM_TypeDef * Timer, USART_TypeDef *Usart)
{
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	// Fixation du Timer
	Chrono_Timer=Timer;
	
	// Fixation du Usart
	Chrono_Usart=Usart;

	// R�glage Timer pour un d�bordement � 10ms
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	// R�glage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,30);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
		
	Chrono_Conf_IO();
	
	MyUart_Conf(Chrono_Usart,9600);
	
}

void Chrono_Conf_IO(void){
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	
	LL_GPIO_InitTypeDef start;
	start.Pin=PinStart;
	start.Mode=LL_GPIO_MODE_FLOATING;
	start.Pull=LL_GPIO_PULL_UP; //valeur prise arbitrairement car ne nous sert pas
	LL_GPIO_Init(GPIO_Pin, &start);
	
	LL_GPIO_InitTypeDef stop;
	stop.Pin=PinStop;
	stop.Mode=LL_GPIO_MODE_INPUT;
	stop.Pull=LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(GPIO_Pin, &stop);
	
	LL_GPIO_InitTypeDef led;
	led.Pin=PinLED;
	led.Mode=LL_GPIO_MODE_OUTPUT;
	led.Speed=LL_GPIO_SPEED_FREQ_LOW; //2MHz
	led.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	led.Pull=LL_GPIO_PULL_DOWN;	//valeur prise arbitrairement car ne nous sert pas
	LL_GPIO_Init(GPIO_LED, &led);
	//Valeurs par d�faut des boutons Start and Stop
	oldStartState = 0;
	oldStopState = 1;
}



void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}

void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronom�tre � 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arr�t Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time priv�e g�r�e dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}




/**
	* @brief  incr�mente la variable priv�e Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		LL_GPIO_TogglePin(GPIO_LED,PinLED);
		FLAG_sendTime=1;
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
}

void Chrono_Background(void){
	
	
	if(LL_GPIO_IsInputPinSet(GPIO_Pin,PinStart) != oldStartState){
		if(oldStartState==0){
			Chrono_Start();
		}
		oldStartState=1-oldStartState;
	}
	
	if(LL_GPIO_IsInputPinSet(GPIO_Pin,PinStop) != oldStopState){
		if(oldStopState==1){
			Chrono_Stop();
		}
		oldStopState=1-oldStopState;
	}
			
}


void Chrono_Send(void){
	if(FLAG_sendTime){
		sendTime(Chrono_Usart,Chrono_Time);
		FLAG_sendTime=0;
	}
}
