#include "RTC.h"



void RTC_init(void){
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);
	
	LL_I2C_InitTypeDef i2c_Init;
	i2c_Init.ClockSpeed=80000;
	i2c_Init.DutyCycle=LL_I2C_DUTYCYCLE_2;
	i2c_Init.OwnAddress1=0x68;//0xD0 ?
	i2c_Init.OwnAddrSize=LL_I2C_OWNADDRESS1_7BIT;
	i2c_Init.PeripheralMode=LL_I2C_MODE_I2C;
	i2c_Init.TypeAcknowledge=LL_I2C_ACK;
	
	LL_I2C_Init(I2C2,&i2c_Init);
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	
	LL_GPIO_InitTypeDef scl;
	scl.Pin=LL_GPIO_PIN_10;
	scl.Mode=LL_GPIO_MODE_ALTERNATE;
	scl.Pull=LL_GPIO_PULL_UP;
	scl.Speed=LL_GPIO_SPEED_FREQ_LOW;
	scl.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	LL_GPIO_Init(GPIOB, &scl);
	
	LL_GPIO_InitTypeDef sda;
	sda.Pin=LL_GPIO_PIN_11;
	sda.Mode=LL_GPIO_MODE_ALTERNATE;
	sda.Pull=LL_GPIO_PULL_UP;
	sda.Speed=LL_GPIO_SPEED_FREQ_LOW;
	sda.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	LL_GPIO_Init(GPIOB, &sda);
	
	LL_I2C_Enable(I2C2);
	
	
}
void RTC_printTime(void){
	int date,tendate,month,tenmonth,year,tenyear,hour,tenhour,min,tenmin,sec,tensec;
	
	MyUart_sendChar(tendate+0x30);
	MyUart_sendChar(date+0x30);
	MyUart_sendChar('/');
	MyUart_sendChar(tenmonth+0x30);
	MyUart_sendChar(month+0x30);
	MyUart_sendChar('/');
	MyUart_sendChar(tenyear+0x30);
	MyUart_sendChar(year+0x30);
	MyUart_sendChar(' ');
	MyUart_sendChar(tenhour+0x30);
	MyUart_sendChar(hour+0x30);
	MyUart_sendChar(';');
	MyUart_sendChar(tenmin+0x30);
	MyUart_sendChar(min+0x30);
	MyUart_sendChar(';');
	MyUart_sendChar(tensec+0x30);
	MyUart_sendChar(sec+0x30);
	MyUart_sendChar('\t');
}
