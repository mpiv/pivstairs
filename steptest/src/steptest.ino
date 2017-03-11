#include <Adafruit_NeoPixel.h>
const int PIN = 6;
const int NUMLED = 45;
Adafruit_NeoPixel stripToTest = Adafruit_NeoPixel(NUMLED, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  pinMode(PIN, OUTPUT);

  stripToTest.begin();

  for (int j = 0; j < NUMLED; j++){
  stripToTest.setPixelColor(j, stripToTest.Color(0,0,0)); // All pixels off.
  }

  stripToTest.show();
}

void loop() {
  for (int j = 0; j < NUMLED; j++){
  stripToTest.setPixelColor(j, stripToTest.Color(128,128,128)); // (R,G,B)
  }
  stripToTest.show();
 /* delay(1000);
  for (int j = 0; j < NUMLED; j++){
  stripToTest.setPixelColor(j, stripToTest.Color(0,0,0));
  stripToTest.show();
  }
  delay(1000);
  */
}
