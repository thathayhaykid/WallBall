#include <FastLED.h>

// Put your # of rows & columns on your physical board (Counting for this starts at 1, not 0)
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

//Big numbers due to millis()
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Use these to save the time / identify periods
int oneperiod = 200;
unsigned long time_now = 0;

// Mux Control Pins
const int s0 = D10;
const int s1 = D11;
const int s2 = D12;

//Read Mux (At a Analog to digital pin of Arduino Nano ESP32)
const int muxInput = A0;

// Number of Piezos to read
const int numPiezo = 6;

// these variables will change:
int sensorReading = 0;

// threshold
const int threshold = 500;

void setup() {
  
  //Open Serial Port
  Serial.begin(115200);

  // Enable output pins for MUX 74HC4051
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);

  // Set initial of these pins to 0 (channel 0 on mux)
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);

  // Register each LED Segment with their associated data pins, enabling each segment to be seen as LEDs
  // Unfortunately, it would not let me do this in a for loop for some reason, so you have to address each strip
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

  // On Mux A0 = R1C1, A1 = R1C2, A2 = R1C3, A3 = R2C1, A4 = R2C2, A5 = R2C3
  int piezoNeeded = piezoRequired(randomRow, randomColumn);
  Serial.print("Piezo Needed: "); Serial.println(piezoNeeded);
  int sensorValues[numPiezo];
  int maxValue = 0;
  int maxValuePin = 0;
  while( maxValuePin != piezoNeeded){
    maxValue = 0;
    Serial.print("Piezo Needed: "); Serial.println(piezoNeeded);
    //Scan for piezo needed to move on
    for (int i = 0; i < numPiezo; i++){
      sensorReading = readMux(i);
      sensorValues[i] = sensorReading;
      delay(10);
      Serial.print("SensorReading: "); Serial.println(sensorReading);
    }

    for(int i=0; i < numPiezo; i++){
      if (sensorValues[i] > maxValue && sensorValues[i] > threshold){
      maxValue = sensorValues[i];
      maxValuePin = i;
      Serial.print("Max Value: "); Serial.print(maxValue); Serial.print(" Channel: "); Serial.println(maxValuePin); Serial.println();

      }
    }

    if(maxValue > threshold){
      Serial.print("Max Value: "); Serial.print(maxValue); Serial.print(" Channel: "); Serial.println(maxValuePin);
    }

    //If we haven't gotten it, flash the box that's needed to get hit
    if (millis() - time_now >= oneperiod){
      fillAll(255, 0, 0);
      lightUpBox(randomRow, randomColumn, 0, 255, 0);
      FastLED.show();
        if(millis() - time_now >= oneperiod * 3){
          lightUpBox(randomRow, randomColumn, 0, 0, 0);
          FastLED.show();
          time_now = millis();
          }
      }

    }
   


  }


// Returns the piezo we need to win This table will need to expand or be made automated to expand with piezos/squares added to the project
// On Mux A0 = R1C1, A1 = R1C2, A2 = R1C3, A3 = R2C1, A4 = R2C2, A5 = R2C3

int piezoRequired(int row, int column){
  if(row == 1 && column == 1){
    return 0;
  }
  else if(row == 1 && column == 2){
    return 1;
  }
  else if(row == 1 && column == 3){
    return 2;
  }
  else if(row == 2 && column == 1){
    return 3;
  }
  else if(row == 2 && column == 2){
    return 4;
  }
  else if(row == 2 && column == 3){
    return 5;
  }
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

// This reads the multiplexor channels by selecting the channel (currently 0-5 with 6 spots)
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
  