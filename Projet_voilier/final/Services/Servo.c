#include "Servo.h"
#include "MyUart.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h" 

//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	//	

void ServoPWM_Conf(TIM_TypeDef * Timer, uint32_t Channel){
	
	//configurationo initiale du Timer
	LL_TIM_InitTypeDef My_LL_Tim_Init_Struct;	// D�claration d'une variable de temps de type LL_TIM_InitTypeDef comporant 5 champs de donn�es
	// Validation horloge locale en fonction du Timer choisi parmi les 4
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	// chargement structure Arr, Psc, Up Count
	// Initialisation des diff�rents champs de la variable My_LL_Tim_Init_Struct
	// Comme on travaille sur 16 bits, on peut coder jusqu'� 65 536. Cependant, on cherche � avoir une p�riode de 20 ms 
	// comme c'est indiqu� dans le cahier des charges. Donc pour respecter cette condition, on a choisi les valeurs suivantes :
	My_LL_Tim_Init_Struct.Autoreload=59999; // ARR = 59 999
	My_LL_Tim_Init_Struct.Prescaler=23; // PSC = 23
	My_LL_Tim_Init_Struct.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1; // DIV1 c'est le facteur de division propre (vaut 1) pour que �a reste identique 
	My_LL_Tim_Init_Struct.CounterMode=LL_TIM_COUNTERMODE_UP; // Pour dire qu'on est entrain de compter et pas d�compter
	My_LL_Tim_Init_Struct.RepetitionCounter=0;	// Comme on est entrain de compter, � chaque d�passement de valeur maximal du compteur, on revient � 0
	LL_TIM_Init(Timer,&My_LL_Tim_Init_Struct); // permission d'initialiser tous les champs de notre timer avec possibilit� d'avoir comme valeur retourn�e une erreur.
	// Blocage IT
	LL_TIM_DisableIT_UPDATE(Timer);	// D�sactivation de la mise � jour de l'interruption 
	// Blocage Timer
	LL_TIM_DisableCounter(Timer); // D�sactivation du compteur
	
	//configuration initiale du mode PWM (commande PWM) 
	
	LL_TIM_OC_InitTypeDef MyTimOC; // D�claration d'une variable de temps de type LL_TIM_OC_InitTypeDef comporant 8 champs de donn�es
	
	MyTimOC.OCMode = LL_TIM_OCMODE_PWM1; // entrain de compter, donc le canal est actif tant que la valeur compt�e est inf�rieure � celle qui est stock�e dans le registre Capture/Compare registers
	MyTimOC.OCState = LL_TIM_OCSTATE_ENABLE; // le signal de configuration de sortie est actif 
	MyTimOC.OCNState = LL_TIM_OCSTATE_DISABLE; // le compl�mentaire du signal de configuration de sortie est d�sactiv�
	MyTimOC.CompareValue = 3000; //init angle de voile � 0 (3000 est la valeur de comparaison qu'on stocke dans le registre CCR
	MyTimOC.OCPolarity = LL_TIM_OCPOLARITY_HIGH; // niveau haut pour la polarit� de la configuation de sortie 
	MyTimOC.OCNPolarity = LL_TIM_OCPOLARITY_LOW; // niveau bas pour celle qui lui est compl�mentaire
	MyTimOC.OCIdleState = LL_TIM_OCIDLESTATE_HIGH; // indication sur l'�tat de la broche de l'OC du timer apr�s l'�tat de repos
	MyTimOC.OCNIdleState = LL_TIM_OCIDLESTATE_LOW; // indication sur son �tat compl�mentaire
	
	LL_TIM_OC_Init(Timer, Channel, &MyTimOC);
	
	//configuration de la broche de sortie sur GPIOA.8

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	LL_GPIO_SetPinMode(Servo_Port,Servo_Pin,LL_GPIO_MODE_ALTERNATE);  // Comme on travaille une fr�quence de 40 Mhz 
	//(Output MODE est MAX : 50 Mhz donc MODE : 11 ==> Alternate function output)
	
	
	LL_TIM_EnableAutomaticOutput(Timer);
	
	//configuration de l'interruption globale

	// Blocage IT (il faudra la d�bloquer voir fct suivante)
	LL_TIM_DisableIT_UPDATE(Timer);
	
	// validation du canal NVIC
	// g�rer les interruptions de tous les timers
	IRQn_Type TIM_irq;
	
	if (Timer==TIM1) TIM_irq=TIM1_UP_IRQn;
	else if (Timer==TIM2)	TIM_irq=TIM2_IRQn;
	else if (Timer==TIM3)	TIM_irq=TIM3_IRQn;
	else 	TIM_irq=TIM4_IRQn;
	
	NVIC_SetPriority(TIM_irq, 0x40); // position des bits : � partir de 25 donc priority = 0x40
	NVIC_EnableIRQ(TIM_irq);
		
	//activation du compteur
	LL_TIM_EnableCounter(Timer);
	
	LL_TIM_EnableIT_UPDATE(Timer); // activation de la mise � jour de l'interruption
}


void CommandeServo(TIM_TypeDef * Timer, int alpha){ // alpha : angle de girouette
	int theta;
	if((alpha>45) && (alpha<=180)){
		theta = 2*alpha/3 - 30; //angle du vent entre 45� et 180� o� angle voile = 90� pour alpha180�
	}else if ((alpha>180) && (alpha<315)){ 
		theta = -2*alpha/3 + 210; //alpha entre 180� et 315�
	}else{	
		theta = 0;
	}
	setTheta(Timer, theta);
}


void setTheta(TIM_TypeDef * Timer, int theta){
	theta = 100*theta/3 + 3000; //theta ~ tempsPWM
	LL_TIM_OC_SetCompareCH1(Timer,theta);	//modification du CCR du timer	
}
