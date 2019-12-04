#include "Accelerometre.h"


volatile int x=-1;
volatile int batteryVoltage=0;
volatile int y=-1;
volatile int var=-1;
enum meas_type{X_measurement,Y_Measurement,batt_Measurement};
enum meas_type currentMeasurement;

void (*pFnc) (void);

// Obtenir la position en x après conversion par l'ADC
void getX(void){
	
	ADCConf(ADC1,LL_ADC_CHANNEL_10);
	currentMeasurement=X_measurement;
	StartConversion (ADC1);
}

// Obtenir la position de la batterie après conversion par l'ADC
void getBatteryVoltage(void){
	
	ADCConf(ADC1,LL_ADC_CHANNEL_12);
	currentMeasurement=batt_Measurement;
	StartConversion (ADC1);
}

// Obtenir la position en y après conversion par l'ADC
void getY(void){
	ADCConf(ADC1,LL_ADC_CHANNEL_11);
	currentMeasurement=Y_Measurement;
	StartConversion(ADC1);
	
}

// Configuration de l'ADC
void ADCConf(ADC_TypeDef * Conv, uint32_t Channel)
{
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
}

// Récupération de la valeur renvoyée par l'ADC
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

void GPIOConf () {
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_0,LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_1,LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_2,LL_GPIO_MODE_ANALOG);
}


 void ADCInit(){
	
		RCC->APB2ENR |=RCC_APB2ENR_ADC1EN;
		RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;
		LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);
   GPIOConf();
	 LL_ADC_Enable(ADC1);

 }
 
 
 // Permettre la conversion des paramètres x, y et le niveau de batterie
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

int Test_critique(){
	 int res=0;
		float aux_x = ((x-1500)<0) ? 1500-x : x-1500;
	 if(aux_x>(y-1700)){
		 MyUart_sendString("Alerte tangage !");
		 res=1;
	 }
	 if(batteryVoltage < 0x456){ //magic number
		MyUart_sendString("Alerte Batterie !! Tension : ");
		MyUart_sendNumber(batteryVoltage/88,0);
		MyUart_sendString("V\r\n");
		 res=2;
	}
	 return res;
}
