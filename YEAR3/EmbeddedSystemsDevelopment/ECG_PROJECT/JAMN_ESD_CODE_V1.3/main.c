/**
 * @file main.c
 * @author Andrew Belcher
 * @date 17 April 2020
 * @brief Source file for the core ECG program for the MSP430FR5969
 */

#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <adc.h>
#include <timer.h>
#include <buttons.h>
#include <leds.h>
#include <serial.h>
#include <event_manager.h>
#include <leds.h>
#include <watchdog.h>
#include <boot.h>
#include <Display/display.h>
#include <Events/event_types.h>
#include <Events/Event/event.h>
#include <Events/EventQueue/event_queue.h>
#include <Events/EventQueueManager/event_queue_manager.h>
#include <Graph/graph.h>
#include <Options/menu.h>
#include <Sharp96/sharp96.h>

#define BPM_UPPER_THRESHOLD 2800

struct Event_Queue_Manager* manager;
struct Event_Queue* q1;
struct Event e;
struct Display display;
struct Graph graph;
struct Menu menu;

#pragma PERSISTENT(fram_test_buffer)
extern unsigned char fram_test_buffer[4] = {0};

char adcstr[5];
char start_reading = 0;


/** @brief Event queue manager
 *  Needs use in the main loop for the device, upon pushed event into the event
 *  queue, this function will check and clear the queue depending on the event type
 *
 *  @param void
 *  @return void
 */
void event_manager(void){

    char bpmstr[5];
    char adcstr[5];
    int y;
    int x;

    // if queue isnt empty, attempt to clear it
    while(!event_queue_empty(q1)){

        // point to queue process it
        e = event_queue_get(q1);

        switch(e.event_id){

            // Button 0 press event
            case EVENT_BUTTON0_PRESS:

                if(display.mode == DISPLAY_MODE_HOME){

                  display.mode = DISPLAY_MODE_MENU;

                  menu.menu_index = 0;

                  menu_draw_screen(&menu, &display);

                  P1OUT &= ~0x01;
                  P4OUT &= ~0x40;
                }

                else if(display.mode == DISPLAY_MODE_MENU){

                    menu.menu_index++;

                    if(menu.menu_index >= NUM_OF_OPTIONS){

                        menu.menu_index = 0;
                    }

                    menu_draw_screen(&menu, &display);
                }

                break;

            // Button 1 press event
            case EVENT_BUTTON1_PRESS:


                if((display.mode == DISPLAY_MODE_MENU) &&
                   (menu.menu_index == (NUM_OF_OPTIONS-1))){

                    display.mode = DISPLAY_MODE_HOME;

                    display_init(&display);

                    display_draw_home_screen(&display);

                    sharp96_output_display(&display);

                    P1OUT &= ~0x01;
                    P4OUT &= ~0x40;
                }

                else if(display.mode == DISPLAY_MODE_PULSE){

                    display.mode = DISPLAY_MODE_HOME;

                    display_init(&display);

                    display_draw_home_screen(&display);

                    sharp96_output_display(&display);

                    P1OUT &= ~0x01;
                    P4OUT &= ~0x40;
                }

                else if(display.mode == DISPLAY_MODE_MENU){

                    display_init(&display);

                    if(menu.menu_index == 0){

                        menu.toggle_opt[0] ^= 1;
                    }

                    else if(menu.menu_index == 1){

                        menu.toggle_opt[1] ^= 1;
                    }

                    else if(menu.menu_index == 2){

                        if(menu.inc_opt[0] == 90){

                            menu.inc_opt[0] = 99;
                        }

                        else{

                            if(menu.inc_opt[0] == 99){

                                menu.inc_opt[0] = 0;
                            }

                            else{

                                menu.inc_opt[0] += 10;
                                menu.inc_opt[0] = menu.inc_opt[0] % 100;
                            }
                        }
                    }

                    menu_draw_screen(&menu, &display);

                    sharp96_output_display(&display);
                }

                else if(display.mode == DISPLAY_MODE_HOME){

                    P1OUT |= 0x01;
                    P4OUT &=  ~0x40;                 // Set P4.6 off (Red LED)

                    display.mode = DISPLAY_MODE_PULSE;

                    display_init(&display);

                    for(y = 1; y < 26; y++){

                        display_set_pixel(&display, 45, y);
                    }

                    for(y = 1; y < 26; y++){

                        display_set_pixel(&display, 94, y);
                    }

                    for(x = 45; x < 95; x++){

                        display_set_pixel(&display, x, 26);
                    }

                    for(x = 45; x < 95; x++){

                        display_set_pixel(&display, x, 1);
                    }

                    display_draw_text(&display, "BPM", 8, 3, 0);
                    display_draw_text(&display, "---", 8, 15, 0);

                }

                break;


            // Serial data received event
            case EVENT_SERIAL_RECV_BYTE:
                break;

            case EVENT_ADC_DONE_READING:

                if(display.mode == DISPLAY_MODE_PULSE){

                    sprintf(adcstr, "%d\n", adc_val);

                    serial_write_str(adcstr);

                    if(adc_val > BPM_UPPER_THRESHOLD){

                         if(first_beat == 0){

                             first_beat = 1;
                         }

                         else if(first_beat == 1){

                             second_beat = 1;
                         }

                         if(old_reading_count < 10){

                             sprintf(bpmstr, "%d", bpm);
                         }

                         else{

                             sprintf(bpmstr, "X_X");
                         }

                         display_draw_text(&display, "   ", 8, 15, 0);
                         display_draw_text(&display, bpmstr, 8, 15, 0);
                         display_draw_heart(&display, 0, 6, 3);

                         P4OUT |=  0x40;                 // Set P4.6 off (Red LED)
                    }

                    else{

                        display_draw_heart(&display, 1, 6, 3);

                        P4OUT &=  ~0x40;                 // Set P4.6 off (Red LED)
                    }

                    graph_add_reading(&graph, adc_val%50);

                    if(old_reading_count < 10){

                        if(menu.toggle_opt[1]){
                            display_draw_graph(&display, &graph);
                        }
                    }

                    else{

                        display_graph_init(&display);
                        display_draw_text(&display, "Connect", 3, 45, 0);
                        display_draw_text(&display, "probes!", 3, 58, 0);

                    }

                    sharp96_output_display(&display);
                }

                break;

            default:

                /* catch incorrect event ids and do nothing */
                break;

        }
    }
}

/** @brief Validation of fram read/write functionality
 *  For power on self test needs
 *
 *  @param void
 *  @return void
 */
void validate_fram(void){

    int i = 0;

    // test fram I/O with 00s

    P1OUT |=  0x01;                 // Set P1.0 on  (Green LED)
    P4OUT &= ~0x40;                 // Set P4.6 off (Red LED)

    for(i = 0; i < 4; i++){

        fram_test_buffer[i] = 0x00;

        if(fram_test_buffer[i] != 0x00){

            // infloop to lock leds
            while(1);
        }
    }

    // test fram I/O with ffs

    P1OUT &=  ~0x01;                 // Set P1.0 on  (Green LED)
    P4OUT |=  0x40;                 // Set P4.6 off (Red LED)

    for(i = 0; i < 4; i++)
    {
        fram_test_buffer[i] = 0xff;

        if(fram_test_buffer[i] != 0xff){

            // infloop to lock leds
            while(1);
        }
    }

    // test fram I/O with AAs

    P1OUT |=  0x01;                 // Set P1.0 on  (Green LED)
    P4OUT &= ~0x40;                 // Set P4.6 off (Red LED)

    for(i = 0; i < 4; i++){

        fram_test_buffer[i] = 0xaa;

        if(fram_test_buffer[i] != 0xaa){

            // infloop to lock leds
            while(1);
        }
    }

    // test fram I/O with 55s

    P1OUT &=  ~0x01;                 // Set P1.0 off  (Green LED)
    P4OUT |=  0x40;                 // Set P4.6 on (Red LED)

    for(i = 0; i < 4; i++){

        fram_test_buffer[i] = 0x55;

        if(fram_test_buffer[i] != 0x55){

            // infloop to lock leds
            while(1);
        }
    }
}

/** @brief Validation of branch instructions
 *  For power on self test needs
 *
 *  @param void
 *  @return void
 */
void validate_branches(void)
{
    // test branch opcodes
    // previous post tests worked on jz/jnz so we dont need to test them

    // jlo jump if lower
    __asm(
         "  setc \n"
         "loop8: \n"
         "  jlo loop8 \n"
         "  clrc \n"
        );

    // jhs jump if higher
    __asm(
         "  clrc \n"
         "loop9: \n"
         "  jhs loop9 \n"
        );

    // jn jump if negative
    __asm(
         "  clrn \n"
         "loop10: \n"
         "  jn loop10 \n"
        );


    // jge jump if greater
    __asm(
         "  mov.w #0aaaah, R15 \n"
         "  cmp.w #05555h, R15 \n"
         "loop11: \n"
         "  jge loop11 \n"
        );

    // jl jump if less
    __asm(
         "  mov.w #05555h, R15 \n"
         "  cmp.w #0aaaah, R15 \n"
         "loop12: \n"
         "  jl loop12 \n"
         "  setc \n"
        );

    // jmp jump (unconditional)
    __asm(
         "  mov.w #00000h, R15 \n"
         "loop13: \n"
         "  cmp.w #00001h, R15 \n"
         "  jz  loop14 \n"
         "  add.w #1, R15 \n"
         "  jmp loop13 \n"
         "loop14: \n"
         "  mov.w #00000h, R15 \n"
        );

}




/** @brief initializes the 8MHz system clock
 *
 *  @param void
 *  @return void
 */
void clock_init(void){

    // Clock System Setup
    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
    CSCTL0_H = 0;                             // Lock CS registers
}

/** @brief Core ECG program for the MSP430FR5969
 *
 *  @param void
 *  @return void
 */
void main(void){

    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // Keep port configurations on hwreset

    // Jump to runtime section if restarting and skip init
    if (restarting == 1){

        goto restart_here;
    }

    /*****************************************************
     *      POST SECTION
     ****************************************************/

    // validate that ram works by writing 00s to each address in its mapping

    P1DIR |=  0x01;                 // Set P1.0 to output direction
    P4DIR |=  0x40;                 // Set P4.6 to output direction

    P1OUT &= ~0x01;                 // Set P1.0 off (Green LED)
    P4OUT &= ~0x40;                 // Set P4.6 off (Red LED)
    P4OUT |=  0x40;                 // Set P4.6 on  (Red LED)

    __asm(
         "  mov.w #1C00h, R12 \n"
         "loop1: \n"
         "  mov.w #0000h, 0(R12) \n"
         "  cmp.w #0000h, 0(R12) \n"
         "  jnz loop1 \n"
         "  add.w #2, R12 \n"
         "  cmp.w #2400h, R12 \n"
         "  jz loop1 \n"
        );

    // validate that ram works by writing ffs to each address in its mapping

    P1OUT |=  0x01;                 // Set P1.0 on  (Green LED)
    P4OUT &= ~0x40;                 // Set P4.6 off (Red LED)


    __asm(
         "  mov.w #1C00h, R12 \n"
         "loop2: \n"
         "  mov.w #0ffffh, 0(R12) \n"
         "  cmp.w #0ffffh, 0(R12) \n"
         "      jnz loop1 \n"
         "  add #2, R12 \n"
         "  cmp.w #2400h, R12 \n"
         "      jz loop2 \n"
            );

    // validate that ram works by writing 55s to each address in its mapping

    P1OUT &= ~0x01;                 // Set P1.0 off (Green LED)
    P4OUT |=  0x40;                 // Set P4.6 on  (Red LED)

    asm(
         "      mov.w #1C00h, R12 \n"
         "loop3: \n"
         "      mov.w #05555h, 0(R12) \n"
         "      cmp.w #05555h, 0(R12) \n"
         "      jnz loop3 \n"
         "      add #2, R12 \n"
         "      cmp.w #2400h, R12 \n"
         "      jz loop3 \n"
        );


    // validate that ram works by writing aas to each address in its mapping

    P1OUT |=  0x01;                 // Set P1.0 on  (Green LED)
    P4OUT &= ~0x40;                 // Set P4.6 off (Red LED)

    asm(
         "      mov.w #1C00h, R12 \n"
         "loop4: \n"
         "      mov.w #0aaaah, 0(R12) \n"
         "      cmp.w #0aaaah, 0(R12) \n"
         "      jnz loop4 \n"
         "      add #2, R12 \n"
         "      cmp.w #2400h, R12 \n"
         "      jz loop4 \n"
        );



    // write ram with 0s and check if any corruption has occured

    P1OUT &= ~0x01;                 // Set P1.0 off (Green LED)
    P4OUT |=  0x40;                 // Set P4.6 on  (Red LED)

    asm(
         "      mov.w #1C00h, R12 \n"
         "loop5: \n"
         "      mov.w R12, 0(R12) \n"
         "      add #2, R12 \n"
         "      cmp.w #2400h, R12 \n"
         "      jz loop5 \n"

         "      mov.w #1C00h, R12 \n"
         "loop6: \n"
         "      cmp.w R12, 0(R12) \n"
         "      jnz loop6 \n"
         "      add #2, R12 \n"
         "      cmp.w #2400h, R12 \n"
         "      jz loop6 \n"
        );

    // write ram with 0s again...

    P1OUT |=  0x01;                 // Set P1.0 on  (Green LED)
    P4OUT &= ~0x40;                 // Set P4.6 off (Red LED)

    asm(
         "      mov.w #1C00h, R12 \n"
         "loop7: \n"
         "      mov.w #0000h, 0(R12) \n"
         "      add #2, R12 \n"
         "      cmp.w #2400h, R12 \n"
         "      jz loop7 \n"
        );

    P1OUT |=  0x01;                 // Set P1.0 on  (Green LED)
    P4OUT |=  0x40;                 // Set P4.6 off (Red LED)


    // now that the stack should work, we can run some post tests in functions
    validate_fram();
    validate_branches();

    // turn off our status leds

    P1OUT &= ~0x01;                 // Set P1.0 off (Green LED)
    P4OUT &= ~0x40;                 // Set P4.6 off (Red LED)


    /*****************************************************
     *      INITIALISATION SECTION
     ****************************************************/

    // Start the watchdog and configure its timer interrupt
    WDTCTL = WDTPW | WDTSSEL__SMCLK | WDTTMSEL | WDTCNTCL | WDTIS_1;

    // Initialize various modules
    timer_init();
    clock_init();
    sharp96_init();
    display_init(&display);
    serial_init();
    leds_init();
    adc_init();
    buttons_init();

    // Enable WDT interrupt
    SFRIE1 |= WDTIE;

    // Enable global interrupts
    _BIS_SR (GIE);

    /*****************************************************
     *      RUNTIME SECTION
     ****************************************************/

    restart_here:

    // Enable the Watchdog Timer
    WDTCTL = WDTPW + WDTCNTCL + 3;

    // Re-set the stack pointer
    asm(
         "      mov.w #2400h, SP \n"
    );

    // setup graph module
    graph = graph_create();
    graph_set_bounds(&graph, 0, 30);

    // draw boot animation
    display_init(&display);
    boot_animation(&display);

    // setup the event queue and its manager
    manager = event_queue_manager_create();
    q1 = event_queue_create();
    event_queue_manager_register_listener(manager, q1);

    // draw the home screen
    display_init(&display);
    display.mode = DISPLAY_MODE_HOME;
    display_draw_home_screen(&display);
    sharp96_output_display(&display);

    menu.toggle_opt[1] = 1;


    // The main runtime loop
    while(1){

        //Kick the watchdog
        WDTCTL = WDTPW + WDTCNTCL + 3;

        // Dispatch and service any events
        event_queue_manager_dispatch(manager);
        event_manager();
    }
}

