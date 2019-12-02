
#ifndef UART_H
#define UART_H

#include "stm32f103xb.h" 


#define PinUartTX LL_GPIO_PIN_9
#define PinUartEnable LL_GPIO_PIN_11
#define USART_Pin GPIOA

#define BUFFER_SIZE 64
#define BUFFER_MASK 63


//Définition d'un buffer de données
typedef struct Buffer{
	char buff[BUFFER_SIZE];
	int head;
	int tail;
}Buffer;

//Fonction booléenne qui vérifie si le buffer de données est plein ou non
int isEmpty(Buffer *b);

//Initialisation du buffer de données
void bufferInit(Buffer *b);

//Fonction qui ajoute un caractère (1 octet) dans le buffer
void bufferPush(Buffer *b,char c);

//Fonction qui extrait un caractère pour l'envoi via l'USART
char bufferPull(Buffer *b);

//Fonction qui envoi un caractère via l'USART
void MyUart_sendChar(char c);

//Fonction qui envoi une chaîne de caractères
void MyUart_sendString(const char* str);

//Fonction qui permet l'envoi d'un chiffre/nombre
void MyUart_sendNumber(int n,int digits);

//Fonction qui envoi un caractère via l'USART si autorisation via le flag TXE
void MyUart_sendCharImmediate(char c);

/**
	* @brief  Regle l'uart
  * @note   Fonction à lancer avant toute autre.
	* @param  USART_TypeDef Uart : indique l'uart à utiliser (UART2 pour utiliser le STLINK/V2)
  * @retval None
  */
void MyUart_Conf(USART_TypeDef * Uart,int baudrate);







#endif
