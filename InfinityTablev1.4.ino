#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=418.20001220703125,441
AudioAnalyzeFFT1024      LeftChannelFFT;          //xy=651.2000122070312,414
AudioAnalyzeFFT1024      RightChannelFFT;      //xy=655.1999969482422,454.1999969482422
AudioConnection          patchCord1(i2s1, 0, LeftChannelFFT, 0);
AudioConnection          patchCord2(i2s1, 1, RightChannelFFT, 0);
AudioControlSGTL5000     audioShield;    //xy=375.20001220703125,276.00001525878906
// GUItool: end automatically generated code

FASTLED_USING_NAMESPACE
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    8
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    176
CRGB leds[NUM_LEDS];
#define BRIGHTNESS          100
#define FRAMES_PER_SECOND   120

// An array to hold the 16 frequency bands
float LeftSpectrum[16];
float RightSpectrum[16];

void setup() {
  delay(3000); // 3 second delay for recovery
  AudioMemory(12);
  Serial.begin(9600);
  audioShield.enable();
  audioShield.volume(1);

  // Configure the window algorithm to use
  LeftChannelFFT.windowFunction(AudioWindowHanning1024);
  RightChannelFFT.windowFunction(AudioWindowHanning1024);
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  float n=0, m=0, x=0;
  int i=0,j=0, k=0, l=0, q=0;

  if (LeftChannelFFT.available()) {
    // each time new FFT data is available
    LeftSpectrum[0] =  LeftChannelFFT.read(0);
    LeftSpectrum[1] =  LeftChannelFFT.read(1);
    LeftSpectrum[2] =  LeftChannelFFT.read(2, 3);
    LeftSpectrum[3] =  LeftChannelFFT.read(4, 6);
    LeftSpectrum[4] =  LeftChannelFFT.read(7, 10);
    LeftSpectrum[5] =  LeftChannelFFT.read(11, 15);
    LeftSpectrum[6] =  LeftChannelFFT.read(16, 22);
    LeftSpectrum[7] =  LeftChannelFFT.read(23, 32);
    LeftSpectrum[8] =  LeftChannelFFT.read(33, 46);
    LeftSpectrum[9] =  LeftChannelFFT.read(47, 66);
    LeftSpectrum[10] = LeftChannelFFT.read(67, 93);
    LeftSpectrum[11] = LeftChannelFFT.read(94, 131);
    LeftSpectrum[12] = LeftChannelFFT.read(132, 184);
    LeftSpectrum[13] = LeftChannelFFT.read(185, 257);
    LeftSpectrum[14] = LeftChannelFFT.read(258, 359);
    LeftSpectrum[15] = LeftChannelFFT.read(360, 511);
    
  //for the number of LEDs used in the Left spectrum display, increment by 4 (loop 15 times)
   for ( i=115; i<=172; i=i+4 ) {
   n = LeftSpectrum[l] * 7;
   m=n*255;
   
   //Choose the colour depending on the value it falls between.
   if (m > 255) {q=0;}
   else if (255>m && m>200) {q=64;}
   else {q=96;}
   
   //for 4 leds in a row set the led value
   for ( j=0; j<4; j++ ) {
      k=i+j;
      leds[k].setHSV( q, 255, m);
   }
   l++;   
   Serial.print(m); 
   Serial.print("\t"); 
  }
  l=0;
  Serial.println();
  }

  //reset variables
  n=0, m=0, x=0;
  i=0,j=0, k=0, l=0, q=0;

 if (RightChannelFFT.available()) {
    // each time new FFT data is available
    RightSpectrum[0] =  RightChannelFFT.read(0);
    RightSpectrum[1] =  RightChannelFFT.read(1);
    RightSpectrum[2] =  RightChannelFFT.read(2, 3);
    RightSpectrum[3] =  RightChannelFFT.read(4, 6);
    RightSpectrum[4] =  RightChannelFFT.read(7, 10);
    RightSpectrum[5] =  RightChannelFFT.read(11, 15);
    RightSpectrum[6] =  RightChannelFFT.read(16, 22);
    RightSpectrum[7] =  RightChannelFFT.read(23, 32);
    RightSpectrum[8] =  RightChannelFFT.read(33, 46);
    RightSpectrum[9] =  RightChannelFFT.read(47, 66);
    RightSpectrum[10] = RightChannelFFT.read(67, 93);
    RightSpectrum[11] = RightChannelFFT.read(94, 131);
    RightSpectrum[12] = RightChannelFFT.read(132, 184);
    RightSpectrum[13] = RightChannelFFT.read(185, 257);
    RightSpectrum[14] = RightChannelFFT.read(258, 359);
    RightSpectrum[15] = RightChannelFFT.read(360, 511);

   //for the number of LEDs used in the Right spectrum display, increment by 4 (loop 15 times)
   for ( i=88; i>=27; i=i-4 ) {
   n = RightSpectrum[l] * 7;
   m=n*255;

   //Choose the colour depending on the value it falls between.
   if (m > 255) {q=0;}
   else if (255>m && m>200) {q=64;}
   else {q=96;}

   //for 4 leds in a row set the led value
   for ( j=0; j<4; j++ ) {
      k=i-j;
      leds[k].setHSV( q, 255, m);
   }
   l++;   
   Serial.print(m); 
   Serial.print("\t"); 
  }
  Serial.println();
  }
    // send the 'leds' array out to the actual LED strip
  Serial.print(" cpu:");
  Serial.println(AudioProcessorUsageMax());
  FastLED.show(); 
  // insert a delay to keep the framerate modest
  //FastLED.delay(1000/FRAMES_PER_SECOND); 
}


