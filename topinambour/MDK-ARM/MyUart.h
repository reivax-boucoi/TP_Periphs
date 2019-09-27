
#ifndef UART_H
#define UART_H

#include "stm32f103xb.h" 


#define PinUartTX LL_GPIO_PIN_2
#define USART_Pin GPIOA


/**
	* @brief  Regle l'uart
  * @note   Fonction à lancer avant toute autre.
	* @param  USART_TypeDef Uart : indique l'uart à utiliser (UART2 pour utiliser le STLINK/V2)
  * @retval None
  */
void MyUart_Conf(USART_TypeDef * Uart,int baudrate);




#endif