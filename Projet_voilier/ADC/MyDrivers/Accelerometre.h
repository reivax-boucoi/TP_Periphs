#ifndef ACCELEROMETRE_H
#define ACCELEROMETRE_H

#include "stm32f103xb.h" 
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

void ADCConf(ADC_TypeDef * Conv);

void RecupADC(void);

void GPIOConf (void) ;

void ADC_IT_Enable(ADC_TypeDef * Conv);

void ADC_IT_Conf(ADC_TypeDef * Conv, void (*IT_function), int Prio);

void StartConversion (ADC_TypeDef * Conv) ;
#endif

