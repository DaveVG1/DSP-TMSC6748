// L138_loop_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "coefs.h"


uint16_t posBuffer = 0;
uint16_t i, c, r;
int16_t sampleIn, sampleOut;
int16_t buffer[BL];
int32_t cuenta;
uint8_t switches;

interrupt void interrupt4(void)  // interrupt service routine
{
	sampleIn = input_right_sample(); // Muestra de entrada
	buffer[posBuffer] = sampleIn;
	posBuffer = (posBuffer + 1) % BL; // Se avanza una posición en el buffer

	switches = read_LCDK_user_DIP(); // Leemos el estado de los switches

	// Se activa con 0, no con 1, depende de la placa
	if((switches & 0x01) == 0) { // Switch 5
		cuenta = 0;
		r = posBuffer;
		for(c=0; c<BL; c++){ // Filtro
				cuenta = cuenta + (B[c] * buffer[r]);
				r = (r+1) % BL;
		}
		cuenta = cuenta >> 15;
		sampleOut = (int16_t) cuenta;
	} else {
		sampleOut = buffer[posBuffer];
	}
	output_right_sample(sampleOut);
	return;
}

int main(void)
{
	for(i=0;i<BL;i++){
		buffer[i] = 0;
	}
	L138_initialise_intr(FS_44100_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
	while(1);
}
