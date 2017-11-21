#include <FastLED.h>
#define NUM_LEDS1 130
#define NUM_LEDS2 130
#define NUM_LEDS3 130
#define DATA_PIN1 0
#define DATA_PIN2 4
#define DATA_PIN3 5

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

// constants won't change. They're used here to set pin numbers and fade:
const int buttonPin01 = 1;     // the number of the pushbutton pin
const int buttonPin02 = 2;     // the number of the pushbutton pin
const int buttonPin03 = 3;     // the number of the pushbutton pin

// variables will change:
int buttonPressed = 0;


void setup() {
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin01, INPUT);
  pinMode(buttonPin02, INPUT);
  pinMode(buttonPin03, INPUT);
  FastLED.addLeds<WS2811, DATA_PIN1, RGB>(leds1, NUM_LEDS1);
  FastLED.addLeds<WS2811, DATA_PIN2, RGB>(leds2, NUM_LEDS2);
  FastLED.addLeds<WS2811, DATA_PIN3, RGB>(leds3, NUM_LEDS3);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
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
  else {
  buttonPressed = 0;
  }
}

/**Lighting 'em up and count them out to see if we're all here
 */
 void checkstrand1() {
 for(int dot = 0; dot < NUM_LEDS1; dot++) { 
            leds1[dot] = CRGB::Blue;
            FastLED.show();
            Serial.println(dot);
            // clear this led for the next time around the loop
            leds1[dot] = CRGB::Black;
            delay(1000);
        }
    }
        
  void checkstrand2() {
 for(int dot = 0; dot < NUM_LEDS2; dot++) { 
            leds2[dot] = CRGB::Blue;
            FastLED.show();
            Serial.println(dot);
            // clear this led for the next time around the loop
            leds2[dot] = CRGB::Black;
            delay(1000);
        }
    }   
    
 void checkstrand3() {
 for(int dot = 0; dot < NUM_LEDS3; dot++) { 
            leds3[dot] = CRGB::Blue;
            FastLED.show();
            Serial.println(dot);
            // clear this led for the next time around the loop
            leds3[dot] = CRGB::Black;
            delay(1000);
        }
    }

/**
 * Handle the button state to render effects
 */
void renderEffects() {
    switch (buttonPressed) {
    case buttonPin01:
      checkstrand1();
      Serial.println("Button 1 pressed.");  
      break;
    case buttonPin02:
      checkstrand2();
      Serial.println("Button 2 pressed.");  
      break;
    case buttonPin03:
      checkstrand3();
      Serial.println("Button 3 pressed."); 
      break;
    case 0:
      Serial.println("No button pressed.");  
      break;
  }
}


/*
 * Main loop
 */
void loop() {
  checkInputs();
  renderEffects();
}
