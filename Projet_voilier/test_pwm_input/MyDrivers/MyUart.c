
#include "MyUart.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges

Buffer tx_buff;

USART_TypeDef * myUart; 
void (*IRQ_USART_handler_ptr) (void);


/* On vérifie que le buffer est plein ou non :
		- si le head pointe vers la même position que tail, le buffer est VIDE
		- sinon le buffer est PLEIN
*/
int isEmpty(Buffer *b){
	return (b->head==b->tail);
}

/* Initialiser les pointeurs du buffer head et tail */
void bufferInit(Buffer *b){
	b->head=0;
	b->tail=0;
}


/*Ajouter un caractère dans le buffer */
void bufferPush(Buffer *b,char c){
	b->buff[b->head]=c;
	b->head=(b->head+1)&BUFFER_MASK;
}


/*Extraire un caractère du buffer pour l'envoi */
char bufferPull(Buffer *b){
	char ret=b->buff[b->tail];
	b->tail=(b->tail+1)&BUFFER_MASK;
	return ret;
}


/*On active l'Interruption liée à la transmission de donnée de l'USART puis on envoi le caractère c */
void MyUart_sendChar(char c){
	//MyUart_sendCharImmediate(c);/*
	if(isEmpty(&tx_buff)) LL_USART_EnableIT_TXE(myUart);
	bufferPush(&tx_buff,c);
}
void MyUart_sendCharImmediate(char c){
	while(LL_USART_IsActiveFlag_TXE(myUart) != RESET);
	LL_USART_TransmitData8(myUart,c);
}

/* Fonction permettant d'envoyer la chaine str (appel en boucle de sendChar() ) */
void MyUart_sendString(const char* str){
	while(*str>0)MyUart_sendChar(*str++);
}

/*Envoi un nmbre (min,sec...) en fonction du nombre de digits (Si digit>0 nb digit fixe, sinon nb digit flexible).*/

void MyUart_sendNumber(int n,int digits){
	int d=1;
	if(digits){	//On aura donc 2 digits, sinon c'est flexible.
	d=10;
	}else{
		while((10*d)<=n)d*=10; //On check le nb de chiffres contenus dans n
	}
	while(d>0){
		MyUart_sendChar(((n/d)%10)+0x30); //Décalage pur atteindre les nombres dans la table ASCII
		d/=10;
	}
}


//Fonction générique pour le traitement des interruptions niveau USART
void IRQ_USART_handler(void){
	if(LL_USART_IsActiveFlag_TXE(myUart)!=RESET){
		if(isEmpty(&tx_buff)){
			LL_USART_DisableIT_TXE(myUart);
		}else{
			LL_USART_TransmitData8(myUart,bufferPull(&tx_buff));
		}
		myUart->SR&=~(LL_USART_SR_TXE);//LL_USART_ClearFlag_TXE(USART1);
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
	
	//Configuration de l'USART
	LL_GPIO_InitTypeDef uart_tx;
	uart_tx.Pin=PinUartTX;
	uart_tx.Mode=LL_GPIO_MODE_ALTERNATE;
	uart_tx.Pull=LL_GPIO_PULL_DOWN;
	uart_tx.Speed=LL_GPIO_SPEED_FREQ_LOW;
	uart_tx.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	//Enable le Tx associé à l'émetteur (PA11)
	LL_GPIO_InitTypeDef uart_en;
	uart_en.Pin=PinUartEnable;
	uart_en.Mode=LL_GPIO_MODE_OUTPUT;
	uart_en.Pull=LL_GPIO_PULL_UP;
	uart_en.Speed=LL_GPIO_SPEED_FREQ_LOW;
	uart_en.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	if(Uart==USART1){
	LL_GPIO_AF_EnableRemap_USART1();
	}else if(Uart==USART2){
	LL_GPIO_AF_EnableRemap_USART2();
	}else if(Uart==USART3){
	LL_GPIO_AF_EnableRemap_USART3();
	}
	
	LL_GPIO_Init(USART_Pin, &uart_tx);
	LL_GPIO_Init(USART_Pin, &uart_en);
	
	LL_USART_ClockInitTypeDef myUartClock;
	myUartClock.ClockOutput=LL_USART_CLOCK_ENABLE;
	myUartClock.ClockPhase=LL_USART_PHASE_1EDGE;
	myUartClock.ClockPolarity=LL_USART_POLARITY_LOW;
	myUartClock.LastBitClockPulse=LL_USART_LASTCLKPULSE_NO_OUTPUT;
	
	LL_USART_ClockInit(Uart,&myUartClock);
	
	
	LL_USART_InitTypeDef myUartInit;
	myUartInit.BaudRate=baudrate;
	myUartInit.DataWidth=LL_USART_DATAWIDTH_8B;
	myUartInit.StopBits=LL_USART_STOPBITS_1;
	myUartInit.Parity=LL_USART_PARITY_NONE;
	myUartInit.TransferDirection=LL_USART_DIRECTION_TX;
	myUartInit.HardwareFlowControl=LL_USART_HWCONTROL_NONE;
	myUartInit.OverSampling=LL_USART_OVERSAMPLING_16;
	
	LL_USART_Init(Uart,&myUartInit);
	
	if(Uart==USART1){
	NVIC_SetPriority(USART1_IRQn,0x40); 
	NVIC_EnableIRQ(USART1_IRQn);
} else if(Uart==USART2){

	NVIC_SetPriority(USART2_IRQn,0x40); 
	NVIC_EnableIRQ(USART2_IRQn);
	}else if(Uart==USART3){

	NVIC_SetPriority(USART3_IRQn,0x40); 
	NVIC_EnableIRQ(USART3_IRQn);
	}
	myUart=Uart;
	IRQ_USART_handler_ptr=IRQ_USART_handler;
	LL_USART_Enable(Uart);
	
	//Init buffer de transmission
	bufferInit(&tx_buff);	

}


void USART1_IRQHandler(void){
	if (IRQ_USART_handler_ptr != 0)(*IRQ_USART_handler_ptr) ();
	
}

void USART2_IRQHandler(void){
	if (IRQ_USART_handler_ptr != 0)(*IRQ_USART_handler_ptr) ();
	
}
void USART3_IRQHandler(void){
	if (IRQ_USART_handler_ptr != 0)(*IRQ_USART_handler_ptr) ();
	
}

