#include <Button.h>
#include <stdlib.h>

#include "Talkie.h"
#include "Vocab_US_Large.h"

// Ben is editing this code.

const int NUM_LIGHTS = 4;
const int SPEAKER_PIN = 3;

int lightPins[NUM_LIGHTS] = {9, 8, 7, 6};
int buttonPins[NUM_LIGHTS] = {13, 12, 11, 10};
// Frequencies for E4 A4 C#5 E5
int tones[NUM_LIGHTS] = {330, 440, 554, 659};
// int tones[NUM_LIGHTS] = {311, 370, 440, 523};
int WRONG_TONE = 220;

Button *buttons[NUM_LIGHTS];

Talkie voice(true, false);

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  for (int i=0; i<NUM_LIGHTS; i++){
    pinMode(lightPins[i], OUTPUT);
    buttons[i] = new Button(buttonPins[i]);
    buttons[i]->begin();
  }
  Serial.begin(9600);
  randomSeed(analogRead(0) * analogRead(1) * analogRead(2));
}

void loop() {
  // Initialize game
  // Create array to hold the sequence, and a length that shows how long the sequence is so far
  int sequence[100];
  int len = 0;
  
  bool inGame = true;
  while(inGame){
    // Generate random number to be added to sequence
    int next = random(0, NUM_LIGHTS);
    Serial.println(next);
    sequence[len] = next;
    len++;

    // Play the sequence of lights/tones
    // We don't care about any inputs during this so we can use delay();
    delay(750);
    for (int i=0; i<len; i++){
      noTone(SPEAKER_PIN);
      tone(SPEAKER_PIN, tones[sequence[i]], 250);
      digitalWrite(lightPins[sequence[i]], HIGH);
      delay(250);
      digitalWrite(lightPins[sequence[i]], LOW);
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
        for (int i=0; i<NUM_LIGHTS; i++){
          if (buttons[i]->pressed()){
            input = i;
            Serial.print(i);
            Serial.println(" pressed");
            inputGiven = true;
          }
        }
      }
      // Check if the given input was correct. 
      // If so, play the note and continue on to wait for the next input
      if ((inputGiven) and (input == sequence[responses])){
        noTone(SPEAKER_PIN);
        tone(SPEAKER_PIN, tones[input], 250);
        digitalWrite(lightPins[input], HIGH);
        delay(250);
        digitalWrite(lightPins[input], LOW);
        responses++;
      } else {
        // If it was wrong, play the "Wrong button" sound and flash the button you were supposed to press
        noTone(SPEAKER_PIN);
        tone(SPEAKER_PIN, WRONG_TONE, 750);
        for(int i=0; i<7; i++){
          digitalWrite(lightPins[sequence[responses]], HIGH);
          delay(100);
          digitalWrite(lightPins[sequence[responses]], LOW);
          delay(100);
        }
        inGame = false;
      }  
    }
    //Now that the sequence has been reentered, we can go back to the start of the while loop, and get a new 
  }
  // The game has ended, so loop forever
  while(true){}
  // TODO: Add a way to start a new game using a start button
}
