/**
 * \addtogroup helloworld
 * @{
 */
/**
 * \file
 *         An example of how to write uIP applications
 *         with protosockets.
 * \author
 *         Adam Dunkels <adam@sics.se>
 */
/*
 * This is a short example of how to write uIP applications using
 * protosockets.
 */
/*
 * We define the application state (struct hello_world_state) in the
 * hello-world.h file, so we need to include it here. We also include
 * uip.h (since this cannot be included in hello-world.h) and
 * <string.h>, since we use the memcpy() function in the code.
 */
#include "hello_world.h"
#include "uip.h"
#include <string.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h> 
#include <stm32f10x_rcc.h> 

#define PARITY_LED_WARN		0
#define OVERRUN_LED_WARN	1  
#define NOISE_LED_WARN 		2
#define FRAME_LED_WARN 		3
/*
 * Declaration of the protosocket function that handles the connection
 * (defined at the end of the code).
 */
static int handle_connection(struct hello_world_state *s);
/*---------------------------------------------------------------------------*/
/*
 * The initialization function. We must explicitly call this function
 * from the system initialization code, some time after uip_init() is
 * called.
 */
void
hello_world_init(void)
{
uartInit();
  /* We start to listen for connections on TCP port 1000. */
  uip_listen(HTONS(1000));
}
/*---------------------------------------------------------------------------*/
/*
 * In hello-world.h we have defined the UIP_APPCALL macro to
 * hello_world_appcall so that this funcion is uIP's application
 * function. This function is called whenever an uIP event occurs
 * (e.g. when a new connection is established, new data arrives, sent
 * data is acknowledged, data needs to be retransmitted, etc.).
 */
void
hello_world_appcall(void)
{
  /*
   * The uip_conn structure has a field called "appstate" that holds
   * the application state of the connection. We make a pointer to
   * this to access it easier.
   */
  struct hello_world_state *s = &(uip_conn->appstate);
  /*
   * If a new connection was just established, we should initialize
   * the protosocket in our applications' state structure.
   */
  if(uip_connected()) {
    PSOCK_INIT(&s->p, s->inputbuffer, sizeof(s->inputbuffer));
  }
  /*
   * Finally, we run the protosocket function that actually handles
   * the communication. We pass it a pointer to the application state
   * of the current connection.
   */
  handle_connection(s);
}
/*---------------------------------------------------------------------------*/
/*
 * This is the protosocket function that handles the communication. A
 * protosocket function must always return an int, but must never
 * explicitly return - all return statements are hidden in the PSOCK
 * macros.
 */
 
 static struct psock *psocket;
 
 #define printfsock(format, ...)\
		do{\
				char buffer[512] = {0xaa,0x43};\
							psock_send_str(psocket, buffer);\
				}while(0)
			//	int size = sprintf(buffer,format, ##__VA_ARGS__);\

				
				


void uartInit(void)
{

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE ); 

	/* Enable USART2 clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );

	/* Remap USART, as USART2 uses alternate pins */
	GPIO_PinRemapConfig( GPIO_Remap_USART2,ENABLE );

	/* Configure USART2 Tx pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   /*changed from 8 to 5 for USART craig 2*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init( GPIOD, &GPIO_InitStructure );

	/* Configure USART2 Rx pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; /* changed from 9 to 6 for USART 2 craig */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOD, &GPIO_InitStructure );

	/* Configure USART 8N1 */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init( USART2, &USART_InitStructure );
	//USART_DeInit

	/* Enable USART */
	USART_Cmd( USART2, ENABLE );
}







void handleParityErr(void)
{
	printf("\nParity off!\n");
	flashLEDWarning(PARITY_LED_WARN);
}


void handleFrameErr(void)
{
	printf("\nFrame error!\n");
	flashLEDWarning(FRAME_LED_WARN);
}

void handleOverrunErr(void)
{
	printf("\nOverrun!\n");
	flashLEDWarning(OVERRUN_LED_WARN);
}


void handleNoiseErr(void)
{
	printf("\nNoise!\n");
	flashLEDWarning(NOISE_LED_WARN);
}


void flashLEDWarning(int error)
{

	int i;

	switch(error)
	{
		case NOISE_LED_WARN:
		//case PARITY_LED_WARN:
			// CODE FOR LED ALTERNATING

			// set led 
			GPIOC->BSRR = 1 << 6; // green
	
			i = 0xfffff;
			while(--i); // do nothing for a bit while led is on


			// reset led 
			GPIOC->BRR = 1 << 6; // green

			i = 0xfffff;
			while(--i); // do nothing for a bit while led is off
			break;

		case OVERRUN_LED_WARN:

			// CODE FOR LED ALTERNATING

			// set led 
			GPIOC->BSRR = 1 << 7; // yellow

	
			i = 0xfffff;
			while(--i); // do nothing for a bit while led is on


			// reset led 
			GPIOC->BRR = 1 << 7; // yellow


			i = 0xfffff;
			while(--i); // do nothing for a bit while led is off
			break;

		case PARITY_LED_WARN:

			// CODE FOR LED ALTERNATING

			// set led 
			GPIOC->BSRR = 1 << 7; // yellow

	
			i = 0xffffff;
			while(--i); // do nothing for a bit while led is on


			// reset led 
			GPIOC->BRR = 1 << 7; // yellow


			i = 0xfff;
			while(--i); // do nothing for a bit while led is off
			break;
		case FRAME_LED_WARN:

			// CODE FOR LED ALTERNATING

			// set led 
			GPIOC->BSRR = 1 << 6;

	
			i = 0xffffff;
			while(--i); // do nothing for a bit while led is on


			// reset led 
			GPIOC->BRR = 1 << 6; // green


			i = 0xfff;
			while(--i); // do nothing for a bit while led is off
			break;

			

	}
}




 
static int
handle_connection(struct hello_world_state *s)
{

  psocket = &s->p;
  PSOCK_BEGIN(&s->p);
  PSOCK_SEND_STR(&s->p, "Hello. What is your name?\n");
  PSOCK_READTO(&s->p, '\n');
 
  strncpy(s->name, s->inputbuffer, strlen(s->inputbuffer));
  int len = strlen(s->name); //grab length
  s->name[len] = '\0'; // null terminate

  PSOCK_SEND_STR(&s->p, "Hello ");
  PSOCK_SEND_STR(&s->p, s->name);
  
  // clear our buffers
  memset(s->name,0,100);
  memset(s->inputbuffer,0,100);

  PSOCK_CLOSE(&s->p);  
  PSOCK_END(&s->p);

  
}
// a nice hexdump function taking from pieces of code here and there
int hexDump(const void *data, int size)
{
	unsigned char *d = (unsigned char *)data;
	int consoleSize = 16;
	char b[consoleSize + 3];
	int i;

	if(data == 0){
		return -1;
		}
	b[0] = '|';
	b[consoleSize + 1] = '|';
	b[consoleSize + 2] = '\0';
	
	printf("\n-------HEX DUMP------\n");
	for (i = 0; i < size; i++)
	{
		if ((i % consoleSize) == 0)
		{
			if (i != 0){
				printf("  %s\n", b);
				}
			printf("%08lx ", (unsigned char *)data + i);
		}

		if(i % consoleSize == 8)
			printf(" ");
		printf(" %02x", d[i]);

		if (d[i] >= ' ' && d[i] <= '~')
			b[i % consoleSize + 1] = d[i];
		else
			b[i % consoleSize + 1] = '.';
	}
	while((i % consoleSize) != 0)
	{
		if(i % consoleSize == 8)
			printf("    ");
		else
			printf("   ");
		b[i % consoleSize + 1] = '.';
		i++;
	}
	printf("  %s\n\n", b);

	return 0;
}
/*---------------------------------------------------------------------------*/

