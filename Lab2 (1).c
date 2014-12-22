#include "basic_io.h"
#include "LCD.h"
#include "SD_Card.h"
#include "fat.h"
#include "wm8731.h"
#include "alt_types.h"  // define types used by Altera code, e.g. alt_u8
#include <stdio.h>
#include <unistd.h>
#include "system.h"  // constants such as the base addresses of any PIOs
#include "sys/alt_irq.h"  // required when using interrupts
#include <io.h>
volatile data_file df;
BYTE buffer[512] = {0};
int chain[5000];
alt_u8 buttons;
int button0Pressed= 0;
alt_u8 switch1;
int NormalFlag = 0;
int HalfFlag = 0;
int ReverseFlag = 0;
int DoubleFlag = 0;
int Numoffiles = 0;

void Init(){
	if (SD_card_init() != 0){
		perror("init card1324");

	}
	if (init_mbr() != 0){
		perror("init mbr");

	}
	if (init_bs() != 0){
		perror("init bs");

	}

	init_audio_codec();
	LCD_Init();
}

static void button_ISR(void* context, alt_u32 id)
{



/*get value from edge capture register and mask off all bits except
the 4 least significant */


	buttons = IORD(BUTTON_PIO_BASE, 3) & 0xf;

	if((buttons & 0x01) == 0x01){// leds

		button0Pressed = 1;


	}

	if ((buttons & 0x02)  == 0x02)
	{
		button0Pressed = 0;
		switch1 = IORD(SWITCH_PIO_BASE, 0)& 0x07;

		if(switch1 == 0x07 || switch1 == 0x00||switch1 == 0x05||switch1 == 0x06){
	        NormalFlag = 1;
	        HalfFlag = 0;
	        ReverseFlag = 0;
	        DoubleFlag = 0;
	    }
	    //If DIP switches set to 3 play song in delay mode
	    if(switch1 == 0x01){
	        DoubleFlag = 1;
	        HalfFlag = 0;
	        NormalFlag = 0;
	        ReverseFlag = 0;
	    }
	    //If DIP switches set to 4 play song in reverse
	    if(switch1 == 0x02){
	        HalfFlag = 1;
	        NormalFlag = 0;
	        ReverseFlag = 0;
	        DoubleFlag = 0;
	    }
	    if(switch1 == 0x04){
	   	    ReverseFlag = 1;
	   		HalfFlag = 0;
	   		NormalFlag = 0;
	   		DoubleFlag = 0;
	   	}


	}
	if ((buttons & 0x04)  == 0x04)
	{
		switch1 = IORD(SWITCH_PIO_BASE, 0)& 0x07;
		search_for_filetype( "WAV", &df, 0,1 );

			LCD_Display(df.Name, switch1);





	}
	if ((buttons & 0x08)  == 0x08)
	{
		switch1 = IORD(SWITCH_PIO_BASE, 0)& 0x07;
		file_number = file_number -2;

		search_for_filetype( "WAV", &df, 0,1 );
			LCD_Display(df.Name, switch1);

	}


 /*any bits that are set in the edge capture register are used to toggle
the LED state */




/* write current state to LED PIO */
	//IOWR(LED_PIO_BASE, 0, led_state);



/* reset edge capture register to clear interrupt */
	IOWR(BUTTON_PIO_BASE, 3, 0x0);
	//printf(flag1);



}



void Normal(){

	printf("in normal");
		UINT16 tmp;

		int Byte = 0 ;
		int Sector = 0;
		UINT32 length = 1+ceil(df.FileSize/(BPB_BytsPerSec * BPB_SecPerClus));
		LCD_File_Buffering(df.Name);
		build_cluster_chain( chain,length, &df );
		LCD_Display(df.Name, switch1);
		while(get_rel_sector(&df, buffer,chain, Sector) != -1){

			while((Byte < 512) && (button0Pressed!= 1)){
				while(IORD( AUD_FULL_BASE, 0 ) ) {} //wait until the FIFO is not full
				tmp = ( buffer[ Byte + 1 ] << 8 ) | ( buffer[ Byte ] ); //Package 2 8-bit bytes from the
				//sector buffer array into the
				//single 16-bit variable tmp
				IOWR( AUDIO_0_BASE, 0, tmp ); //Write the 16-bit variable tmp to the FIFO where it
				//will be processed by the audio CODEC
				Byte = Byte+2;

			}
			Byte = 0;
			Sector++;

		}
		NormalFlag = 0;
}
void Double(){

	printf("in double");
		UINT16 tmp;

		int Byte = 0 ;
		int Sector = 0;
		UINT32 length = 1+ceil(df.FileSize/(BPB_BytsPerSec * BPB_SecPerClus));
		LCD_File_Buffering(df.Name);
		build_cluster_chain( chain,length, &df );
		LCD_Display(df.Name, switch1);
		while(get_rel_sector(&df, buffer,chain, Sector) != -1){
			while((Byte < 512) && (button0Pressed!= 1)){
				while(IORD( AUD_FULL_BASE, 0 ) ) {} //wait until the FIFO is not full
				tmp = ( buffer[ Byte + 1 ] << 8 ) | ( buffer[ Byte ] ); //Package 2 8-bit bytes from the
				//sector buffer array into the
				//single 16-bit variable tmp
				IOWR( AUDIO_0_BASE, 0, tmp ); //Write the 16-bit variable tmp to the FIFO where it
				//will be processed by the audio CODEC
				Byte = Byte+4;

			}
			Byte = 0;
			Sector++;

		}
		DoubleFlag = 0;
}
void Half(){
	printf("in half");
		UINT16 tmp;

		int Byte = 0 ;
		int Sector = 0;
		UINT32 length = 1+ceil(df.FileSize/(BPB_BytsPerSec * BPB_SecPerClus));
		LCD_File_Buffering(df.Name);
		build_cluster_chain( chain,length, &df );
		LCD_Display(df.Name, switch1);
		while(get_rel_sector(&df, buffer,chain, Sector) != -1){
			while((Byte < 512) && (button0Pressed!= 1)){
				while(IORD( AUD_FULL_BASE, 0 ) ) {} //wait until the FIFO is not full
				tmp = ( buffer[ Byte + 1 ] << 8 ) | ( buffer[ Byte ] ); //Package 2 8-bit bytes from the
				//sector buffer array into the
				//single 16-bit variable tmp
				IOWR( AUDIO_0_BASE, 0, tmp ); //Write the 16-bit variable tmp to the FIFO where it
				//will be processed by the audio CODEC
				IOWR( AUDIO_0_BASE, 0, tmp );
				Byte = Byte+2;

			}
			Byte = 0;
			Sector++;

		}
		HalfFlag = 0;

}

void Reverse(){
	printf("in reverse");

	UINT16 tmp;

		UINT32 length = 1+ceil(df.FileSize/(BPB_BytsPerSec * BPB_SecPerClus));
		LCD_File_Buffering(df.Name);
		build_cluster_chain( chain,length, &df );
		LCD_Display(df.Name, switch1);
		3

		int Byte = 0 ;

		UINT32 Sector = (ceil(df.FileSize/BPB_BytsPerSec))-2;
		while(get_rel_sector(&df, buffer,chain, Sector) != -1){
			while((Byte >=0) && (button0Pressed!= 1)){
				while(IORD( AUD_FULL_BASE, 0 ) ) {} //wait until the FIFO is not full
				tmp = ( buffer[ Byte -1   ] << 8 ) | ( buffer[ Byte  ] ); //Package 2 8-bit bytes from the
				//sector buffer array into the
				//single 16-bit variable tmp
				IOWR( AUDIO_0_BASE, 0, tmp ); //Write the 16-bit variable tmp to the FIFO where it
				//will be processed by the audio CODEC
				Byte = Byte-2;

			}
			Byte = 512;
			Sector--;

		}
		ReverseFlag = 0;
}



int main(void)
{
	alt_irq_register( BUTTON_PIO_IRQ, (void*)0, button_ISR );
	IOWR(BUTTON_PIO_BASE, 3, 0x0);
	IOWR(BUTTON_PIO_BASE, 2, 0xf);
	IOWR(LED_PIO_BASE, 0, 0x00);

	Init();



	 //display that it is buffering



	//Normal();

	while(1)
	{

		if(ReverseFlag == 1){Reverse();}
		if(NormalFlag == 1){Normal();}
		if(DoubleFlag == 1){Double();}
		if(HalfFlag == 1){Half();}
	}


	return 0;

	}


