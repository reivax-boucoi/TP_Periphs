
#ifndef UART_H
#define UART_H

#include "stm32f103xb.h" 


#define PinUartTX LL_GPIO_PIN_2
#define USART_Pin GPIOA

#define BUFFER_SIZE 64
#define BUFFER_MASK 63

typedef struct Buffer{
	char buff[BUFFER_SIZE];
	int head;
	int tail;
}Buffer;

int isEmpty(Buffer *b);
void bufferInit(Buffer *b);
void bufferPush(Buffer *b,char c);
char bufferPull(Buffer *b);


/**
	* @brief  Regle l'uart
  * @note   Fonction à lancer avant toute autre.
	* @param  USART_TypeDef Uart : indique l'uart à utiliser (UART2 pour utiliser le STLINK/V2)
  * @retval None
  */
void MyUart_Conf(USART_TypeDef * Uart,int baudrate);

void MyUart_sendChar(char c);
void MyUart_sendString(const char* str);
void MyUart_sendNumber(int n,int digits);




#endif
