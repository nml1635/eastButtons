//Three buttons doing some things. Debra Lemak 10/14/17
#include <FastLED.h>
#define NUM_LEDS 398
#define NUM_CLUSTERS 43
#define DATA_PIN 0
#define BRIGHTNESS  240
#define FRAMES_PER_SECOND 40

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

CRGB leds[NUM_LEDS];

// 1 == small/large | 2 == medium
int barnacles[44][2] = {
  {1, 0},   // 1
  {1, 10},  // 2
  {1, 20},  // 3
  {1, 27},  // 4
  {1, 34},  // 5
  {1, 41},  // 6
  {1, 50},  // 7
  {1, 57},   // 8
  {0, 65},   // 9
  {1, 81},   // 10
  {0, 92},   // 11
  {1, 105},   // 12
  {0, 112},   // 13
  {0, 125},   // 14
  {1, 138},   // 15
  {1, 145},   // 16
  {1, 152},   // 17
  {1, 159},   // 18
  {0, 166},   // 19
  {1, 180},   // 20
  {1, 190},   // 21
  {0, 197},   // 22
  {0, 210},   // 23
  {1, 223},   // 24
  {1, 232},   // 25
  {1, 239},   // 26
  {1, 246},   // 27
  {1, 253},   // 28
  {1, 260},   // 29
  {1, 267},   // 30
  {0, 274},   // 31
  {1, 287},   // 32
  {1, 295},   // 33
  {1, 305},   // 34
  {0, 312},   // 35
  {1, 325},   // 36
  {1, 332},   // 37
  {0, 339},   // 38
  {0, 352},   // 39
  {1, 365},   // 40
  {1, 372},   // 41
  {1, 379},   // 42
  {1, 386},   // 43
  {1, 392},   // 44
};

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
      // rainbowWithGlitter();
      test();
      break;
    case buttonPin02:
      Serial.println("Button 2 pressed.");  
      bpm();
      break;
    case buttonPin03:
      Serial.println("Button 3 pressed."); 
      // juggle();
      hotSection();
      break;
    case 0:
      Serial.println("No button pressed.");  
      hotSection();
      break;
  }
}

void setCluster(int cluster[], CHSV primary, CHSV secondary)
{
  if (cluster[0] == 1) {
    leds[cluster[1]] = primary;
    leds[(cluster[1] + 1)] = secondary;
    leds[(cluster[1] + 2)] = secondary;
    leds[(cluster[1] + 3)] = secondary;
    leds[(cluster[1] + 4)] = secondary;
    leds[(cluster[1] + 5)] = secondary;
    leds[(cluster[1] + 6)] = secondary;
  } else {
    leds[cluster[1]] = primary;
    leds[(cluster[1] + 2)] = secondary;
    leds[(cluster[1] + 4)] = secondary;
    leds[(cluster[1] + 6)] = secondary;
    leds[(cluster[1] + 8)] = secondary;
    leds[(cluster[1] + 10)] = secondary;
    leds[(cluster[1] + 12)] = secondary;
  }
}

/*
 * Mapping
 */
void test()
{
  FastLED.clear();
  for( int i = 0; i < NUM_CLUSTERS; i++) {
    setCluster(barnacles[i], CHSV(0, 0, 255), CHSV(165, 255, 255));
  }
}

void hotSection() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_CLUSTERS, 10);
  int pos = random8(NUM_CLUSTERS);
  setCluster(barnacles[pos], CHSV(0, 0, 255), CHSV(random8(140, 180), 255, 255));
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
  uint8_t BeatsPerMinute = FRAMES_PER_SECOND;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_CLUSTERS; i++) { //9948
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

