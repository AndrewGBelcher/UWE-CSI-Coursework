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

#define PARITY_LED_WARN		0
#define OVERRUN_LED_WARN	1  
#define NOISE_LED_WARN 		2
#define FRAME_LED_WARN 		3

int outbyte(int c) {

	/* Wait until ready to send */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
	}

	USART_SendData(USART2, (u16) c);

	//USART_SendData(USART2, 0x34);

	return c;
}


int inbyte(void) {


	/* Handle each error*/
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE))handleOverrunErr();
	if(USART_GetFlagStatus(USART2, USART_FLAG_NE))handleNoiseErr();
	if(USART_GetFlagStatus(USART2, USART_FLAG_FE))handleFrameErr();
	if(USART_GetFlagStatus(USART2, USART_FLAG_PE))handleParityErr();


	/* Wait until ready to recieve */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{
	}

	int c = USART_ReceiveData(USART2);

	return c;
}

void handleParityErr(void)
{



	outbyte('\n');
	outbyte('P');
	outbyte('a');
	outbyte('r');
	outbyte('i');
	outbyte('t');
	outbyte('y');
	outbyte(' ');
	outbyte('o');
	outbyte('f');
	outbyte('f');
	outbyte('!');
	outbyte('\n');
	flashLEDWarning(PARITY_LED_WARN);



}


void handleFrameErr(void)
{



	outbyte('\n');
	outbyte('F');
	outbyte('r');
	outbyte('a');
	outbyte('m');
	outbyte('e');
	outbyte(' ');
	outbyte('e');
	outbyte('r');
	outbyte('r');
	outbyte('!');
	outbyte('\n');
	flashLEDWarning(FRAME_LED_WARN);

}

void handleOverrunErr(void)
{



	outbyte('\n');
	outbyte('O');
	outbyte('v');
	outbyte('e');
	outbyte('r');
	outbyte('R');
	outbyte('u');
	outbyte('n');
	outbyte('!');
	outbyte('\n');
	flashLEDWarning(OVERRUN_LED_WARN);


}


void handleNoiseErr(void)
{


	outbyte('\n');
	outbyte('N');
	outbyte('o');
	outbyte('i');
	outbyte('s');
	outbyte('e');
	outbyte('!');
	outbyte('\n');
	flashLEDWarning(NOISE_LED_WARN);


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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIO_MODE_AF_OD;//GPIO_Mode_AF_PP;
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

	/* Enable USART */
	USART_Cmd( USART2, ENABLE );
}

int main(void)

{
	int i;
	
	COMPortInit();
	LEDPortInit();

	for ( i=0; i != 10; i++)
	{
	outbyte('h');
	outbyte('e');
	outbyte('l');
	outbyte('l');
	outbyte('o');
	
}
	outbyte('\n');
	outbyte('\r');
	outbyte('b');
	outbyte('y');
	outbyte('e');


	while(1)
	{
		outbyte(inbyte());
		outbyte('\n');
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
