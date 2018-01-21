/*
Neopixel stair lighting sketch - 2016 - Manuel Pivert
Inspired from NeoPixel Ring simple sketch (c) 2013 Shae Erisson
released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
*/

// We include the Neopixel library
#include <Adafruit_NeoPixel.h>

/*
The stair has NUMSTEPS steps
There is one NeoPixel strip on each step
As the steps do not have the same size, the number of NeoPixel leds is not the same on each step
*/

// How many steps has the stair?
const uint8_t NUMSTEPS = 14;

// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
const uint32_t HIGH_R = 64; // 64
const uint16_t HIGH_G = 70; // 70
const uint8_t HIGH_B = 96; // 96
const uint32_t LOW_R = 16; // 16
const uint16_t LOW_G = 20; // 20
const uint8_t LOW_B = 24; // 24

// pixels.Color is 0,0,0 for the leds switched off
const uint8_t LED_OFF = 0;

// Let's define how many NeoPixel leds are on each strip
const uint8_t NUMLED[] = {49, 49, 53, 57, 49, 46, 45, 45, 45, 45, 45, 45, 45, 45};

/*
We connect each strip to different pins on the Arduino
Let's define which pins will be used for each strip
*/
const uint8_t PIN[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};

// Let's define the input pins for PIR sensors
const uint8_t topPirPin = 2;
const uint8_t bottomPirPin = 3;

// Let's define the pins for luminosity reading
const uint8_t photoCellPin = 0xA0; // the cell and 10K pulldown are connected to a0

// Let's declare our ledstrips
Adafruit_NeoPixel strip[NUMSTEPS];

// Let's declare some variables
uint32_t lowBrightnessColor = 0;
uint32_t highBrightnessColor = 0;
uint16_t longDelay = 500;
uint16_t shortDelay = 70;
uint16_t arrivedUpDelay = 7000;
uint16_t wantToGoDownDelay = 7000;
uint16_t offDelay = 2000;
uint8_t upGoing = 0;
uint8_t downGoing = 0;
uint16_t photoCellReading = 0;
uint16_t lumValue = 450;

/*
When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
example for more information on possible values.
*/
void initStrips() {
	for (uint8_t i = 0; i < NUMSTEPS; i++) {
	strip[i] = Adafruit_NeoPixel(NUMLED[i], PIN[i], NEO_GRB + NEO_KHZ800);
	}
}

// We calculate a color 32 bit word based on R, G and B values
uint32_t calculateColor(uint8_t r, uint8_t g, uint8_t b) {
	uint32_t color;
	uint32_t redVal = (uint32_t)r << 16;
	uint16_t greenVal = (uint32_t)g << 8;
	uint8_t blueVal = (uint32_t)b << 0;
	color = redVal + greenVal + blueVal;
	return color;
}

// We use this function to convert from 32 bit word to 1 color value (r or g or b)
uint8_t splitColor(uint32_t color, char value) {
	switch ( value ) {
    case 'r': return (uint8_t)(color >> 16);
    case 'g': return (uint8_t)(color >>  8);
    case 'b': return (uint8_t)(color >>  0);
    default:  return 0;
  }
}

// We use this function to light the strip number n
void stepOn(uint8_t n, uint32_t ledColor) {
	for (uint8_t i = 0; i < NUMLED[n]; i++){
  	strip[n].setPixelColor(i, ledColor);
  	}
	strip[n].show();
}

// Same as above but with rgb values
void stepOnRGB(uint8_t n, uint8_t r, uint8_t g, uint8_t b) {
	for (uint8_t i = 0; i < NUMLED[n]; i++){
  	strip[n].setPixelColor(i, r, g, b);
  	}
	strip[n].show();
}

// We use this function to light the strip number n, but with style !
void leftSlideStepOn(uint8_t n, uint32_t ledColor) {
	for (uint8_t i = 0; i < NUMLED[n]; i++){
  	strip[n].setPixelColor(i, ledColor);
		strip[n].show();
		delay(2);
  	}
}

// We use this function to light the strip number n, but with style !
void rightSlideStepOn(uint8_t n, uint32_t ledColor) {
	for (uint8_t i = 0; i < NUMLED[n]; i++){
		uint8_t j = NUMLED[n] - 1 - i;
		strip[n].setPixelColor(j, ledColor);
		strip[n].show();
		delay(2);
  	}
}

// We use this function to turn off the strip number n
void stepOff(uint8_t n) {
	for (uint8_t i = 0; i < NUMLED[n]; i++){
  	strip[n].setPixelColor(i, LED_OFF);
  	}
	strip[n].show();
}

/*
Here is the code for the lighting animation,
one for going upstairs and one for going downstairs
for each we first light all the steps with low brightness
and then we light a group of 3 steps with high brightness
then we "move" this 3 steps group through the stair
at las we switch off all the steps
*/
void goUpAnim() {
	for (uint8_t i = 0; i < NUMSTEPS; i++) { // lighting all steps with "background" low brightness
		stepOn(i, lowBrightnessColor);
		delay(shortDelay);
	}
	for (uint8_t i = 0; i < 3; i++) { // lighting a group of 3 steps with high brightness
		stepOn(i, highBrightnessColor);
		//delay(longDelay);
	}
	delay(2 * longDelay);
	for (uint8_t i = 3; i < NUMSTEPS; i++) { // "moving" our 3 steps group through the stair
		stepOn(i, highBrightnessColor);
		uint8_t j = i - 3;
		stepOn(j, lowBrightnessColor);
		delay(longDelay);
	}
	delay(arrivedUpDelay);
	for (uint8_t i = NUMSTEPS - 3; i < NUMSTEPS; i++) { // terminating the "move" of the 3 steps group
		stepOn(i, lowBrightnessColor);
		delay(longDelay);
	}
	for (uint8_t i = 0; i < NUMSTEPS; i++) { // switching off all the steps
		stepOff(i);
		delay(shortDelay);
	}
}

// the same as above but moving downstairs !
void goDownAnim() {
	for (uint8_t i = 0; i < NUMSTEPS; i++) {
		uint8_t j = NUMSTEPS - 1 - i;
		stepOn(j, lowBrightnessColor);
		delay(shortDelay);
	}
	for (uint8_t i = 0; i < 3; i++) {
		uint8_t j = NUMSTEPS - 1 - i;
		stepOn(j, highBrightnessColor);
		//delay(longDelay);
	}
	delay(wantToGoDownDelay);
	for (uint8_t i = 3; i < NUMSTEPS; i++) {
		uint8_t j = NUMSTEPS - 1 - i;
		stepOn(j, highBrightnessColor);
		uint8_t k = j + 3;
		stepOn(k, lowBrightnessColor);
		delay(longDelay);
	}
	for (size_t i = NUMSTEPS - 3; i < NUMSTEPS; i++) {
		uint8_t j = NUMSTEPS - 1 - i;
		stepOn(j, lowBrightnessColor);
		delay(longDelay);
	}
	for (uint8_t i = 0; i < NUMSTEPS; i++) {
			uint8_t j = NUMSTEPS - 1 - i;
			stepOff(j);
			delay(shortDelay);
	}
}

void goUpStyleAnim() {
	for (uint8_t i = 0; i < NUMSTEPS; i++) { // lighting all steps with "background" low brightness
		stepOn(i, lowBrightnessColor);
		delay(shortDelay);
	}
	for (uint8_t i = 0; i < 3; i++) { // lighting a group of 3 steps with high brightness
		stepOn(i, highBrightnessColor);
		//delay(longDelay);
	}
	delay(2 * longDelay);
	for (uint8_t i = 3; i < NUMSTEPS; i++) { // "moving" our 3 steps group through the stair
		uint8_t j = i - 3;
		rightSlideStepOn(j, lowBrightnessColor);
		leftSlideStepOn(i, highBrightnessColor);
		delay(longDelay);
	}
	delay(arrivedUpDelay);
	for (uint8_t i = NUMSTEPS - 3; i < NUMSTEPS; i++) { // terminating the "move" of the 3 steps group
		rightSlideStepOn(i, lowBrightnessColor);
		delay(longDelay);
	}
	for (uint8_t i = 0; i < NUMSTEPS; i++) { // switching off all the steps
		stepOff(i);
		delay(shortDelay);
	}
}

// the same as above but moving downstairs !
void goDownStyleAnim() {
	for (uint8_t i = 0; i < NUMSTEPS; i++) {
		uint8_t j = NUMSTEPS - 1 - i;
		stepOn(j, lowBrightnessColor);
		delay(shortDelay);
	}
	for (uint8_t i = 0; i < 3; i++) {
		uint8_t j = NUMSTEPS - 1 - i;
		stepOn(j, highBrightnessColor);
		//delay(longDelay);
	}
	delay(wantToGoDownDelay);
	for (uint8_t i = 3; i < NUMSTEPS; i++) {
		uint8_t j = NUMSTEPS - 1 - i;
		uint8_t k = j + 3;
		rightSlideStepOn(k, lowBrightnessColor);
		leftSlideStepOn(j, highBrightnessColor);
		delay(longDelay);
	}
	for (size_t i = NUMSTEPS - 3; i < NUMSTEPS; i++) {
		uint8_t j = NUMSTEPS - 1 - i;
		rightSlideStepOn(j, lowBrightnessColor);
		delay(longDelay);
	}
	for (uint8_t i = 0; i < NUMSTEPS; i++) {
			uint8_t j = NUMSTEPS - 1 - i;
			stepOff(j);
			delay(shortDelay);
	}
}

void setup() {

// Setting pins to OUTPUT mode for led strips
	for (uint8_t i = PIN[0]; i <= PIN[13]; i++) {
		pinMode(i, OUTPUT);
	}

// Setting pins to INPUT mode for PIR sensors
	pinMode(topPirPin, INPUT);
	pinMode(bottomPirPin, INPUT);

// Starting to work with our ledstrips
initStrips();
	for (uint8_t i = 0; i < NUMSTEPS; i++) {
		strip[i].begin();
		for (uint8_t j = 0; j < NUMLED[i]; j++){
    	strip[i].setPixelColor(j, LED_OFF); // All pixels off.
			}
		strip[i].show(); // To actually see colors we previously set
	}
highBrightnessColor = calculateColor(HIGH_R, HIGH_G, HIGH_B);
lowBrightnessColor = calculateColor(LOW_R, LOW_G, LOW_B);
}

void loop() {
	photoCellReading = analogRead(photoCellPin);

	upGoing = digitalRead(bottomPirPin); // Read value of bottom PIR sensor
	if ((upGoing == HIGH) && (photoCellReading < lumValue)) { // if motion detected and low luminosity, launches the go up animation
		goUpStyleAnim();
		delay(offDelay);
	}
	downGoing = digitalRead(topPirPin); // Read value of top PIR sensor
	if ((downGoing == HIGH) && (photoCellReading < lumValue)) { // if motion detected and low luminosity, launches the go down animation
		goDownStyleAnim();
		delay(offDelay);
	}
	delay(shortDelay);
}
