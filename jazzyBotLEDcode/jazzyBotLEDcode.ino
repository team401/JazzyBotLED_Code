// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define BODYPIN 8
#define EYEPIN 9
// How many NeoPixels are attached to the Arduino for the BODY
#define NUMPIXELS 210

// Each eye has 24 NeoPixels, 48 Total, First Eye : 0-23, Second Eye : 24-47

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel body = Adafruit_NeoPixel(NUMPIXELS, BODYPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel eyes = Adafruit_NeoPixel(48, EYEPIN, NEO_GRB + NEO_KHZ800);

int delayval = 200;  // delay for 1/5th of a second 

bool eyeLoop = 0;
bool pulse = 0;

int counter = 0;

// Defines Colors which can later be passed to setPixelColor(), RGB Values wrapped/packed into a 32bit Integer Value (Long type)
const uint32_t RED = body.Color(255, 0, 0);
const uint32_t GREEN = body.Color(0, 255, 0);
const uint32_t BLUE = body.Color(0, 0, 255);
const uint32_t ROYAL_BLUE = body.Color(65, 105, 225);
const uint32_t YELLOW = body.Color(255, 255, 0);
const uint32_t GOLD = body.Color(255, 223, 0);
const uint32_t WHITE = body.Color(255, 255, 255);
const uint32_t BLACK = body.Color(0, 0, 0);

void setup() {
  eyes.begin();
  eyes.setBrightness(35);   

  body.begin();             // This initializes the NeoPixel library.
  body.setBrightness(100);  // 5 for running off arduino, 10 for battery testing, 100 for parade
}

//Runs a "Pulse" from the start pixel to the end pixel of a specified length and a random choice between two colors provided. -Will
void bodyPulse(uint32_t color1, uint32_t color2, int length, int start, int end){
  pulse = !pulse;
  //Shifts Every Pixel down by one, intended to be run continuously
  for(int i = end - 1; i >= start + length; i--){
    body.setPixelColor(i, body.getPixelColor(i -1));
  }

  // Add a Pulse of Length from start, Randomly Chooses Color
  for(int i = start; i < start + length; i++){
    body.setPixelColor(i, pulse? color1 : color2);
  }

  // Turn off pixels at end of strip
  for(int i = end - length; i < end; i++){
    body.setPixelColor(i, 0);
  }

  body.show();
  delay(delayval);
}

void colorEyes(uint32_t eyeColor) {
  for (int i = 0; i <= 24; i++) {
    eyes.setPixelColor(i, eyeColor);
    eyes.setPixelColor(47 - i, eyeColor);
    eyes.show();
    delay(delayval / 4);
  }
}

void twoColorSlow(uint32_t nColor1, uint32_t nColor2) {
  unsigned long startTime_ms = millis();
  
  // Runs for 10,000ms or 10s
  while (startTime_ms > millis() - 10000) {
    counter += 1;
    for (int i = 0; i < NUMPIXELS; i++) {
      if (i % 10 == counter % 10 || i % 10 == counter % 10 - 1) {
        // body.Color takes RGB values, from 0,0,0 up to 255,255,255
        body.setPixelColor(i, nColor2);  // Moderately bright red color.
      } else /*if(i%4 == n%4)*/ {
        body.setPixelColor(i, nColor1);  // Makes it green
        //body.show();
        // delay(delayval);
      }
    }
    body.show();  // This sends the updated pixel color to the hardware.
    delay(delayval);
  }
}

void loopingThing(uint32_t baseColor, uint32_t snakeColor, int loopCount, int loopDelay, int snakeSize, int loopAccel) {
  int basePixel;
  //Roll eyes, faster and faster
  for (int i = 0; i < loopCount; i++) {
    for (int j = 0; j <= 23; j++) {
      //each eye has 24 body, so 1 eye starts from 0, the other from 24
      eyes.setPixelColor(24 + j, snakeColor);
      eyes.setPixelColor(23 - j, snakeColor);
      // reset the trailing side
      basePixel = j - snakeSize;
      if (basePixel >= 0 || i > 0) {
        if (basePixel < 0) {
          basePixel += 24;
        }
        eyes.setPixelColor(24 + basePixel, baseColor);
        eyes.setPixelColor(23 - basePixel, baseColor);
      }
      eyes.show();
      delay(loopDelay - (loopAccel * j));
    }
  }
  // cleanup eyes
  // TBD

  // send pulse down the body
  for (int i = 0; i < NUMPIXELS; i++) {
    basePixel = i - snakeSize;
    body.setPixelColor(i, snakeColor);
    if (basePixel >= 0) {
      if (basePixel < 0) {
        basePixel += 24;
      }
      body.setPixelColor(basePixel, baseColor);
    }
    body.show();
  }
}

//09/23/2024, Will, added passthrough of color array, so you can choose the colors it sparkles with, still random 

void sparkle(int duration_sec, uint32_t colors[]) {

  unsigned long startTime_ms = millis();
  while (startTime_ms < millis() - (duration_sec * 1000)) {
    int pixel = random(NUMPIXELS);
    int color = random(sizeof(colors) / 32);

    body.setPixelColor(pixel, colors[color]);
    
    /*
    if (color == 1)
      body.setPixelColor(pixel, BLUE);
    else if (color == 2)
      body.setPixelColor(pixel, RED);
    else if (color == 3)
      body.setPixelColor(pixel, GREEN);
    else if (color == 4)
      body.setPixelColor(pixel, WHITE);
    else if (color == 5)
      body.setPixelColor(pixel, 0);  // black (off)
    */

    body.show();
  }
}

void loop() {

  // Changes the color of eyes per loop
  eyeLoop = !eyeLoop;
  if (eyeLoop)
    colorEyes(ROYAL_BLUE);
  else
    colorEyes(GOLD);

  // Ig theres a bunch of different functions being called in order to have some alternation? - Will

  twoColorSlow(ROYAL_BLUE, GOLD);
  delay(delayval);  // Delay for a period of time (in milliseconds).

  // (Base Color, Snake Color, Loop Count, Delay, Snake Size, Loop Accel)
  loopingThing(ROYAL_BLUE, GOLD, 5, 50, 5, 10);

  delay(delayval);
  uint32_t colors[] = {ROYAL_BLUE, GOLD, BLACK};
  sparkle(10, colors);  // 10 Seconds of sparkle
}
