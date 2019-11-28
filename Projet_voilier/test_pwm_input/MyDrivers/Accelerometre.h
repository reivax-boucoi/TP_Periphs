#ifndef ACCELEROMETRE_H
#define ACCELEROMETRE_H

#include "stm32f103xb.h" 
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
<<<<<<< HEAD
#include "stm32f1xx_ll_rcc.h"
#include "Math.h"
#include "MyUart.h"


//conf adc pour bon channel, lance conv, recupere ancienne valeur de x
void getX(void);

void getBatteryVoltage(void);
=======


//conf adc pour bon channel, lance conv, recupere ancienne valeur de x
int getX(void);

int getBatteryVoltage(void);
>>>>>>> a6c2231785edfd00822180a6dba411316c81a9d7

void ADCConf(ADC_TypeDef * Conv, uint32_t Channel);


void RecupADC(void);

void GPIOConf (void) ;

void ADC_IT_Enable(ADC_TypeDef * Conv);

void ADC_IT_Conf(ADC_TypeDef * Conv, void (*IT_function), int Prio);

void StartConversion (ADC_TypeDef * Conv);
void ADCInit(void);
<<<<<<< HEAD
void getY(void) ;
void AllConversion(void);

int Test_critique(void);
=======
int getY(void) ;
void AllConversion(void);

void Test_critique(void);
>>>>>>> a6c2231785edfd00822180a6dba411316c81a9d7
#endif

