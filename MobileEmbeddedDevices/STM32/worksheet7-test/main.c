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
#include <misc.h>

#include "com_port.h"
#include <stm32f10x_usart.h>


/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;

//static int yellowledval = 0;
//static int greenledval=0;
//int greenon=0;

//protorypes
void setup_USART();
void USART2_IRQHandler();
int outbyte(int c);
int inbyte();

//debug
int outbyte2(int c);
int inbyte2();

// buffer
static int buffsize = 20;

char* rxbuffer[20];
char* txbuffer[20];

//char rxdata;
//circular queue
//add to the head, remove from the tail
int rxhead = 0;
int rxtail = 0;

int txhead = 0;
int txtail = 0;

int txsize = 0;
int rxsize = 0;
//change later without size

void setup_USART(){
  //configure the usart com_port
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE );
  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
  /* Remap USART, as USART2 uses alternate pins */
  GPIO_PinRemapConfig( GPIO_Remap_USART2,ENABLE );
  /* Configure USART2 Tx pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; /*changed 8 to 5 for USART2 craig */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init( GPIOD, &GPIO_InitStructure );
  /* Configure USART2 Rx pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; /* changed 9 to 6 for USART2 craig */
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
  USART_InitStructure.USART_HardwareFlowControl =
  USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init( USART2, &USART_InitStructure );
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);


  ///nvic for USART

  //setup the interrupt handler for the usart
  /* Configure interrupts for usart  */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
  NVIC_EnableIRQ(USART2_IRQn);
}

/*
void configurechar c;//if(rxsize >= buffsize){}_timer(void){
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

      TIM_TimeBaseInitTypeDef timerInitStructure;
      timerInitStructure.TIM_Prescaler = 36000;
      timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
      timerInitStructure.TIM_Period = 100;
      timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
      timerInitStructure.TIM_RepetitionCounter = 0;
      TIM_TimeBaseInit(TIM2, &timerInitStructure);
      TIM_Cmd(TIM2, ENABLE);
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    }

void configure_timer2_interrupt(){
        NVIC_InitTypeDef nvicStructure;
        nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
        nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
        nvicStructure.NVIC_IRQChannelSubPriority = 1;
        nvicStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&nvicStructure);
    }

void TIM2_IRQHandler(){
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    if (greenon == 1){
      GPIO_WriteBit(GPIOC , GPIO_Pin_6 , (greenledval) ? Bit_SET : Bit_RESET);
      greenledval = 1 - greenledval;
    }
  }
}
*/

void USART2_IRQHandler(){
  //
  //char c = 'a';

  //input handling
  //TX
  if(txsize >0){//if buffer no empty
    //get char
    //send uart tx
    outbyte2(txbuffer[txtail++]);
    //update txtail
    txtail = txtail % buffsize;//wraps around array
    txsize--;

  }

  //RX
  //if(rxsize >0){
    rxbuffer[rxhead++] = inbyte2();//recieve
    //update txtail
    rxhead = rxhead % buffsize;//wraps around array
    rxsize++;
  //}
}

//test code that doesnt quite work
/*
void tst(){
  //
  char c = 'a';
  outbyte2('I');

  //input handling
  //TX
  if(txsize >0){//if buffer no empty
    outbyte2('T');
    if (USART_GetFlagStatus(USART2, USART_FLAG_TXNE) != RESET){
      outbyte2('X');
      //get char
      c = txbuffer[txtail++];
      //send uart tx
      USART_SendData(USART2, (u16) c);
      //update txtail
      txtail = txtail % buffsize;//wraps around array
      txsize--;
    }
  }

  //RX
  if(rxsize >0){
    outbyte2('R');
    if((USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)){
      outbyte2('X');
      rxbuffer[rxhead++] = (char) USART_ReceiveData(USART2) & 0xff;//recieve
      //update txtail
      rxhead = rxhead % buffsize;//wraps around array
      rxsize++;
      USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
  }
  outbyte2('\n');
  outbyte2('\r');
}
*/
//end







//uart until
//added

//send data
int outbyte(int c){
  txbuffer[txhead++] = c;
  //txhead++;
  txhead = txhead % buffsize;
  txsize++;//for working out when overflow
  return c;
}

//recieve data
int inbyte(){
  char c;//if(rxsize >= buffsize){}
  if(rxsize>0){
    c = rxbuffer[rxtail++];
    rxtail = rxtail % buffsize;//wraps around array
    rxsize--;
    return c;
  }
  return -1;
}


int outbyte2(int c) {

	/* Wait until ready to send */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}
	USART_SendData(USART2, (u16) c);
	return c;
}

int inbyte2() {
	int discard = 0;
	/* Wait until ready to send */
	//while(1){
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
		{}
		return (int)USART_ReceiveData(USART2); //USART_ReceiveData
	//}
}


void print_out(char* instr){
  int i = 0;
  char c;
  while(1){
    c = (int)instr[i];
    if(c == '\0'){
      return;
    }
    outbyte2(c);
    i++;
  }
}

int main(void)
{
  //configure_leds();
  //configure_buttonsA();
  //configure_timer();
  //configure_timer2_interrupt();

  setup_USART();//COMPortInit();
  //outbyte('\r');
  //outbyte('h');
  //outbyte('e');
  //outbyte('l');
  //outbyte('l');
  //outbyte('o');
  //outbyte('\n');


  //outbyte2('W');
  //outbyte2('o');
  //outbyte2('r');
  //outbyte2('l');
  //outbyte2('d');
  //outbyte2('\n');

  ///outbyte('\r');
  while (1) {
    //outbyte();
    int inchar = -1;
    inchar = inbyte();
    if(inchar != -1){
    outbyte(inchar);
  }

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
