// Authors: Kaibo Huang, Luck Xing
// Course: TER4M1-1
// Date: Jan 22, 2023
// Assignment: Culminating Activity
// Description: This is code for the self tying laces. 
// This code has the ability to read the pressure inside the shoe and automatically tighten and lossen based on the pressure
// You can also tighten, loosen, and reset the tightness of the shoe by pressing buttons manually

#include <Servo.h>

#define FORCE_SENSOR_PIN A0 //declare force pin

// declaring servo pins
int servoPin1 = 9;
Servo Servo1;

//button pins
const int incrementButtonPin = 2;
const int decrementButtonPin = 3;
const int resetButtonPin = 4;

int variableToManipulate = 90;  // Initial value for the servo

//initial button states
int previousIncrementButtonState = HIGH;
int previousDecrementButtonState = HIGH;
int previousResetButtonState = HIGH;

//assign force reading pins
unsigned long lastForceAbove500Time = 0;
unsigned long lastForceBelow500Time = 0;
bool tighteningInProgress = false;
bool looseningInProgress = false;

void setup() {
  Serial.begin(9600);

  //assign pins to each button
  pinMode(incrementButtonPin, INPUT_PULLUP);
  pinMode(decrementButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);

  //attach servo to pin
  Servo1.attach(servoPin1);
}

void loop() {
  // read force reading
  int analogReading = analogRead(FORCE_SENSOR_PIN);

  // get button state
  int currentIncrementButtonState = digitalRead(incrementButtonPin);
  int currentDecrementButtonState = digitalRead(decrementButtonPin);
  int currentResetButtonState = digitalRead(resetButtonPin);

  // Increment button logic
  if (currentIncrementButtonState == HIGH && previousIncrementButtonState == LOW) {
    variableToManipulate += 90;

    Servo1.write(variableToManipulate);  // Move the servo to the new position
    delay(2000);  // Delay for 2 seconds to avoid rapid button presses
  }

  // Decrement button logic
  if (currentDecrementButtonState == HIGH && previousDecrementButtonState == LOW) {
    variableToManipulate -= 90;
    
    Servo1.write(variableToManipulate);  // Move the servo to the new position
    delay(2000);  // Delay for 2 seconds to avoid rapid button presses
  }

  // Reset button logic
  if (currentResetButtonState == HIGH && previousResetButtonState == LOW) {
    variableToManipulate = 90;
   
    Servo1.write(variableToManipulate);  // Move the servo to the reset position
    delay(2000);  // Delay for 2 seconds to avoid rapid button presses
  }

  // Force sensor logic
  if (analogReading > 500) {
    if (!tighteningInProgress && millis() - lastForceBelow500Time > 1000) {
      // Force has been consistently above 500 for a second
      Servo1.write(90);  // Set servo to 90 degrees (tighten)
      tighteningInProgress = true;
      looseningInProgress = false;
    }
  } else {
    if (!looseningInProgress && millis() - lastForceAbove500Time > 1000) {
      // Force has been consistently below 500 for a second
      Servo1.write(0);  // Set servo to 0 degrees (loosen)
      looseningInProgress = true;
      tighteningInProgress = false;
    }
  }

  // Reset the tightening/loosening flags when force is within the threshold
  if (tighteningInProgress && analogReading <= 500) {
    tighteningInProgress = false;
  }

  if (looseningInProgress && analogReading > 500) {
    looseningInProgress = false;
  }

  // Update button states for the next iteration
  previousIncrementButtonState = currentIncrementButtonState;
  previousDecrementButtonState = currentDecrementButtonState;
  previousResetButtonState = currentResetButtonState;
}
