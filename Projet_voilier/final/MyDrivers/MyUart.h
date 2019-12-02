
#ifndef UART_H
#define UART_H

#include "stm32f103xb.h" 


#define PinUartTX LL_GPIO_PIN_9
#define PinUartEnable LL_GPIO_PIN_11
#define USART_Pin GPIOA

#define BUFFER_SIZE 64
#define BUFFER_MASK 63


//D�finition d'un buffer de donn�es
typedef struct Buffer{
	char buff[BUFFER_SIZE];
	int head;
	int tail;
}Buffer;

//Fonction bool�enne qui v�rifie si le buffer de donn�es est plein ou non
int isEmpty(Buffer *b);

//Initialisation du buffer de donn�es
void bufferInit(Buffer *b);

//Fonction qui ajoute un caract�re (1 octet) dans le buffer
void bufferPush(Buffer *b,char c);

//Fonction qui extrait un caract�re pour l'envoi via l'USART
char bufferPull(Buffer *b);

//Fonction qui envoi un caract�re via l'USART
void MyUart_sendChar(char c);

//Fonction qui envoi une cha�ne de caract�res
void MyUart_sendString(const char* str);

//Fonction qui permet l'envoi d'un chiffre/nombre
void MyUart_sendNumber(int n,int digits);

//Fonction qui envoi un caract�re via l'USART si autorisation via le flag TXE
void MyUart_sendCharImmediate(char c);

/**
	* @brief  Regle l'uart
  * @note   Fonction � lancer avant toute autre.
	* @param  USART_TypeDef Uart : indique l'uart � utiliser (UART2 pour utiliser le STLINK/V2)
  * @retval None
  */
void MyUart_Conf(USART_TypeDef * Uart,int baudrate);







#endif
