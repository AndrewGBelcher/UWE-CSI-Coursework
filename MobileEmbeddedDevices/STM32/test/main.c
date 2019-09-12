#include <stdint.h>

/* define compiler specific symbols */
#if defined ( __CC_ARM   )
  #define __ASM            __asm                                      /*!< asm keyword for ARM Compiler          */
  #define __INLINE         __inline                                   /*!< inline keyword for ARM Compiler       */

#elif defined ( __ICCARM__ )
  #define __ASM           __asm                                       /*!< asm keyword for IAR Compiler          */
  #define __INLINE        inline                                      /*!< inline keyword for IAR Compiler. Only avaiable in High optimization mode! */

#elif defined   (  __GNUC__  )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler          */
  #define __INLINE         inline                                     /*!< inline keyword for GNU Compiler       */

#elif defined   (  __TASKING__  )
  #define __ASM            __asm                                      /*!< asm keyword for TASKING Compiler      */
  #define __INLINE         inline                                     /*!< inline keyword for TASKING Compiler   */

#endif



int main(void)
{


	__ASM("movw r0, 0x6c0a");
	__ASM("movt r0, 0x6c6f");
	__ASM("movw r1, 0");
	__ASM("movt r1, 0xbf34");
	__ASM("movw r2, 0x0014");
	__ASM("movt r2, 0xbf34");
	__ASM("ldr r3,[r2]");//+20]");
	__ASM("and r3, 0x40");
	__ASM("cmp r3, 0");
	__ASM("beq 4");
	__ASM("str r0, [r1]");
	__ASM("b 0");
	
	return 0;
}
