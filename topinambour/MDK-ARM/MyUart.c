
#include "MyUart.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges



void MyUart_Conf(USART_TypeDef * Uart,int baudrate){
	
	if(Uart==USART1){
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	}else if(Uart==USART2){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	}else if(Uart==USART3){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
	}
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	
	LL_GPIO_InitTypeDef uart_tx;
	uart_tx.Pin=PinUartTX;
	uart_tx.Mode=LL_GPIO_MODE_ALTERNATE;
	uart_tx.Pull=LL_GPIO_PULL_DOWN;
	uart_tx.Speed=LL_GPIO_SPEED_FREQ_LOW;
	uart_tx.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	if(Uart==USART1){
	LL_GPIO_AF_EnableRemap_USART1();
	}else if(Uart==USART2){
	LL_GPIO_AF_EnableRemap_USART2();
	}else if(Uart==USART3){
	LL_GPIO_AF_EnableRemap_USART3();
	}
	
	LL_GPIO_Init(USART_Pin, &uart_tx);
	
	LL_USART_ClockInitTypeDef myUartClock;
	myUartClock.ClockOutput=LL_USART_CLOCK_ENABLE;
	myUartClock.ClockPhase=LL_USART_PHASE_1EDGE;
	myUartClock.ClockPolarity=LL_USART_POLARITY_LOW;
	myUartClock.LastBitClockPulse=LL_USART_LASTCLKPULSE_NO_OUTPUT;
	
	LL_USART_ClockInit(Uart,&myUartClock);
	
	
	LL_USART_InitTypeDef myUart;
	myUart.BaudRate=baudrate;
	myUart.DataWidth=LL_USART_DATAWIDTH_8B;
	myUart.StopBits=LL_USART_STOPBITS_1;
	myUart.Parity=LL_USART_PARITY_NONE;
	myUart.TransferDirection=LL_USART_DIRECTION_TX;
	myUart.HardwareFlowControl=LL_USART_HWCONTROL_NONE;
	myUart.OverSampling=LL_USART_OVERSAMPLING_16;
	
	LL_USART_Init(Uart,&myUart);
	
	LL_USART_Enable(Uart);
}


/*
		while(!LL_USART_IsActiveFlag_TXE(USART2));
		
			LL_USART_TransmitData8(USART2,'h');
		
		while(!LL_USART_IsActiveFlag_TXE(USART2));
			LL_USART_TransmitData8(USART2,'\r');
		
		while(!LL_USART_IsActiveFlag_TXE(USART2));
			
			LL_USART_TransmitData8(USART2,'\n');
		*/