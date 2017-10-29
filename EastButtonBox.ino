//Three buttons doing some things. Debra Lemak 10/14/17
/*
 Started with code from: 

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Button
*/
#include <FastLED.h>
#define NUM_LEDS 200
#define DATA_PIN 0
#define LED_PIN     0
#define COLOR_ORDER RGB
#define CHIPSET     WS2811
#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];

// constants won't change. They're used here to set pin numbers and fade:
const int buttonPin01 = 1;     // the number of the pushbutton pin
const int buttonPin02 = 2;     // the number of the pushbutton pin
const int buttonPin03 = 3;     // the number of the pushbutton pin
bool gReverseDirection = false;

// variables will change:
int buttonPressed = 0;

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin01, INPUT);
  pinMode(buttonPin02, INPUT);
  pinMode(buttonPin03, INPUT);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
}

void loop() {
  if (digitalRead(buttonPin01) == HIGH) {
    buttonPressed = buttonPin01;
  } else if (digitalRead(buttonPin02) == HIGH) {
    buttonPressed = buttonPin02;
  } else if (digitalRead(buttonPin03) == HIGH) {
    buttonPressed = buttonPin03;
  } else {
    buttonPressed = 0;
  }

if (buttonPressed == 1) {
  breath(1);
} else {
  breath(0);
}

if (buttonPressed == 2) {
  setFire(1);
} else {
  setFire(0);
}

if (buttonPressed == 3) {
  lightChase(1);
} else {
  lightChase(0);
}

/*
  switch (buttonPressed) {
    case buttonPin01:
      breath();
    case buttonPin02:
      setFire();
    case buttonPin03:
      lightChase();
  }
  */
}

// Function breathes lights in purple 
int breath(int state) {
  while (state == 1) {
    float breathBrightness = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
    FastLED.setBrightness(breathBrightness);
    FastLED.show();
  }
}

//Function lights leds to look like a fire 
int setFire(int state) {
  while (state == 1) {
    Fire2012(); // run simulation frame
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
}

// Function wipes leds in a chase
int lightChase(int state) {
  while (state == 1) {
    for(int i = 0; i < NUM_LEDS; i = i + 1) {
        leds[i] = CRGB::WhiteSmoke;
        leds[i].maximizeBrightness();
        FastLED.show();
        FastLED.delay(45);
        leds[i] = CRGB::OldLace;
        leds[i].fadeLightBy( 128 );
        leds[i] = CRGB::Black;
        FastLED.show();
    }
  } 
}

#define COOLING  55
#define SPARKING 80

void Fire2012() {
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

