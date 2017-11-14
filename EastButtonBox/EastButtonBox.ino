//Three buttons doing some things. Debra Lemak 10/14/17
#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 0
#define BRIGHTNESS  20
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];

// constants won't change. They're used here to set pin numbers and fade:
const int buttonPin01 = 8;     // the number of the pushbutton pin
const int buttonPin02 = 9;     // the number of the pushbutton pin
const int buttonPin03 = 10;     // the number of the pushbutton pin

// variables will change:
bool gReverseDirection = false;
int buttonPressed = 0;
int previousButton = 0;
int ledMode = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void setup() {
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin01, INPUT_PULLUP);
  pinMode(buttonPin02, INPUT_PULLUP);
  pinMode(buttonPin03, INPUT_PULLUP);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
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

/**
 * Check our inputs and set the button state
 */
void checkInputs() {
  // Read pins
  if (digitalRead(buttonPin01) == LOW) {
    buttonPressed = buttonPin01;
    return buttonPressed;
  } 
  
  if (digitalRead(buttonPin02) == LOW) {
    buttonPressed = buttonPin02;
    return buttonPressed;
  } 
  
  if (digitalRead(buttonPin03) == LOW) {
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
      Serial.println("Button 1 pressed.");  
      rainbowWithGlitter();
      break;
    case buttonPin02:
      Serial.println("Button 2 pressed.");  
      bpm();
      break;
    case buttonPin03:
      Serial.println("Button 3 pressed."); 
      juggle();
      break;
    case 0:
      Serial.println("No button pressed.");  
      break;
  }
}

/*
 * Efect #1
 */
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
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

