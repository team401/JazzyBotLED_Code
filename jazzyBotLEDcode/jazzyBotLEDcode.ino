// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define BODYPIN           8
#define EYEPIN            9
// How many NeoPixels are attached to the Arduino for the BODY?
#define NUMPIXELS      210

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel body = Adafruit_NeoPixel(NUMPIXELS, BODYPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel eyes   = Adafruit_NeoPixel(48, EYEPIN, NEO_GRB + NEO_KHZ800);
int delayval = 200; // delay for 1/5th of a second
long RED ;
long GREEN;
long BLUE;
long WHITE;
long DARK_BLUE;
long YELLOW;
int eyeLoop = 0;

int n = 0;

void setup() {

  eyes.begin();
  eyes.setBrightness(35);
  body.begin(); // This initializes the NeoPixel library.
  body.setBrightness(250); // 5 for running off arduino, 10 for battery testing, 100 for parade
  RED = body.Color(255,0,0);
  GREEN = body.Color(0,255,0);
  BLUE = body.Color(0,0,255);
  WHITE  = body.Color(255,255,255);
  DARK_BLUE = body.Color(0, 0, 139);
  YELLOW = body.Color(255, 255, 0);


}
 
void loop() {
 
  // change eye color each loop through?
  eyeLoop++ ;
  if ( eyeLoop%2 == 1)
      colorEyes(DARK_BLUE) ;
  else
      colorEyes(YELLOW);
 
  twoColorSlow(DARK_BLUE , YELLOW) ;
  delay(delayval); // Delay for a period of time (in milliseconds).
 
  // (Base Color, Snake Color, Loop Count, Delay, Snake Size, Loop Accel)
  loopingThing(DARK_BLUE, YELLOW, 5, 50, 5, 10);
 
    delay(delayval);
  sparkle(10); // 10 Seconds of sparkle
 
 
}

void colorEyes(long eyeColor){
  for(int i = 0;i<=23;i++) {
    //each eye has 24 body, so 1 eye starts from 0, the other from 24
    eyes.setPixelColor(i, eyeColor);
    eyes.setPixelColor(47-i, eyeColor);
    eyes.show() ;
    delay(delayval/4);
  }
}
void twoColorSlow (long nColor1, long nColor2) {
  unsigned long startTime_ms = millis() ;
  while( startTime_ms > millis()-10000){
    n += 1;
    for(int i=0;i<NUMPIXELS;i++){
      if (i%10 == n%10 || i%10 == n%10 - 1){
        // body.Color takes RGB values, from 0,0,0 up to 255,255,255
        body.setPixelColor(i, nColor2); 
      }
      else /*if(i%4 == n%4)*/{
        body.setPixelColor(i, nColor1); 
        //body.show();
       // delay(delayval);
      }
    }
    body.show(); // This sends the updated pixel color to the hardware.
    delay(delayval) ;
  }
 
}

void loopingThing(long baseColor, long snakeColor, int loopCount, int loopDelay, int snakeSize, int loopAccel){
  int basePixel;
//   Roll eyes, faster and faster
  for(int j = 0; j < loopCount; j++){
   for(int i = 0; i<=23; i++) {
    //each eye has 24 body, so 1 eye starts from 0, the other from 24
   
    eyes.setPixelColor(24+i, snakeColor);
    eyes.setPixelColor(23-i, snakeColor);
    // reset the trailing side
    basePixel = i - snakeSize;  
      if (basePixel >= 0 || j > 0){
        if (basePixel < 0){
          basePixel += 24;  
        }
        eyes.setPixelColor(24 + basePixel, baseColor);
        eyes.setPixelColor(23 - basePixel, baseColor);
      }
    eyes.show() ;
    delay(loopDelay - (loopAccel * j));
   }
  }
  // cleanup eyes
  // TBD

  // send pulse down the body
  for (int i = 0; i < NUMPIXELS; i++){
    basePixel = i - snakeSize;
    body.setPixelColor(i, snakeColor);
      if (basePixel >=0){
        if (basePixel < 0){
          basePixel += 24;
          }
          body.setPixelColor(basePixel, baseColor);
        }
   body.show();
  }  
 }
 
 void sparkle(int duration_sec){
 
  unsigned long startTime_ms = millis() ;
  while( startTime_ms < millis()-(duration_sec*1000)){
      int pixel = random(NUMPIXELS);
        int color = random(4);
          switch(color) {
          
          case 1: body.setPixelColor(pixel, BLUE);
          case 2: body.setPixelColor(pixel, YELLOW);
          case 3: body.setPixelColor(pixel, WHITE);
          case 4: body.setPixelColor(pixel, 0);
          
          default: body.setPixelColor(pixel, RED);
         body.show();
    }
  }
 }
   
 
