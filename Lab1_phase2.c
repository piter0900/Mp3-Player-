/*
 * Lab1_phase2.c
 *
 *  Created on: 2014-10-14
 *      Author: y22lai
 */


#include "alt_types.h"  // define types used by Altera code, e.g. alt_u8
#include <stdio.h>
#include <unistd.h>
#include "system.h"  // constants such as the base addresses of any PIOs
#include "sys/alt_irq.h"  // required when using interrupts
#include <io.h>

alt_u32 previous_state = 0x00;
volatile int pulse_count = 0;
int pulse_intr = 0;
alt_u16 timerPeriod;
alt_u32 data;
//
//void polling(){
//	if(pulse_count < 100){
//		data = IORD(PIO_PULSE_BASE, 0);
//		if( data == previous_state){
//
//			IOWR(PIO_RESPONSE_BASE, 0, data);
//			pulse_count++;
//			previous_state = IORD(PIO_PULSE_BASE, 0);
//
//		}
//
//
//	}
//
//}

//void TIMER_ISR() {
//
//
//	polling();
//	IOWR(TIMER_0_BASE, 0, 0x0);
//}

static void pulse_ISR(void* context, alt_u32 id) {
	pulse_intr++;
	data = IORD(PIO_PULSE_BASE, 0);
	IOWR(PIO_RESPONSE_BASE, 0, data);
	IOWR(PIO_PULSE_BASE, 3, 0x0);


}








// This function performs a specified number of calculations
// and returns the result of the calculation. For the purpose
// of this lab, the return value can be safely ignored.







// our test


int main(void)
{
//	timerPeriod =   0.0005 * TIMER_0_FREQ;
//	alt_irq_register(TIMER_0_IRQ, (void*)0, TIMER_ISR);
//	IOWR(TIMER_0_BASE, 2, (alt_u16)timerPeriod);
//	IOWR(TIMER_0_BASE, 3, (alt_u16)(timerPeriod >> 16));
//	IOWR(TIMER_0_BASE, 0, 0x0);
//	IOWR(TIMER_0_BASE,1,7);//timer

	alt_irq_register(PIO_PULSE_IRQ, (void*)0, pulse_ISR);
	IOWR(PIO_PULSE_BASE, 2, 0xf);
	IOWR(PIO_PULSE_BASE, 3, 0x0);//pulse






	int i;
	int x ;
	for(i = 3; i < 13; i++){
		for(x = 3; x < 13; x++){
			init(i, x);
			while(pulse_intr < 100)
			{
				background(20);

			}

			pulse_intr = 0;
//			while(pulse_count < 100)
//			{
//				background(20);
//
//			}
//			pulse_count = 0;
			finalize(x);

//			printf("a = %d", a);
//			printf("i = %d", i);
//			printf("x = %d", x);
		}

	}

}
