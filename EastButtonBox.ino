//Three buttons doing some things. Debra Lemak 10/14/17
#include <FastLED.h>
#define NUM_LEDS1 60
#define NUM_LEDS2 120
#define NUM_LEDS3 116
#define DATA_PIN1 9
#define DATA_PIN2 5
#define DATA_PIN3 6

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

// constants won't change. They're used here to set pin numbers and fade:
const int buttonPin01 = 2;     // the number of the pushbutton pin
const int buttonPin02 = 4;     // the number of the pushbutton pin
const int buttonPin03 = 7;     // the number of the pushbutton pin

bool gReverseDirection = false;

// variables will change:

int ledMode = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
long boredomTimer = 0; 


///////Pulling in new color sequence from Andrew Tuline, Title: inoise8_pal_demo.ino
static uint16_t dist;         // A random number for our noise generator.
uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 48;      // Value for blending between palettes.
 
CRGBPalette16 currentPalette(CRGB::Amethyst);
CRGBPalette16 targetPalette(OceanColors_p);
/////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin01, INPUT);
  pinMode(buttonPin02, INPUT);
  pinMode(buttonPin03, INPUT);

  // set up LED strands
  FastLED.addLeds<WS2811, DATA_PIN1, RGB>(leds1, NUM_LEDS1);
  FastLED.addLeds<WS2811, DATA_PIN2, RGB>(leds2, NUM_LEDS2);
  FastLED.addLeds<WS2811, DATA_PIN3, RGB>(leds3, NUM_LEDS3);
  FastLED.setBrightness( BRIGHTNESS );

  // now, make them do something 
  FastLED.clear(); // probably not needed, if we are filling?
  basicColorFill();
  FastLED.show();

//////////from Tuline
  dist = random16(12345);          // A semi-random number for our noise generator
/////////
}

/*
 * Main loop
 */
void loop() {
  int buttonPressed;

  // get button and use it
  buttonPressed = checkInputs();

  // was anything pressed?
  if (0 != buttonPressed) {
    boredomTimer = 0;
    renderEffects(buttonPressed);
  }
  else if (boredomTimer > 50000) {
      boredom();
      Serial.println("boredom() activated");
    }
    else {
      boredomTimer++;
      Serial.println(boredomTimer);
  }
  
 // we are go for LED activity
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

/**
 * Check our inputs and set the button state
 */
int checkInputs() {
  int buttonPressed = 0; // initialize to BUTTON_PRESS_NONE

  // lowest priority button (big normal one)
  if (digitalRead(buttonPin03) == HIGH) {
    buttonPressed = buttonPin03;
  } 

  // will overrule button 1, if both are pushed
  if (digitalRead(buttonPin02) == HIGH) {
    buttonPressed = buttonPin02;
  } 

  // highest priority button (big red one)
  if (digitalRead(buttonPin01) == HIGH) {
    buttonPressed = buttonPin01;
  } /*5ty*/
  
  return buttonPressed;
}

void basicColorFill() {  
    // fill tentacle 1
    for( int i = 0; i < NUM_LEDS1; i++) {
      leds1[i] = CRGB::Red;
    }
    // fill tentacle 2
    for( int i = 0; i < NUM_LEDS2; i++) {
      leds2[i] = CRGB::Green;
    }    
    // fill tentacle 3
    for( int i = 0; i < NUM_LEDS3; i++) {
      leds3[i] = CRGB::Blue;
    }
    
    return;
}

// react to button press
void renderEffects(int buttonPressed) {
  
    switch (buttonPressed) {
    case buttonPin01:
      Serial.println("Button 1 pressed.");  
      noisepalWithGlitter();
      break;
    case buttonPin02:
      Serial.println("Button 2 pressed.");  
      bpm(leds1, NUM_LEDS1);
      bpm(leds2, NUM_LEDS2);
      bpm(leds3, NUM_LEDS3);
      break;
    case buttonPin03:
      Serial.println("Button 3 pressed."); 
      Fire2012 (leds1, NUM_LEDS1);
      Fire2012 (leds2, NUM_LEDS2);
      Fire2012 (leds3, NUM_LEDS3);
      break;
    case 0:
      Serial.println("No button pressed??");  
      break;
    }
}

/* Efect #1
*/ 
void addGlitter(CRGB leds, int NUM_LEDS, fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void fillnoise8(CRGB leds, int NUM_LEDS) {
  for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*scale, dist+i*scale) % 255;                  // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  dist += beatsin8(10,1, 4);                                               // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                                                                           // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} 

void noisepal(CRGB leds, int NUM_LEDS) 
{
  EVERY_N_MILLISECONDS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
    fillnoise8(leds, NUM_LEDS);                                                           // Update the LED array with noise at the new location
  }
  EVERY_N_SECONDS(5) {             // Change the target palette to a random one every 5 seconds.
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
}

void noisepalWithGlitter() 
{
  noisepal(leds1, NUM_LEDS1);
  noisepal(leds2, NUM_LEDS2);
  noisepal(leds3, NUM_LEDS3);
  addGlitter(leds1, NUM_LEDS1,80);
  addGlitter(leds2, NUM_LEDS2,80);
  addGlitter(leds3, NUM_LEDS3,80);
}



/*
 * Efect #2
 */
void bpm(CRGB leds,int NUM_LEDS)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = OceanColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    
  }
}


/*
 * Efect #3
 */

#define COOLING  55
#define SPARKING 80

void Fire2012(CRGB leds, int NUM_LEDS) {
// Array of temperature readings at each simulation cell
    byte heat[NUM_LEDS];
  
   // Step 1.  Cool down every cell a little
     for( int i = 0;  i < NUM_LEDS; i++) {
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



/**
 * Effect #4 for passing time when the Button Beast is not receiving love
 */
 
void boredom() {
     fill_solid(leds1, NUM_LEDS1, CRGB::Purple);
     float breath = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
     FastLED.setBrightness(breath);
     FastLED.show(); 
}





// For future reference - http://tuline.com/some-fastled-notes/
