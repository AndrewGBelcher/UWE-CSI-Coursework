/** 
  ****************************************************************************** 
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

/* Private variables ---------------------------------------------------------*/ 
GPIO_InitTypeDef GPIO_InitStructure; 


	int GreenOn = 1;
	int YellowOn = 0;
	int button_pressed = 0;


int main(void) 
{ 
	volatile uint32_t i; 
	static int greenledval = 0;	 
	 

  ///!< At this stage the microcontroller clock setting is already configured, 
    //   this is done through SystemInit() function which is called from startup 
    //   file (startup_stm32f10x_xx.s) before to branch to application main. 
    //   To reconfigure the default setting of SystemInit() function, refer to 
    //   system_stm32f10x.c file 
          
        
  // GPIOC Periph clock enable 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  // Configure PC6 & PC7 in output pushpull mode 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

  // Configure PA0 in input floating mode  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure );

  // Configure PC13 in input floating mode 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &GPIO_InitStructure );


  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PD0 or PD2 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing. 
     This code needs to be compiled with high speed optimization option.  */ 

  while (1) 
  { 



	/*
		Goal here is to detect button press, debounce the signal, and then flip between green/yellow leds once
		after flipping we set pressed to true making the flip only happen once
		if the button signal is not high then we reset button state to not being pressed
		allowing the user to press a button again to toggle between the leds again.
		
		This prevents the user from swapping between leds too quickly so that they it doesnt
		appear as if both leds are on at the same time
	*/



	//button state high
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		i = 1000000;// delay 20ms
		while(i--);

		// button state pressed
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
		{			

				button_pressed = 1;

				// keep led from switching when button is held
				while(button_pressed)
				{
					// drive led
					GPIO_WriteBit(GPIOC , GPIO_Pin_6 , GreenOn);
					GPIO_WriteBit(GPIOC , GPIO_Pin_7 , YellowOn);

					// allow loop to be broken when button is released
					if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))break;
				}
				GreenOn = !GreenOn; // invert GreenOn state
				YellowOn = !YellowOn; // invert YellowOn state
		}
	}
	else
	{

		button_pressed =0;

		// every loop of the program we are updating the leds state
		GPIO_WriteBit(GPIOC , GPIO_Pin_6 , GreenOn);
		GPIO_WriteBit(GPIOC , GPIO_Pin_7 , YellowOn);

	}
	// else button state low
	// else button state released





	/* 
	using write bit, we compiled with asm from that library 
	function adding to the size of the text section..

	without it we require fewer assembly!

	without write bit
	   text	   data	    bss	    dec	    hex	filename
   	   112	      0	      4	    116	     74	./main.o

	with write bit
	   text	   data	    bss	    dec	    hex	filename
    	   180	      0	      4	    184	     b8	./main.o
	*/

/*

	// CODE FOR LED ALTERNATING

	// set led 
	GPIOC->BSRR = 1 << 6;
	//GPIOC->BRR = 1 << 7;
	GPIO_WriteBit(GPIOC , GPIO_Pin_7 , 0);
	
	i = 0xfffff;
	while(--i); // do nothing for a bit while led is on


	// reset led 
	GPIOC->BRR = 1 << 6; // green
	//GPIOC->BSRR = 1 << 7; // yellow
	GPIO_WriteBit(GPIOC , GPIO_Pin_7 , 1);

	i = 0xfffff;
	while(--i); // do nothing for a bit while led is off

*/
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
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


