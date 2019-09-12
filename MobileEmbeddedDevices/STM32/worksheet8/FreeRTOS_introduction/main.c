/* module banner */

#include <FreeRTOS.h>
#include <stm32f10x.h>
#include <partest.h>
#include <task.h>
#include <semphr.h>

#include <stm32f10x_usart.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_rcc.h> 
#include <stm32f10x_gpio.h> 


extern char _ebss;
extern char __bss_start__;


#define PARITY_LED_WARN		0
#define OVERRUN_LED_WARN	1  
#define NOISE_LED_WARN 		2
#define FRAME_LED_WARN 		3

int counter = 5;
/*
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
*/


void outbyte(int c) {

	/* Wait until ready to send */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
	}

	USART_SendData(USART2, (u16) c);

	return c;
}


int inbyte(void) {


	// for generating USART errors to test our error handling
	//printf("%s\n",USART_ReceiveData(USART2));

	
	/*// Check error flags before we send anything 
	while(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET  ||
		USART_GetFlagStatus(USART2, USART_FLAG_NE)  != RESET  ||
		USART_GetFlagStatus(USART2, USART_FLAG_FE)  != RESET  ||
		USART_GetFlagStatus(USART2, USART_FLAG_PE)  != RESET)
	{*/


		// Handle each error
		if(USART_GetFlagStatus(USART2, USART_FLAG_ORE))handleOverrunErr();
		if(USART_GetFlagStatus(USART2, USART_FLAG_NE))handleNoiseErr();
		if(USART_GetFlagStatus(USART2, USART_FLAG_FE))handleFrameErr();
		if(USART_GetFlagStatus(USART2, USART_FLAG_PE))handleParityErr();
	//	break;
//	}


	/* Wait until ready to recieve */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{
	}



	return USART_ReceiveData(USART2);;
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






#define mainGREEN_LED_TOGGLE (tskIDLE_PRIORITY + 1)
#define mainORANGE_LED_TOGGLE (tskIDLE_PRIORITY + 1) 

#define button_bounce_delay (200 / portTICK_RATE_MS)
static xSemaphoreHandle xButtonSemaphore;

void vGreenToggleTask(void *pvParameters)
{
  int i;

  for(;;)
  {
    vParTestToggleLED(1);
    vTaskDelay( 500 / portTICK_PERIOD_MS );
  }
}

void vOrangeToggleTask(void *pvParameters)
{
  for(;;)
  {
    vParTestToggleLED(0);
    vTaskDelay( 500 / portTICK_PERIOD_MS );
  }
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}

typedef struct{
	int ledNum;
	int delayTime;
}ledArgs;

static ledArgs ledArgs_1 = {0, 300};
static ledArgs ledArgs_2 = {1, 30};

int keepLedsOn = 1;
int button_pressed = 0;

void ledToggleTask(void* pvParameters)
{

	ledArgs *ledargs;
	ledargs = (ledArgs*)pvParameters;
	//int argArray[2] = pvParameters;
	int ledNum = ledargs->ledNum;//ledArgs_1.ledNum;
	int delayTime = ledargs->delayTime;//ledArgs_1.delayTime;
	
	while(1)
	{
		if(keepLedsOn)
		{
			//vParTestSetLED(ledNum,1);
			vParTestToggleLED(ledNum);
			vTaskDelay(delayTime / portTICK_PERIOD_MS);
		}
		else
		{
			//vParTestToggleLED(1);
			vParTestSetLED(ledNum,0);
			vTaskDelay(delayTime / portTICK_PERIOD_MS);
		}
		
				//	vParTestToggleLED(ledNum);
				//				vTaskDelay(delayTime / portTICK_PERIOD_MS);
	}
}

void checkButtonAPress(void* pvParameters)
{
int state = 0;
	while(1)
	{
		//button state high
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{
			int i = 1440000/6;// delay 20ms
			while(i--);
			printf("button a says hello\n");

			// button state pressed
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			{			
					button_pressed = 1;

					// keep led from switching when button is held
					while(button_pressed)
					{
						// allow loop to be broken when button is released
						if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))break;
					}
					keepLedsOn = !keepLedsOn;
			}
		}
		else
		{
			button_pressed =0;
		}
		
		
		
 			/*if(state != eTaskGetState(xTaskGetHandle("LedToggleGreen")) )
 			{*/
  			//printf("state:%d\n",state);
  			  				//vTaskDelay(1000 / portTICK_PERIOD_MS);
  			//state++;
  		//}

  		//state = eTaskGetState(xTaskGetHandle("LedToggleGreen"));
  				//vTaskDelay(1000 / portTICK_PERIOD_MS);
	
	}
}

void buttonA_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 

  // GPIOC Periph clock enable 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  // Configure PA0 in input floating mode  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure );
}

void hwInit(void)
{
	uartInit();
	buttonA_init();
}

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

void taskMonitor(void)
{
int state = 0;
int prevState = 1;
 while(1)
	{

			//int i = 10000;
  		//while(i--);

 			
 			if(state != 1)//eTaskGetState(xTaskGetHandle("LedToggleGreen")) )
 			{
  			printf("state:%d\n",state);
  			  				vTaskDelay(1000 / portTICK_PERIOD_MS);
  			state++;
  		}

  		//state = eTaskGetState(xTaskGetHandle("LedToggleGreen"));
  				vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

int main(void) 
{   


  int i,j=0;
  vParTestInitialise(); 
  


	hwInit();

  
  /*
   BaseType_t xTaskCreate(
							  TaskFunction_t pvTaskCode,
							  const char * const pcName,
							  configSTACK_DEPTH_TYPE usStackDepth,
							  void *pvParameters,
							  UBaseType_t uxPriority,
							  TaskHandle_t *pvCreatedTask
						  );*/

  xTaskCreate( checkButtonAPress, "checkButtonAPress", 128, NULL, 10, NULL );
  
  // set an led task lower than the buttton press task priority and you will starve it
  xTaskCreate( ledToggleTask, "LedToggleGreen", 128, (void *)&ledArgs_1, 4, NULL );
  xTaskCreate( ledToggleTask, "LedToggleOrange", 128, (void *)&ledArgs_2, 10, NULL );
  
    //xTaskCreate( taskMonitor, "taskMonitor", 128, NULL, 11, NULL );

  //int state;
 // printf("state of led toggle:");
  //outbyte(state);


  vTaskStartScheduler();

	while(1);

  return 0;
} 

#ifdef  USE_FULL_ASSERT 

/** 
  * @brief  Reports the name of the source file and the source line number 
  *         where the assert_param error has occurred. 
  * @param  file: pointer to the source file name 
  * @param  line: assert_param error line source number 
  * @retval None 
  */ 
void assert_failed(uint8_t* file, uint32_t line) 
{ 
  /* User can add his own implementation to report the file name and line number, 
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */ 

  /* Infinite loop */ 
  while (1) 
  { 
  } 
} 

#endif 

