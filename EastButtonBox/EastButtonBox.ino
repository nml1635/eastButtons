//Three buttons doing some things. Debra Lemak 10/14/17
#include <FastLED.h>
#define NUM_LEDS 125
#define DATA_PIN 0
#define BRIGHTNESS  20
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];

// constants won't change. They're used here to set pin numbers and fade:
const int buttonPin01 = 1;     // the number of the pushbutton pin
const int buttonPin02 = 2;     // the number of the pushbutton pin
const int buttonPin03 = 3;     // the number of the pushbutton pin

// variables will change:
bool gReverseDirection = false;
int buttonPressed = 0;
int previousButton = 0;
int ledMode = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


///////Pulling in new color sequence from Andrew Tuline, Title: inoise8_pal_demo.ino
static uint16_t dist;         // A random number for our noise generator.
uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 48;      // Value for blending between palettes.
 
CRGBPalette16 currentPalette(CRGB::Black);
CRGBPalette16 targetPalette(OceanColors_p);
/////////////////////////////////////////////////////////////////

void setup() {
  //Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin01, INPUT_PULLUP);
  pinMode(buttonPin02, INPUT_PULLUP);
  pinMode(buttonPin03, INPUT_PULLUP);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
//////////from Tuline
  dist = random16(12345);          // A semi-random number for our noise generator
/////////
}


/*
 * Efect #1
 */
void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void fillnoise8() {
  for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*scale, dist+i*scale) % 255;                  // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  dist += beatsin8(10,1, 4);                                               // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                                                                           // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} 

void noisepal() 
{
  EVERY_N_MILLISECONDS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
    fillnoise8();                                                           // Update the LED array with noise at the new location
  }
  EVERY_N_SECONDS(5) {             // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
}

void noisepalWithGlitter() 
{
  noisepal();
  addGlitter(80);
}



/*
 * Efect #2
 */
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}


/*
 * Efect #3
 */
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


/**
 * Check our inputs and set the button state
 */
void checkInputs() {
  // Read pins
  if (digitalRead(buttonPin01) == HIGH) {
    buttonPressed = buttonPin01;
    return buttonPressed;
  } 
  
  if (digitalRead(buttonPin02) == HIGH) {
    buttonPressed = buttonPin02;
    return buttonPressed;
  } 
  
  if (digitalRead(buttonPin03) == HIGH) {
    buttonPressed = buttonPin03;
    return buttonPressed;
  } 

  buttonPressed = 0;
}

/**
 * Handle the button state to render effects
 */
void renderEffects() {
    switch (buttonPressed) {
    case buttonPin01:
    //  Serial.println("Button 1 pressed.");  
      noisepalWithGlitter();
      break;
    case buttonPin02:
      //Serial.println("Button 2 pressed.");  
      bpm();
      break;
    case buttonPin03:
     // Serial.println("Button 3 pressed."); 
      juggle();
      break;
    case 0:
     // Serial.println("No button pressed.");  
      break;
  }
}


/*
 * Main loop
 */
void loop() {
  checkInputs();
  renderEffects();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}


// For future reference - http://tuline.com/some-fastled-notes/
