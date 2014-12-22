// File: lab1.c
//
// Contents: Helper functions to complete lab 1

#include "alt_types.h"  // define types used by Altera code, e.g. alt_u8
#include <stdio.h>
#include <unistd.h>
#include "system.h"  // constants such as the base addresses of any PIOs
                     // defined in your hardware
#include "sys/alt_irq.h"  // required when using interrupts
#include <io.h>
// our test

alt_u8 led_state = (alt_u8)0x00;

#ifdef BUTTON_PIO_BASE
#ifdef LED_PIO_BASE
#ifdef GREEN_LED_PIO_BASE
#ifdef LED_PIO_BASE
alt_u8 switch1;
alt_u8 switch2;
 alt_u32 timerPeriod;
int counter1;
int counter2;
volatile int flag1 = 0;
volatile int flag2 = 0;
volatile int flag3 = 0;
alt_u8 buttons;

static void button_ISR(void* context, alt_u32 id)
{
/*	flag1 = 0;
	flag2 = 0;
	buttons = 0x00;
	IOWR(TIMER_0_BASE,1,7);
	switch1 = 0x00;
	
	counter = 0;


/*get value from edge capture register and mask off all bits except
the 4 least significant */


	buttons = IORD(BUTTON_PIO_BASE, 3) & 0xf;
	//led_state = led_state ^ buttons;


	//IOWR(LED_PIO_BASE, 0, switch1);



	if((buttons & 0x01) == 0x01){// leds
		flag1 = 1;
		switch1 = IORD(SWITCH_PIO_BASE, 0)& 0xff;
		counter1 = 0

	}

	if ((buttons & 0x02)  == 0x02)
	{
		flag2 = 1;
		switch1 = IORD(SWITCH_PIO_BASE, 0)& 0xff;
		v
	}


 /*any bits that are set in the edge capture register are used to toggle
the LED state */




/* write current state to LED PIO */
	//IOWR(LED_PIO_BASE, 0, led_state);



/* reset edge capture register to clear interrupt */
	IOWR(BUTTON_PIO_BASE, 3, 0x0);
	//printf(flag1);



}



static void TIMER_ISR(void* context, alt_u32 id)
{
// acknowledge the interrupt by clearing the TO bit in the status register
	if(flag1 == 1){
		if(switch1 & 0x1 == 0x1){
			
			IOWR(LED_PIO_BASE, 0, 0x01);
			counter1++;
			switch1 = switch1 >> 1;
			}
			

		else{	
		IOWR(LED_PIO_BASE, 0, 0x00);
				counter1++;
			switch1 = switch1 >> 1;
			}
	
		}
		
	if(flag2 == 1){
		if(switch2 & 0x1 == 0x1){
			
			IOWR(SEVEN_SEG_PIO_BASE,0, 0xCF );
			counter2++;
			}
			

		else{	
			IOWR(SEVEN_SEG_PIO_BASE,0, 0x81 );
			counter2++;
			
			}
	
		}

			
			
			if (counter1 == 8 ){

				counter1 =0;
				flag1 = 0;
			
			
			}
			if (counter2 == 8 ){

				counter2 =0;
				flag2 = 0;
				
			}


			//printf(counter);







// set the flag with a non zero value
//count_flag = 0xf;

		IOWR(TIMER_0_BASE, 0, 0x0);
}


int main(void)
{


	counter = 0;

	alt_irq_register( BUTTON_PIO_IRQ, (void*)0, button_ISR );
	timerPeriod = 1*TIMER_0_FREQ;
	/* reset the edge capture register by writing to it (any value will do) */
	IOWR(BUTTON_PIO_BASE, 3, 0x0);
	IOWR(BUTTON_PIO_BASE, 2, 0xf);
	IOWR(LED_PIO_BASE, 0, 0x00);
	alt_irq_register(TIMER_0_IRQ, (void*)0, TIMER_ISR);
	IOWR(TIMER_0_BASE, 2, (alt_u16)timerPeriod);
	IOWR(TIMER_0_BASE, 3, (alt_u16)(timerPeriod >> 16));

	IOWR(TIMER_0_BASE, 0, 0x0);

	IOWR(TIMER_0_BASE,1,7);




  /* output initial message */
  printf("\n nPush a button to turn on an LED\n");

while(1)
{

}

  return(0);
}

#endif
#endif
#endif
#endif

