// L138_loop_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "fdacoefslo.h"
#include "fdacoefshi.h"

int16_t buffer[BL];
uint16_t posBuffer = 0;
uint16_t i, c, c1, r, r1;
int16_t sampleIn, sampleOut;
float cuenta, cuenta1, cuentafinal;
uint8_t switches;

interrupt void interrupt4(void)  // interrupt service routine
{
	sampleIn = input_right_sample(); // Muestra de entrada
	buffer[posBuffer] = sampleIn;
	r = (posBuffer + 1) % BL; // Se avanza una posición en el buffer
	r1 = r;
	switches = read_LCDK_user_DIP(); // Leemos el estado de los switches

	// Bandas del EQ

	// Se activa con 0, no con 1, depende de la placa
	if(((switches & 0x01) == 0) && ((switches & 0x02) == 0)) {

		// Banda baja
		cuenta = 0.0;
		for(c=0; c<BL; c++){ // Filtro paso bajo
				cuenta = cuenta + (float) (BLo[c] * buffer[r]);
				r = (r+1) % BL;
		}
		cuenta = cuenta*2;

		// Banda alta
		cuenta1 = 0.0;
		for(c1=0; c1<BL; c1++){ // Filtro paso alto
				cuenta1 = cuenta1 + (float) (BHi[c1] * buffer[r1]);
				r1 = (r1+1) % BL;
		}
		cuenta1 = cuenta1*2;

		cuentafinal = cuenta1+cuenta;

		sampleOut = ((int16_t) cuentafinal); // Amplificamos ambas bandas
	} else if((switches & 0x01) == 0) { // Switch 5
		// Banda baja
		cuenta = 0.0;
		for(c=0; c<BL; c++){ // Filtro paso bajo
				cuenta = cuenta + (float) (BLo[c] * buffer[r]);
				r = (r+1) % BL;
		}
		cuenta = cuenta*2;
		sampleOut = ((int16_t) cuenta); // Amplificamos las frecuencias bajas
	} else if ((switches & 0x02) == 0){ // Switch 6
		// Banda alta
		cuenta1 = 0.0;
		for(c=0; c<BL; c++){ // Filtro paso alto
				cuenta1 = cuenta1 + (float) (BHi[c] * buffer[r]);
				r = (r+1) % BL;
		}
		cuenta1 = cuenta1*2;

		sampleOut = ((int16_t) cuenta1); // Amplificamos las frecuencias altas
	} else { // Ningún switch activado
		sampleOut = buffer[posBuffer]; // La salida es igual que la entrada
	}
	posBuffer = r;
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
