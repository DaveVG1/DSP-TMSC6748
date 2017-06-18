// L138_loop_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#define BUFFER_SIZE 22050
int16_t sampleIn, sampleOut;
int16_t buffer[BUFFER_SIZE];
#pragma DATA_SECTION(buffer, ".EXT_RAM")
uint16_t posBuffer = 0;
uint16_t i;

interrupt void interrupt4(void)  // interrupt service routine
{
	sampleIn = input_right_sample(); // Muestra de entrada
	buffer[posBuffer] = sampleIn + (buffer[posBuffer] >> 1); // Se almacena en el buffer
	posBuffer = (posBuffer + 1) % BUFFER_SIZE; // Se avanza una posición en el
	// buffer
	sampleOut = buffer[posBuffer]; // Se toma la muestra más antigua
	output_right_sample(sampleOut); // Se saca la muestra más antigua
	return;
}

int main(void)
{
	for(i=0;i<BUFFER_SIZE;i++){
		buffer[i] = 0;
	}
  L138_initialise_intr(FS_44100_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
