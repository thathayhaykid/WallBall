/* 
Filename: KnockFlashLED Strip
Author: Haythem Mansour
Date: 03/23/2025
Version: 1.0.0

Description:
Using the Knock example + flashing light test from youtube, when we knock the LEDs will light up
See "Research" page in google docs, to be transfered to MD in this repository at the end of this project for links to what helped me accomplish this
*/

//External Libraries
#include <FastLED.h>

// Constants for single piezo sensor
const int knockSensor = A0;
const int threshold = 50;

// Constants for RGB Light Strip
const int NUM_LEDS = 60*5;
const int DATA_PIN = 9;
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
const int BRIGHTNESS = 60;
const int VOLTS = 5;
const int MAX_AMPS = 500;

CRGB leds[NUM_LEDS];

// these variables will change:
int sensorReading = 0;

void setup() {
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  
  Serial.begin(115200);       // use the serial port
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  sensorReading = analogRead(knockSensor);

  // if the sensor reading is greater than the threshold:
  if (sensorReading >= threshold) {
    // Flash the LED Strip if knocked
    for (int pixel = 0; pixel < NUM_LEDS; pixel++){
      leds[pixel] = CRGB(255, 0, 0);
    }
    FastLED.show();
    delay(100);
    FastLED.clear();
    FastLED.show();

    // send the string "Knock!" back to the computer, followed by newline
    Serial.print(sensorReading);
    Serial.println(" Knock!");
  }
  delay(100);  // delay to avoid overloading the serial port buffer
}
