/* 
Filename: KnockFlashLEDDebounced
Author: Haythem Mansour
Date: 03/26/2025
Version: 1.0.0

Description:
Using the Knock example + flashing light test from youtube, when we knock the LEDs will light up
This script takes the above, and translated it to using "debounce" method
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

// Big Numbers due to millis
unsigned long lastDebouncetime = 0;
unsigned long debounceDelay = 50;

int oneSecond = 1000;
unsigned long time_now = 0;


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
    // Check the last debounce time for noise or not
    lastDebounceTime = millis();
  }

  //
  if ((millis() - lastDebounceTime) > debounceDelay){

    if(sensorReading >=threshold) {
      for (int pixel = 0; pixel < NUM_LEDS; pixel++){
            leds[pixel] = CRGB(255, 0, 0);
          }

      FastLED.show(); 
      while(millis() < time_now + period) {
          FastLED.clear();
          FastLED.show();

          Serial.print(sensorReading);
          Serial.println(" Knock!");

        }
      
    }

  }

}


  
  
  }
