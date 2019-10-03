
#include "MyUart.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges

struct Buffer tx_buff;



/* On vérifie que le buffer est plein ou non :
		- si le head pointe vers la même position que tail, le buffer est VIDE
		- sinon le buffer est PLEIN
*/
int isEmpty(void){
	return (tx_buff.head==tx_buff.tail);
}

/* Initialiser les pointeurs du buffer head et tail */
void bufferInit(void){
	tx_buff.head=0;
	tx_buff.tail=0;
}


/*Ajouter un caractère dans le buffer */
void bufferPush(char c){
	tx_buff.buff[tx_buff.head]=c;
	tx_buff.head=(tx_buff.head+1)&BUFFER_MASK;
}


/*Extraire un caractère du buffer pour l'envoi */
char bufferPull(void){
	char ret=tx_buff.buff[tx_buff.tail];
	tx_buff.tail=(tx_buff.tail+1)&BUFFER_MASK;
	return ret;
}


/*On active l'Interruption liée à la transmission de donnée de l'USART puis on envoi le caractère c */
void sendChar(char c){
	LL_USART_EnableIT_TXE(USART2);
	bufferPush(c);
}

/* Fonction permettant d'envoyer la chaine str (appel en boucle de sendChar() ) */
void sendString(const char* str){
	while(*str>0)sendChar(*str++);
}

/*Envoi un nmbre (min,sec...) en fonction du nombre de digits (1 ou 2 dans notre cas).*/

void sendNumber(int n,int digits){
	int d=1;
	if(digits){	//On aura donc 2 digits, sinon c'est flexible.
	d=10;
	}else{
		while((10*d)<=n)d*=10; //On check le nb de chiffres contenus dans n
	}
	while(d>0){
		sendChar(((n/d)%10)+0x30); //Décalage pur atteindre les nombres dans la table ASCII
		d/=10;
	}
}

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
	NVIC_SetPriority(USART2_IRQn,0x40);
	NVIC_EnableIRQ(USART2_IRQn);
	
	LL_USART_Enable(Uart);
	bufferInit();
}

// Ce handler remplace le handler WEAK associé 
void USART2_IRQHandler(void){
	if(LL_USART_IsActiveFlag_TXE(USART2)!=RESET){
		if(isEmpty()){
			LL_USART_DisableIT_TXE(USART2);
		}else{
			LL_USART_TransmitData8(USART2,bufferPull());
		}
		USART2->SR&=~(LL_USART_SR_TXE);//LL_USART_ClearFlag_TXE(USART2);
	}
}
