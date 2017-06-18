#include "L138_LCDK_aic3106_init.h"
#include "math.h"

#define SAMP_FREC 8000 // Frecuencia de muestreo

int32_t frecuencia = 1000; // Frecuencia del seno a generar
int16_t amplitud_max = 20000; // Amplitud de la señal
int16_t amplitud_min = -20000; // Amplitud de la señal
int16_t contador = 0;

interrupt void interrupt4(void)
{
	if (contador < 4){
		output_right_sample(amplitud_max);
	} else {
		output_right_sample(amplitud_min);
	}
	contador = (contador+1) % (SAMP_FREC/frecuencia);
	return;
}

int main(void)
{
	L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
	while(1);
}
