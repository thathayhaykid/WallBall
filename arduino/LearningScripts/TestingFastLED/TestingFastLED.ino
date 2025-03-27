#include <FastLED.h>
#define NUM_LEDS 60*5
#define DATA_PIN 9
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 60
#define VOLTS 5
#define MAX_AMPS 500

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
  delay(1000);

  FastLED.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called

  // turn pixels to green one-by-one with delay between each pixel
  for (int pixel = 0; pixel < NUM_LEDS; pixel++) {           // for each pixel
    leds[pixel] = CRGB(0, 255, 0);
    FastLED.show();                                           // update to the NeoPixel Led Strip

    delay(100);  // 500ms pause between each pixel
  }

  // turn off all pixels for two seconds
  FastLED.clear();
  FastLED.show();  // update to the NeoPixel Led Strip
  delay(2000);      // 2 seconds off time

  // turn on all pixels to red at the same time for two seconds
  for (int pixel = 0; pixel < NUM_LEDS; pixel++) {           // for each pixel
    leds[pixel] = CRGB(255, 0, 0);
  }
  FastLED.show();  // update to the NeoPixel Led Strip
  delay(1000);      // 1 second on time

  // turn off all pixels for one seconds
  FastLED.clear();
  FastLED.show();  // update to the NeoPixel Led Strip
  delay(1000);      // 1 second off time

}
