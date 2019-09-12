/******************************************************************************** 
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team 
  * @version V3.5.0 
  * @date    08-April-2011 
  * @brief   Main program body. 
  ****************************************************************************** 
  * @attention 
  * 
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS 
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE 
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY 
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING 
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS. 
  * 
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2> 
  ****************************************************************************** 
  */ 

/* Includes ------------------------------------------------------------------*/ 
#include <stm32f10x.h> 	
#include <stm32f10x_rcc.h> 
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_usart.h>


#include <misc.h> 
#include "com_port.h"


#define PARITY_LED_WARN		0
#define OVERRUN_LED_WARN	1  
#define NOISE_LED_WARN 		2
#define FRAME_LED_WARN 		3


/* Private variables ---------------------------------------------------------*/ 
GPIO_InitTypeDef GPIO_InitStructure;
static int yellowledval, greenledval = 0;
int gotit, rxdata = 0;
int bufIndex;
char charBuf[20];



void putchar(int c)
{
	if(bufIndex < 20 && bufIndex >= 0)
	{


		printf("\nindex:%d\n",bufIndex);
		charBuf[bufIndex++] = c;

		int i;
		for(i=0;i<20;i++)outbyte(charBuf[i]);

	}
	else
	{

		printf("\nbuffer full!\n");	
		bufIndex = 19;	
	}
}

int getchar(void)
{
	char c;

	if(bufIndex < 20 && bufIndex >= 0)
	{
		c	= charBuf[bufIndex--];
		return c;
	}
	else
	{
		printf("\nbuffer empty!\n");
		
		int i;
		for(i=0;i<20;i++)charBuf[i]=0;

		bufIndex = 0;	
		return 0;
	}

}



int outbyte(int c) {

	/* Wait until ready to send */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
	}

	USART_SendData(USART2, (u16) c);

	return c;
}


int inbyte(void) {


	/* Wait until ready to recieve */
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
	{
	}

	int c = USART_ReceiveData(USART2);
	USART_SendData(USART2, (u16) c);

	return c;
}







void configure_buttonsA(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* GPIOC/A Periph clock enable */ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

	/* Initialise the GPIOA for button WKUP */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

	/* Connect Button EXTI Line to Button GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

	/*Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Configure interrupts for button */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

}

void  EXTI0_IRQHandler()
{
    if ( EXTI_GetITStatus(EXTI_Line0) == SET)
    {
				printf("\nButton A says hello :)\n");
				EXTI_ClearITPendingBit( EXTI_Line0 );	
    }

}



void configure_buttonsC(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	
	/* Initialise the GPIOC for button TAMPER */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init( GPIOC, &GPIO_InitStructure );

	/* Connect Button EXTI Line to Button GPIO Pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);

	/*Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Configure interrupts for button */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

}



void  EXTI15_10_IRQHandler()
{
/*
    if ( EXTI_GetITStatus(EXTI_Line13) == SET)
    {
			EXTI_ClearITPendingBit( EXTI_Line13 );	
			GPIO_WriteBit(GPIOC , GPIO_Pin_6 , (yellowledval) ? Bit_SET : Bit_RESET);
			//int i = 100000;
			//while(i--);
			yellowledval = 1 - yellowledval;
    }*/

}



void COMPortInit ( void ) {

	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


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

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource6);

	/* Enable USART */
	USART_Cmd( USART2, ENABLE );

	/* Configure USART 8N1 */
	USART_InitStructure.USART_BaudRate = mainCOM_PORT_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init( USART2, &USART_InitStructure );
	//USART_DeInit


	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Configure interrupts for usart */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_EnableIRQ(USART2_IRQn);


}


void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		rxdata = (int) USART_ReceiveData(USART2) & 0xff;
		//gotit = 1;     
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		putchar(rxdata);


	}

	if(USART_GetITStatus(USART2, USART_IT_TXE) == RESET)
	{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) == RESET)
			USART_SendData(USART2, (u16) getchar());
			//USART_SetITPendingBit(USART2,USART_IT_TXE);
	}

	if(USART_GetITStatus(USART2, USART_IT_ORE) != RESET)
	{
			//bufIndex = 0;
			//printf("\n%s\n",bufIndex);
			printf("\noverun!\n");
			USART_ClearITPendingBit(USART2,USART_IT_ORE);
	}

}


void configure_timer(void)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure PC7 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // alt func push pull
	GPIO_Init(GPIOC, &GPIO_InitStructure);



	// Map TIM3_CH1 to GPIOC_Pin6 & GPIO_Pin7
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

	// Let PWM freqency equal 100Hz
	// Let period equal 1000. Therefore, timer runs from zero to 1000.
	// Gives 0.1Hz resolutions.
	// Solving for prescaler gives 240.
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 240 - 1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;

//	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	
	// Initial duty cycle equals 0%. Value can range from zero to 999.
	TIM_OCInitStruct.TIM_Pulse = 0; // 0 .. 1000 (0=Always Off, 999=Always On)
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);

	TIM_Cmd(TIM3,ENABLE);

}



int main(void) 
{   

  //configure_buttonsC();
  configure_buttonsA();
	COMPortInit();
    
	int i;
	int n = 1;
	int brightness = 0;
	configure_timer();
	
  while (1) 
  { 
		if(brightness >= 1000) n=-1; // if brightness maximum change direction

		if(brightness <= 0) n=1;		 // if brightness minimum change direction
		brightness += n;						 // increment or decrement brightness refer by direction

		/* =complement of brightness & mask/align to 1000 to create value slider on opposite ends=
 
		*i.e 600+23 ~= 001001101111 +23 is needed because 0x3ff mask is 1023 and val is -
									 110110010000 = 3,472 & 0x3ff
									 001111111111 is mask of 0x3ff
									 000110010000 = 400! yay! 400/600 brightness slide

		we now have a slider for brightness where yellow/green leds are on either side
		of the current value in range of 1000 
		*/
	
		TIM3->CCR2 =~(brightness+23)&0x3ff; 		// set yellow brightness
		TIM3->CCR1 = brightness; 		 						// set green brightness

		//printf("%d\n",~(brightness+23)&0x3ff);
		for(i=0;i<0x1000;i++); 			 // delay
	
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

