/*
	Copyright 2012, Michael Scott

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/*changed USART3 to USART2 for Rev B boards - craig 18/2/14 */
#include "com_port.h"

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_adc.h>


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
	//printf("\nParity off!\n");
	flashLEDWarning(PARITY_LED_WARN);
}


void handleFrameErr(void)
{
	//printf("\nFrame error!\n");
	flashLEDWarning(FRAME_LED_WARN);
}

void handleOverrunErr(void)
{
	//printf("\nOverrun!\n");
	flashLEDWarning(OVERRUN_LED_WARN);
}


void handleNoiseErr(void)
{
	//printf("\nNoise!\n");
	flashLEDWarning(NOISE_LED_WARN);
}


void COMPortInit ( void ) {

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
	USART_InitStructure.USART_BaudRate = mainCOM_PORT_BAUD_RATE;
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

void LEDPortInit(void)
{

	GPIO_InitTypeDef GPIO_InitStructure; 

  // GPIOC Periph clock enable 
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 


  // Configure PC6 & PC7 in output pushpull mode 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ; 
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  	GPIO_Init(GPIOC, &GPIO_InitStructure); 


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


void configure_adc(void)
{
		ADC_InitTypeDef ADC_InitStructure;


    RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADCCLK = PCLK22/6 = 72/6=12MHz
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Enable ADC1 Clock

    /* ADC1 configuration */
    ADC_DeInit(ADC1); //Power-on default
    ADC_InitStructure.ADC_Mode = ADC_Mode_FastInterl; //Independent conversion mode (single)
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //Convert single channel only
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //Convert 1 time
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //No external triggering
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //Right 12-bit data alignment
    ADC_InitStructure.ADC_NbrOfChannel = 10; //single channel conversion
    ADC_Init(ADC1, &ADC_InitStructure);
    //ADC_TempSensorVrefintCmd(ENABLE); //wake up temperature sensor
    ADC_Cmd(ADC1, ENABLE); //Enable ADC1
    ADC_ResetCalibration(ADC1); //Enable ADC1 reset calibration register
    }


uint32_t get_adc_val()
{
    uint32_t seed;
   	uint32_t tmpval;
   	int match = 0;
   	int run = 1;
   	
   	
   	tmpval = 1000;
   	
    while(tmpval--);
    
    
   
		  while(ADC_GetResetCalibrationStatus(ADC1)); //check the end of ADC1 reset calibration register  
		  ADC_StartCalibration(ADC1); //Start ADC1 calibration
		
		  while(ADC_GetCalibrationStatus(ADC1)); //Check the end of ADC1 calibration
		  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5); //Select 1.5 cycles conversion for channel 16
		  ADC_SoftwareStartConvCmd(ADC1, ENABLE); //Start ADC1 software conversion

		  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Wail for conversion complete 
		  seed = ADC_GetConversionValue(ADC1); //Read ADC value
		  ADC_ClearFlag(ADC1, ADC_FLAG_EOC); //Clear EOC flag
		  
			seed = seed ^ counter++;
			
			/*// change sample time
			uint16_t i = AD_value%1000;
			i++;
			while(i--);

		  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Wail for conversion complete 
		  AD_value = ADC_GetConversionValue(ADC1); //Read ADC value
		  ADC_ClearFlag(ADC1, ADC_FLAG_EOC); //Clear EOC flag*/
	

		return seed;
}




int main(void)
{	
  	int i,c;
	int STATE = 0;

	COMPortInit();
	LEDPortInit();

printf("hello\n");

int j,t = 0;


	char b[50];
	memset(&b,0,50);

configure_adc();



	for (;;)
	{
	/*

	int *x[4];
	x[0] = malloc(50);

	scanf(" %d",&x[0]);
	printf("x is:%d\n",x[0]);

hexDump(&x[0],0x20);*/


	//printf("\nprinting current stack frame\n");
	//hexDump(__get_MSP(),0x50);

	



	char *bb = (char *)malloc(50);

	printf("bb is:%x\n",bb);

	//hexDump(&bb,0x50);





	

	printf("testing read syscall so type now:\n");


	int j,t = 0;

	char abc;

	scanf("%s",bb);

	printf("%s\n",bb);

hexDump(bb,0x20);
/*

	while(1)
	{
		_read(0,&b[t],1);
		if(b[t]=='\n')
		{
			b[t+1]=0;
			break;
		}
		else
			printf("%c\t",b[t]);

		t++;
	}*/

	//printf("%s\t",b);		

	printf("\n\n==Debug Time==\n");
	printf("\nprinting current stack frame\n");
	hexDump(__get_MSP(),0x50);
	
	printf("\nprinting unit data section\n");
	hexDump(&__bss_start__,0x50);






	printf("\nstarting our math test!\n\n");
	printf("you need to solve random equations\nand will be scored acorrdingly!\n");


	//printf("clock():%d\n",clock());

	
	int xx;
	int points = 0;
	uint32_t seed1;
	uint32_t seed2;
	uint32_t seed3;


	for(xx=0; xx<9; xx++)
	{
		// sample adc for some psuedo random value, its good enough
		
		
		
		seed1 = get_adc_val();
		srand(seed1);
		int a = rand()%100;
		
		seed2 = get_adc_val();
		srand(seed2);
		int b = rand()%100;
		
		seed3 = get_adc_val();
		srand(seed3);
		int c = rand()%3;
		

		
		printf("c is:%d\n",c);

		enum op{plus,sub,divide,mul};

			int x;
		x = malloc(50);
		memset(&x,0,50);
	
		fflush(0);


		//int p = scanf("%d",&x);
		//printf("x is:%d scanf ret:%d\n",x,p);

	//	hexDump(&x,0x20);

//while(1);

		switch(c)
		{
			//memset(&x,0,50);
			case plus:
				printf("%d + %d = ",a,b);
				scanf("%d",&x);
				//hexDump(&x,0x20);
				printf("%d",x);
				if(a+b==x)
				{
					printf("\nCorrect!\n");
					points++;
					break;
				}
				else
				{
					printf("\nWrong! :(\n");
					break;
				}
			case sub:
				printf("%d - %d = ",a,b);
				int sret = scanf("%d\r",&x);
				//printf("sret:%d\n",sret);
				//hexDump(&x,0x20);
				printf("%d",x);
				if(a-b==x)				
				{
					printf("\nCorrect!\n");
					points++;
					break;
				}
				else
				{
					printf("\nWrong! :(\n");
					break;
				}
			case divide:
				printf("%d / %d = ",a,b);
				scanf("%d",&x);
				int e;
				//for(e=100000;e>0;e--);
				//f = inbyte();
				printf("%d",x);
				if(a/b==x)
				{
					printf("\nCorrect!\n");
					points++;
				}
				else
				{
					printf("\nWrong! :(\n");
					break;
				}
			case mul:
				printf("%d * %d = ",a,b);
				scanf("%d",&x);
				printf("%d",x);
				if(a*b==x)
				{
					printf("\nCorrect!\n");
					points++;
				}
				else
				{
					printf("\nWrong! :(\n");
					break;
				}
			default:
				printf("why\n");
		}
			//while(1);
			
		//	free(x);

	}
	
	int newscore = points*10;	
	printf("\n\nyour final score is:%d/100\n",newscore);
	while(1);

}	
	
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

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
