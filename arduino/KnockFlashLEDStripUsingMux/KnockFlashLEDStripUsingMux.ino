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
const int threshold = 500;

// Constants for RGB Light Strip
const int NUM_LEDS = 15;
const int DATA_PIN = 9;
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
const int BRIGHTNESS = 60;
const int VOLTS = 5;
const int MAX_AMPS = 500;

// Big Numbers due to millis
unsigned long lastDebouncetime = 0;
unsigned long debounceDelay = 10;

int oneSecond = 1000;
unsigned long time_now = 0;

CRGB leds[NUM_LEDS];

// Mux Control Pins
const int s0 = D10;
const int s1 = D11;
const int s2 = D12;

//Read Mux
const int muxInput = A0;

// Number of Piezos to read
const int numPiezo = 6;

// these variables will change:
int sensorReading = 0;

void setup() {
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  
  Serial.begin(115200);       // use the serial port

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
}
 
void loop() {
  // read the sensor and store it in the variable sensorReading:
  int sensorValues[numPiezo];

  for (int i = 0; i < numPiezo; i++){
    
    sensorReading = readMux(i);
    sensorValues[i] = sensorReading;

    
    if(sensorReading > threshold){
      Serial.print("Channel: "); Serial.print(i); Serial.print(" Sensor Reading: "); Serial.println(sensorReading);
      delay(5);
    }
  }
/*
    if(sensorReading >=threshold) {
      for (int pixel = 0; pixel < NUM_LEDS; pixel++){
            leds[pixel] = CRGB(255, 0, 0);
          }

          FastLED.show();
          delay(300);
          Serial.print(sensorReading);
          Serial.println(" Knock!");
    }
*/
  

  FastLED.clear();
  FastLED.show();
}

float readMux(int channel){
  int controlPin[] = {s0, s1, s2};

  int muxChannel[8][3]={
    {0,0,0}, //channel 0
    {1,0,0}, //channel 1
    {0,1,0}, //channel 2
    {1,1,0}, //channel 3
    {0,0,1}, //channel 4
    {1,0,1}, //channel 5
    {0,1,1}, //channel 6
    {1,1,1}, //channel 7
  };

  //loop through the 3 sig
  for(int i = 0; i < 3; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the Z pin
  int val = analogRead(muxInput);

  //return the value
  return val;
}
  
  

