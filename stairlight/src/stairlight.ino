// Neopixel stair lighting sketch - 2016 - Manuel Pivert
// Inspired from NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

// We include the Neopixel library
#include <Adafruit_NeoPixel.h>

// The stair has NUMSTEPS steps
// There is one NeoPixel strip on each step
// As the steps do not have the same size, the number of NeoPixel leds is not the same on each step

// How many steps has the stair?
const int NUMSTEPS = 14;

// Let's define how many NeoPixel leds are on each strip
const int NUMLED[] = {49, 49, 53, 57, 49, 46, 45, 45, 45, 45, 45, 45, 45, 45};

// We connect each strip to different pins on the Arduino
// Let's define which pins will be used for each strip
const int PIN[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

// Let's declare our ledstrips
Adafruit_NeoPixel strip[14];


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
void initStrips() {
	for (int i = 0; i < NUMSTEPS; i++) {
	strip[i] = Adafruit_NeoPixel(NUMLED[i], PIN[i], NEO_GRB + NEO_KHZ800);
	}
}

void stepOn(int n) {
	for (int i = 0; i < NUMLED[n]; i++){
		// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  	strip[n].setPixelColor(i, strip[n].Color(64,64,96));
  	}
	strip[n].show();
}

void stepOff(int n) {
	for (int i = 0; i < NUMLED[n]; i++){
		// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  	strip[n].setPixelColor(i, strip[n].Color(0,0,0));
  	}
	strip[n].show();
}

// In this function we first switch on a step and then we wait a delay before we switch it off
// We need to pass the number of the step and the delay in ms
void stepOnFewInstants(int n, int delayval) {
	stepOn(n);
	delay(delayval);
	stepOff(n);
}

void setup() {

// Setting pins to OUTPUT mode
	for (int i = PIN[0]; i <= PIN[13]; i++) {
		pinMode(i, OUTPUT);
	}

// Starting to work with our ledstrips
	for (int i = 0; i < NUMSTEPS; i++) {
  	strip[i].begin();
  }

  for (int i = 0; i < NUMSTEPS; i++) {
		for (int j = 0; j < NUMLED[i]; j++){
    strip[i].setPixelColor(j, strip[i].Color(0,0,0)); // All pixels off.
		}
  }

	for (int i = 0; i < NUMSTEPS; i++) {
		strip[i].show(); // To actually see colors we previously set
	}

}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

}