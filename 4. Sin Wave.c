#include "L138_LCDK_aic3106_init.h"
#include "math.h"

#define SAMP_FREC 8000 // Frecuencia de muestreo
#define PI 3.141592

float frecuencia = 250.0; // Frecuencia del seno a generar
float amplitud = 20000.0; // Amplitud de la señal
float theta = 0.0; // Valor entre -2*PI y 2*PI
float theta_inc; // Incrementos de la señal de salida
uint8_t switches;

interrupt void interrupt4(void)
{
	switches = read_LCDK_user_DIP();
	// Se activa con 0, no con 1, depende de la placa
	if((switches & 0x01) == 0) { // Switch 5
		frecuencia = 500.0;
	} else {
		if((switches & 0x02) == 0 ) { // Switch 6
			frecuencia = 750.0;
		} else {
			if((switches & 0x04) == 0){ // Switch 7
				frecuencia = 1500.0;
			} else {
				if((switches & 0x08) == 0){ // Switch 8
					frecuencia = 2000.0;
				} else { // Ningún switch
					frecuencia = 250.0;
				}
			}
		}
	}

	theta_inc = 2*PI*frecuencia/SAMP_FREC;
	theta += theta_inc;
	if (theta > 2*PI) theta = -2*PI;
	output_left_sample((int16_t) amplitud*sin(theta));
	return;
}

int main(void)
{
	L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
	while(1);
}
