// L138_loop_intr.c
//

#include "L138_LCDK_aic3106_init.h"

  uint32_t sampleIn; // Full input sample
  int16_t lSampleIn; // Left input channel
  int16_t rSampleIn; // Right input channgel
  uint32_t sampleOut; // Full output sample

interrupt void interrupt4(void)  // interrupt service routine
{
  sampleIn = input_sample(); // read L + R samples from ADC
  lSampleIn = sampleIn >> 16;
  rSampleIn = sampleIn;
  // Cambiamos los canales
  sampleOut = (uint32_t) rSampleIn;
  sampleOut = sampleOut << 16;
  sampleOut = sampleOut | (uint32_t) lSampleIn;

  output_sample(sampleOut);   // write L + R samples to DAC
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
