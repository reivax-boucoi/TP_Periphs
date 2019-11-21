#include "Accelerometre.h"
#include "MyUart.h"
#include "Math.h"

volatile int x=-1;
volatile int batteryVoltage=0;
volatile int y=-1;
volatile int var=-1;
enum meas_type{X_measurement,Y_Measurement,batt_Measurement};
enum meas_type currentMeasurement;

void (*pFnc) (void);

int getX(void){
	
	ADCConf(ADC1,LL_ADC_CHANNEL_10);
	currentMeasurement=X_measurement;
	StartConversion (ADC1);
	//currentMeasurement=X_measurement;
	return x;
}
int getBatteryVoltage(void){
	
	ADCConf(ADC1,LL_ADC_CHANNEL_12);
	currentMeasurement=batt_Measurement;
	StartConversion (ADC1);
	//currentMeasurement=batt_Measurement;
	return batteryVoltage;
}

int getY(void){
	ADCConf(ADC1,LL_ADC_CHANNEL_11);
	currentMeasurement=Y_Measurement;
	StartConversion(ADC1);
	//currentMeasurement=Y_Measurement;
	return y;
	
}
void ADCConf(ADC_TypeDef * Conv, uint32_t Channel)
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
	
	Aregconf.TriggerSource=LL_ADC_REG_TRIG_SOFTWARE ;
	Aregconf.ContinuousMode=LL_ADC_REG_CONV_SINGLE;
	LL_ADC_REG_Init(Conv,&Aregconf);
	
	
LL_ADC_REG_SetSequencerRanks(Conv,LL_ADC_REG_RANK_1,Channel);
LL_ADC_REG_SetTriggerSource(ADC1,LL_ADC_REG_TRIG_SOFTWARE);
/*LL_ADC_REG_Init(Conv,Aregconf);
LL_ADC_REG_SetSequencerRanks(Conv,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_10);
LL_ADC_REG_SetSequencerRanks(Conv,LL_ADC_REG_RANK_2,LL_ADC_CHANNEL_11);
*/

}

void RecupADC (void)
{
	volatile int Resultat;
  Resultat =LL_ADC_REG_ReadConversionData32(ADC1);
	if(currentMeasurement==X_measurement)
		x=Resultat;
	else if (currentMeasurement==Y_Measurement)
		y=Resultat;
	else 
		batteryVoltage=Resultat;
	
	

	
}

void StartConversion (ADC_TypeDef * Conv) 
{
  LL_ADC_REG_StartConversionSWStart(Conv);
	

}

void GPIOConf () 
{

	
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_0,LL_GPIO_MODE_ANALOG);
	//LL_GPIO_SetPinPull(GPIOC,LL_GPIO_PIN_0,LL_GPIO_PULL_UP);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_1,LL_GPIO_MODE_ANALOG);
	//LL_GPIO_SetPinPull(GPIOC,LL_GPIO_PIN_1,LL_GPIO_PULL_UP);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_2,LL_GPIO_MODE_ANALOG);
	
	



}

/*void ADC_IT_Enable(ADC_TypeDef * Conv)
{
LL_ADC_EnableIT_EOS(Conv);

}
*/

/*void ADC_IT_Conf(ADC_TypeDef * Conv, void (*IT_function), int Prio)
{
pFnc=&(RecupADC);
NVIC->ISER[0]|=(0x01<<18);
NVIC->IP[18]|=(Prio<<4);

}
*/

/*void ADC1_2_IRQHandler() 
{
 LL_ADC_ClearFlag_EOS(ADC1);
 (*pFnc)();

}
*/

/* Pour la prochaine fois, il faudra améliorer le programme pour qu'on puisse choisir lorsque
l'on lance la conversion si on veut récupérer donnée canal 10 ou donnée (autre canal à chercher)
Pour ce faire, je pourrai rajouter argument pour choix de canal. */

 void ADCInit(){
	
   GPIOConf();
	 LL_ADC_Enable(ADC1);

	 //ADC_IT_Conf(ADC1, RecupADC,8);
	 //ADC_IT_Enable(ADC1);
	 //getX();
	 //while(x==-1);
	 //getY();
	 //while (y==-1);
	 //getBatteryVoltage();
	 

 }
 
 void AllConversion () {
	 getX();
	 while(LL_ADC_IsActiveFlag_EOS(ADC1)==0);
	 LL_ADC_ClearFlag_EOS(ADC1);
	 RecupADC();
	 getY();
	 while(LL_ADC_IsActiveFlag_EOS(ADC1)==0);
	 LL_ADC_ClearFlag_EOS(ADC1);
	 RecupADC();
	 getBatteryVoltage();
	 while(LL_ADC_IsActiveFlag_EOS(ADC1)==0);
	 LL_ADC_ClearFlag_EOS(ADC1);
	 RecupADC();
	
 }

 
void Test_critique(){
	float aux = sqrt((X_measurement*X_measurement+1)/(Y_Measurement*Y_Measurement+1));
	if(aux>1){
		MyUart_sendString("Alerte tangage !");
		MyUart_sendNumber((int)(aux*45),0);
	}
	if(batt_Measurement*13 < 12.6){
		MyUart_sendString("Alerte Batterie !! !!");
		MyUart_sendNumber(batt_Measurement*13/15,0);
	}
}
