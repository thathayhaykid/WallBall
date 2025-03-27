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
  FastLED.clear();
  FastLED.show();
  delay(200);
  fillAll(255, 0,0);
  int randomRow = random(1, rows+1);
  int randomColumn = random(1, columns+1);
  lightUpBox(randomRow, randomColumn, 0, 255, 0);
  FastLED.show();
  delay(500);
}

//This function lights up a particular horizontal segment
void HSegment(int strip, int SegmentNumber, int R, int G, int B) {
  int startPixel = SegmentNumber * H_SEGMENT_NUM_LEDS;
  for (int segPix = startPixel; segPix < startPixel + H_SEGMENT_NUM_LEDS; segPix++){
    HLeds[strip][segPix] = CRGB(R, G, B);
  }
}

//This function lights up a particular vertical segment
void VSegment(int strip, int SegmentNumber, int R, int G, int B) {
  int startPixel = SegmentNumber * V_SEGMENT_NUM_LEDS;
  for (int segPix = startPixel; segPix < startPixel + V_SEGMENT_NUM_LEDS; segPix++){
    VLeds[strip][segPix] = CRGB(R, G, B);
  }
}


// This function fills all LEDS with a selected color
void fillAll(int R, int G, int B){
  for (int strips = 0; strips < HSTRIPS; strips++){
    for (int pixel = 0; pixel < NUM_LEDS_HORIZONTAL; pixel++){
      HLeds[strips][pixel] = CRGB(R, G, B);
   }
  }

  for (int strips = 0; strips < VSTRIPS; strips++){
    for (int pixel = 0; pixel < NUM_LEDS_VERTICAL; pixel++){
      VLeds[strips][pixel] = CRGB(R, G, B);
   }
  }
}

void lightUpBox(int rowNum,int columnNum, int R, int G, int B){
  // Lots of these if cases are to locate where in the grid we're selecting to light up, and lighting up the appropriate segments to their correct colors
  // IF the box specified is in the upper right corner
  if(rowNum == 1 && columnNum == 1){
    int Hstrip = rowNum - 1;
    int Vstrip = columnNum - 1;
    int Hsegment = columnNum - 1;
    int Vsegment = rowNum - 1;
    HSegment(Hstrip, Hsegment, R, G, B);
    VSegment(Vstrip, Vsegment, R, G, B);
  }

  //If the last box in the first row is selected
  else if(rowNum == 1 && columnNum == columns){
    int HStrip = rowNum - 1;
    int VStrip = columnNum - 2;
    int Hsegment = columnNum - 1;
    int Vsegment = rowNum - 1;
    HSegment(HStrip, Hsegment, R, G, B);
    VSegment(VStrip, Vsegment, R, G, B);
  }
  //If the top row, center box is selected
  else if (rowNum == 1 && columnNum > 1 && columnNum < columns){
    int HStrip = rowNum - 1;
    int VStrip1 = columnNum - 2;
    int VStrip2 = columnNum - 1;
    int Hsegment = columnNum - 1;
    int Vsegment = rowNum - 1;
    HSegment(HStrip, Hsegment, R, G, B);
    VSegment(VStrip1, Vsegment, R, G, B);
    VSegment(VStrip2, Vsegment, R, G, B);
  }
  //If center row, first column is selected
  else if (rowNum > 1 && rowNum < rows && columnNum == 1){
    int HStrip1 = rowNum - 2;
    int HStrip2 = rowNum - 1;
    int VStrip = columnNum - 1;
    
    int Hsegment = columnNum -1;
    int Vsegment = rowNum - 1;
    HSegment(HStrip1, Hsegment, R, G, B);
    HSegment(HStrip2, Hsegment, R, G, B);
    VSegment(VStrip, Vsegment, R, G, B);
  }
  // if center, center row is selected
  else if (rowNum > 1 && rowNum < rows && columnNum > 1 && columnNum < columns){
    int HStrip1 = rowNum - 2;
    int HStrip2 = rowNum - 1;
    int VStrip1 = columnNum - 2;
    int VStrip2 = columnNum - 1;

    int Hsegment = columnNum - 1;
    int Vsegment = rowNum - 1;
    HSegment(HStrip1, Hsegment, R, G, B);
    HSegment(HStrip2, Hsegment, R, G, B);
    VSegment(VStrip1, Vsegment, R, G, B);
    VSegment(VStrip2, Vsegment, R, G, B);
  }

  // if Center row, last column is selected
  else if (rowNum > 1 && rowNum < rows && columnNum == columns){
    int HStrip1 = rowNum - 2;
    int HStrip2 = rowNum - 1;
    int VStrip = columnNum - 2;
    
    int Hsegment = columnNum -1;
    int Vsegment = rowNum - 1;
    HSegment(HStrip1, Hsegment, R, G, B);
    HSegment(HStrip2, Hsegment, R, G, B);
    VSegment(VStrip, Vsegment, R, G, B);

  }

  // if First Column, Last Row  is selected
  else if (rowNum == rows && columnNum == 1){
    int Hstrip = rowNum - 2;
    int Vstrip = columnNum - 1;
    int Hsegment = columnNum - 1;
    int Vsegment = rowNum - 1;
    HSegment(Hstrip, Hsegment, R, G, B);
    VSegment(Vstrip, Vsegment, R, G, B);
  }

  // if a center column, last row is selected
  else if (rowNum == rows && columnNum > 1 && columnNum < columns){
    int HStrip = rowNum - 2;
    int VStrip1 = columnNum - 2;
    int VStrip2 = columnNum - 1;
    int Hsegment = columnNum - 1;
    int Vsegment = rowNum - 1;
    HSegment(HStrip, Hsegment, R, G, B);
    VSegment(VStrip1, Vsegment, R, G, B);
    VSegment(VStrip2, Vsegment, R, G, B);
  }

  // if the last column, last row is selected 
  else if (rowNum == rows && columnNum == columns){
    int Hstrip = rowNum - 2;
    int Vstrip = columnNum - 2;
    int Hsegment = columnNum - 1;
    int Vsegment = rowNum - 1;
    HSegment(Hstrip, Hsegment, R, G, B);
    VSegment(Vstrip, Vsegment, R, G, B);

  }

}