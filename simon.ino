#include <Button.h>
#include <stdlib.h>

#include "Talkie.h"
#include "Vocab_US_Large.h"

#include "FastLED.h"

// -=-=-=-=- CONFIGURATION -=-=-=-=-=-=-
const int NUM_BUTTONS = 6;
const int NUM_LIGHTS = 15;
const int SPEAKER_PIN = 5;
const int LIGHT_STRIP_PIN = 2;

// Light mode has been permanently changed to light strip
int buttonPins[NUM_BUTTONS] = {8, 11, 9, 12, 13, 10};

// Frequencies for E4 A4 C#5 E5
// int tones[NUM_BUTTONS] = {330, 440, 554, 659};

// Frequencies for E4 F#4 G#4 B4 C#5 E5
int TONES[NUM_BUTTONS] = {330, 370, 415, 494, 554, 659};
int WRONG_TONE = 220;

CRGB COLORS[NUM_BUTTONS] = {0x600000, 0x401500, 0x403000, 0x003000, 0x000030, 0x202020};

// -=-=-=-=- END CONFIGURATION -=-=-=-=-

Button *buttons[NUM_BUTTONS];
CRGB leds[NUM_LIGHTS];

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  FastLED.addLeds<WS2812B, LIGHT_STRIP_PIN, GRB>(leds, NUM_LIGHTS);
  
  for (int i=0; i<NUM_BUTTONS; i++){
    buttons[i] = new Button(buttonPins[i]);
    buttons[i]->begin();
    displayColor(COLORS[i]);
    tone(SPEAKER_PIN, TONES[i], 200);
    delay(250);
  }
  displayColor(CRGB::Black);
  randomSeed(analogRead(0) * analogRead(1) * analogRead(2));


  
}

void loop() {
  delay(100);
  // Initialize game
  // Create array to hold the sequence, and a length that shows how long the sequence is so far
  int sequence[100];
  int len = 0;
  
  bool inGame = true;
  while(inGame){
    // Generate random number to be added to sequence
    int next = random(0, NUM_BUTTONS);
    sequence[len] = next;
    len++;

    // Play the sequence of lights/tones
    // We don't care about any inputs during this so we can use delay();
    delay(750);
    for (int i=0; i<len; i++){
      noTone(SPEAKER_PIN);
      tone(SPEAKER_PIN, TONES[sequence[i]], 250);
      displayColor(COLORS[sequence[i]]);
      delay(250);
      displayColor(CRGB::Black);
      delay(250);
    }

    // Wait for all of the responses
    int responses = 0;
    while((responses < len) and inGame){
      long lastActionTime = millis();
      bool inputGiven = false;
      int input = 0;
      
      // Wait for an input
      // This loop stops when the user takes over 5 seconds or gives a press
      while (((millis() - lastActionTime) < 5000) and (inputGiven == false)){
        // Check all of the buttons for their state
        for (int i=0; i<NUM_BUTTONS; i++){
          if (buttons[i]->pressed()){
            input = i;
            inputGiven = true;
          }
        }
      }
      // Check if the given input was correct. 
      // If so, play the note and continue on to wait for the next input
      if ((inputGiven) and (input == sequence[responses])){
        noTone(SPEAKER_PIN);
        tone(SPEAKER_PIN, TONES[input], 250);
        displayColor(COLORS[input]);
        delay(250);
        displayColor(CRGB::Black);
        responses++;
      } else {
        // If it was wrong, play the "Wrong button" sound and flash the button you were supposed to press
        noTone(SPEAKER_PIN);
        tone(SPEAKER_PIN, WRONG_TONE, 750);
        for(int i=0; i<7; i++){
          displayColor(COLORS[sequence[responses]]);
          delay(100);
          displayColor(CRGB::Black);
          delay(100);
        }
        inGame = false;
      }  
    }
    //Now that the sequence has been reentered, we can go back to the start of the while loop, and get a new 
  }
  // The game has ended, so loop forever
  bool waiting = true;
  while(waiting){
    for (int i=0; i<NUM_BUTTONS; i++){
          if (buttons[i]->pressed()){
            waiting=false;
          }
        }  
  }
  noTone(SPEAKER_PIN);
  tone(SPEAKER_PIN, 880, 40);
  delay(100);
  tone(SPEAKER_PIN, 880, 60);
  delay(500);
  // TODO: Add a way to start a new game using a start button
}

void displayColor(CRGB color){
  for (int i=0; i<NUM_LIGHTS; i++){
    leds[i] = color;
  }
  FastLED.show();
}
