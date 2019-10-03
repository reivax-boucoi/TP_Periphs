
#ifndef UART_H
#define UART_H

#include "stm32f103xb.h" 


#define PinUartTX LL_GPIO_PIN_2
#define USART_Pin GPIOA

#define BUFFER_SIZE 64
#define BUFFER_MASK 63

struct Buffer{
	char buff[BUFFER_SIZE];
	int head;
	int tail;
};

int isEmpty(void);
void bufferInit(void);
void bufferPush(char c);
char bufferPull(void);



/**
	* @brief  Regle l'uart
  * @note   Fonction à lancer avant toute autre.
	* @param  USART_TypeDef Uart : indique l'uart à utiliser (UART2 pour utiliser le STLINK/V2)
  * @retval None
  */
void MyUart_Conf(USART_TypeDef * Uart,int baudrate);

void sendChar(char c);
void sendString(const char* str);
void sendNumber(int n,int digits);




#endif
