

#include "Accelerometre.h"


void (*pFnc) (void);

void ADCConf(ADC_TypeDef * Conv)
{
/*LL_ADC_InitTypeDef * Aconf;
	
	
Aconf->DataAlignment=LL_ADC_DATA_ALIGN_RIGHT ;
Aconf->SequencersScanMode=LL_ADC_SEQ_SCAN_ENABLE ;

LL_ADC_REG_InitTypeDef * Aregconf;
Aregconf->TriggerSource=LL_ADC_REG_TRIG_SOFTWARE ; //pour le moment réglé en interne pour tester en simulation 
Aregconf->SequencerLength=LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS ; // car conversion sur 2 canaux pour le moment
Aregconf->SequencerDiscont=LL_ADC_REG_SEQ_DISCONT_DISABLE ;
Aregconf->ContinuousMode=LL_ADC_REG_CONV_CONTINUOUS ; //Conversion unique pour le test 
Aregconf->DMATransfer=LL_ADC_REG_DMA_TRANSFER_NONE ;
	*/
	
	LL_ADC_InitTypeDef Aconf	;
	LL_ADC_StructInit(&Aconf);

	LL_ADC_Init(Conv,&Aconf);	

	LL_ADC_REG_InitTypeDef Aregconf ;
  LL_ADC_REG_StructInit(&Aregconf);
	
	Aregconf.TriggerSource=LL_ADC_REG_TRIG_SOFTWARE;
	Aregconf.ContinuousMode=LL_ADC_REG_CONV_SINGLE;
	LL_ADC_REG_Init(Conv,&Aregconf);
	
	
LL_ADC_REG_SetSequencerRanks(Conv,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_10);
	
/*LL_ADC_REG_Init(Conv,Aregconf);
LL_ADC_REG_SetSequencerRanks(Conv,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_10);
LL_ADC_REG_SetSequencerRanks(Conv,LL_ADC_REG_RANK_2,LL_ADC_CHANNEL_11);
*/

}

void RecupADC (void)
{
	volatile int Resultat;
  Resultat =LL_ADC_REG_ReadConversionData32(ADC1);
}

void StartConversion (ADC_TypeDef * Conv) 
{
LL_ADC_REG_StartConversionSWStart(Conv);

}

void GPIOConf () 
{

	
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_0,LL_GPIO_MODE_ANALOG);
	//LL_GPIO_SetPinPull(GPIOC,LL_GPIO_PIN_0,LL_GPIO_PULL_UP);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_1,LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(GPIOC,LL_GPIO_PIN_1,LL_GPIO_PULL_UP);



}

void ADC_IT_Enable(ADC_TypeDef * Conv)
{
LL_ADC_EnableIT_EOS(Conv);

}

void ADC_IT_Conf(ADC_TypeDef * Conv, void (*IT_function), int Prio)
{
pFnc=&(RecupADC);
NVIC->ISER[0]|=(0x01<<18);
NVIC->IP[18]|=(Prio<<4);

}

void ADC1_2_IRQHandler() 
{
 LL_ADC_ClearFlag_EOS(ADC1);
 (*pFnc)();

}


/* Pour la prochaine fois, il faudra améliorer le programme pour qu'on puisse choisir lorsque
l'on lance la conversion si on veut récupérer donnée canal 10 ou donnée (autre canal à chercher)
Pour ce faire, je pourrai rajouter argument pour choix de canal. */

