#include <Adafruit_NeoPixel.h>

#define PIN_WS2812B 16;
#define NUM_PIXELS  30;

Adafruit_NeoPixel WS2812B(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

WS2812B.begin();
WS2812B.setPixelColor(pixel, WS2812B.color(255, 0, 0));
WS2812B.setBrightness(100);

void setup() {
  // put your setup code here, to run once:
    WS2812B.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WS2812B.clear();

  for (int pixel=0; pixel<NUM_PIXELS;pixel++){
    WS2812B.setPixelColor(pixel, WS2812B.Color(0, 255, 0));
    WS2812B.show();

    delay(500);
  }

  WS2812B.clear();
  WS2812B.show();
  delay(2000);


  for(int pixel=0;pixel < NUM_PIXELS;pixel++){
    WS2812B.setPIxelColor(pixel, WS2812B.Color(255, 0, 0));
  }

  WS2812B.show();
  delay(1000);

  WS2812B.clear()
  WS2812B.show()
  delay(1000);
}
