#include <FastLED.h>

// Put your # of rows & columns on your physical board
#define rows 2
#define columns 3

// This is the number of strips you should have
#define HSTRIPS rows - 1
#define VSTRIPS columns - 1

// Insert your number of LEDS per vertical strip (They all must be equal, NUM_LEDS_VERTICAL must be wholly divisible by V_SEGMENT_NUM_LEDS), and type how many LEDS per segment (needs to be equal on the board itself)
#define NUM_LEDS_VERTICAL 6
#define V_SEGMENT_NUM_LEDS 3
#define V_SEGMENTS NUM_LEDS_VERTICAL / V_SEGMENT_NUM_LEDS

// Insert your number of LEDS per horizontal strip (They all must be equal, NUM_LEDS_HORIZONTAL must be wholly divisible by V_SEGMENT_NUM_LEDS), and type how many LEDS per segment (needs to be equal on the board itself)
#define NUM_LEDS_HORIZONTAL 15
#define H_SEGMENT_NUM_LEDS 5
#define H_SEGMENTS NUM_LEDS_HORIZONTAL / H_SEGMENT_NUM_LEDS

// Define your data pins for your strips. This will change when using shift registers but for now this is what we're doing
// Unfortunately we can't use arrays so we have to define each pin individually
const int V_DATA_PIN00 = 8;
const int V_DATA_PIN01 = 7;

const int H_DATA_PIN00 = 9;

// Predefines for FastLED Library, also reflects the information about our LEDS in particular, so be sure to double check your equipment
#define COLOR_ORDER GRB
#define CHIPSET WS2812B

// Max Values/Restrictions to ensure power is supplied well throughout the board. Adjust as needed
#define BRIGHTNESS 60
#define VOLTS 5
#define MAX_AMPS 500

// Define our LED Segments for the FastLED Library, the "base" array will only contain the LEDS in that particular segment,allowing us to address whole segments at a time
CRGB HLeds[HSTRIPS][NUM_LEDS_HORIZONTAL];
CRGB VLeds[VSTRIPS][NUM_LEDS_VERTICAL];

void setup() {
  // Register each LED Segment with their associated data pins, enabling each segment to be seen as LEDs
  // Unfortunately, it would not let me do this in a for loop for some reason, so you have to address each strip

  Serial.begin(115200);

  FastLED.addLeds<CHIPSET, H_DATA_PIN00, COLOR_ORDER>(HLeds[0], NUM_LEDS_HORIZONTAL);
  FastLED.addLeds<CHIPSET, V_DATA_PIN00, COLOR_ORDER>(VLeds[0], NUM_LEDS_VERTICAL);
  FastLED.addLeds<CHIPSET, V_DATA_PIN01, COLOR_ORDER>(VLeds[1], NUM_LEDS_VERTICAL);

  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS);


  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  for (int HS = 0; HS < HSTRIPS; HS++) {
    for (int pixel = 0; pixel < NUM_LEDS_HORIZONTAL; pixel++) {
      HLeds[HS][pixel] = CRGB(255, 0, 0);
    }
  }

  for (int VS = 0; VS < VSTRIPS; VS++) {
    for (int pixel = 0; pixel < NUM_LEDS_VERTICAL; pixel++) {
      VLeds[VS][pixel] = CRGB(255, 0, 0);
    }
  }
  FastLED.show();
  delay(500);
  FastLED.clear();
  FastLED.show();
  delay(200);

  HSegment(0, 0, H_SEGMENT_NUM_LEDS, H_SEGMENTS, 0, 255, 0);
  FastLED.show();
  delay(300);
}
void HSegment(int strip, int SegmentNumber, int NUM_LEDS, int H_SEG, int R, int G, int B) {
  int startPixel = SegmentNumber * NUM_LEDS;
  for (int segPix = startPixel; segPix < startPixel + NUM_LEDS; segPix++){
    HLeds[strip][segPix] = CRGB(R, G, B);
  }
}