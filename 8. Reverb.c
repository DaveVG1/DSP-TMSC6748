// L138_loop_intr.c
//
#include <stdint.h>
#include "L138_LCDK_aic3106_init.h"


//uint16_t BL = 44100;
// uint16_t posBuffer = 0;
uint16_t posBufferD1 = 0;
uint16_t posBufferD2 = 0;
uint16_t posBufferD3 = 0;
uint16_t D1_max = 2205;
uint16_t D2_max = 4410;
uint16_t D3_max = 2205;
uint16_t i;
int16_t sampleIn, sampleOut;
//int16_t buffer[BL];
int16_t bufferD1[2205];
int16_t bufferD2[4410];
int16_t bufferD3[2205]; // El buffer 3 siempre son 50 ms
float G;
float cuenta;
float val;
uint8_t switches;
uint8_t newestado;
uint8_t estado = 4;

interrupt void interrupt4(void)  // interrupt service routine
{
	sampleIn = input_right_sample(); // Muestra de entrada

	switches = read_LCDK_user_DIP(); // Leemos el estado de los switches

	// Se activa con 0, no con 1, depende de la placa

	if((switches & 0x01) == 0)
		newestado = 0;
    else if((switches & 0x02) == 0)
    	newestado = 1;
	else if((switches & 0x04) == 0)
		 newestado = 2;

	else if((switches & 0x08) == 0)
		 newestado = 3;
	else
		 newestado = 4;
	if (estado != newestado)
	{
		posBufferD1 = 0;
		posBufferD2 = 0;
		posBufferD3 = 0;
		estado = newestado;
		switch(estado) {
		case 0: // Estudio
			D1_max = 882;
			D2_max = 1764;
			G = 0.422;
			break;
		case 1: // Auditorio
			D1_max = 1764;
			D2_max = 3528;
			G = 0.631;
			break;
		case 2: // Concierto
			D1_max = 1103;
			D2_max = 2205;
			G = 0.825;
			break;
		case 3: // Iglesia
			D1_max = 2205;
			D2_max = 4410;
			G = 0.858;
			break;
		}
	}

	if (estado == 4) {
	   output_right_sample(sampleIn);
	   return;
	}
	bufferD2[posBufferD2] = sampleIn;
	bufferD1[posBufferD1] = sampleIn;

	val = bufferD2[posBufferD2] + bufferD3[posBufferD3];
	bufferD3[posBufferD3] = (int16_t) (val * G);
	cuenta = (val + (float)sampleIn + (float)(bufferD1[posBufferD1]*G));

	if(cuenta > INT16_MAX)
		sampleOut = INT16_MAX;
	else if (cuenta < INT16_MIN)
		sampleOut = INT16_MIN;
	else
		sampleOut = (int16_t) (cuenta);

	posBufferD1 = (posBufferD1 + 1) % D1_max; // Se avanza una posición en el buffer
	posBufferD2 = (posBufferD2 + 1) % D2_max; // Se avanza una posición en el buffer
	posBufferD3 = (posBufferD3 + 1) % D3_max; // Se avanza una posición en el buffer

	output_right_sample(sampleOut);
	return;
}

int main(void)
{
	for(i=0;i<D1_max;i++){
		bufferD1[i] = 0;
	}
	for(i=0;i<D2_max;i++){
		bufferD2[i] = 0;
	}
	for(i=0;i<D3_max;i++){
		bufferD3[i] = 0;
	}
	L138_initialise_intr(FS_44100_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
	while(1);
}
