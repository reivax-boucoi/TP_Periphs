#ifndef RTC_H
#define RTC_H

#include "stm32f103xb.h" 
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "MyUart.h"

void RTC_init(void);

void RTC_printTime(void);

#endif
