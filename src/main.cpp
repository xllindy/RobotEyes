#include <Arduino.h>
#include <Servo.h>

Servo eyeServo;
Servo nodServo;

const int eyeServoPin = 9;
const int nodServoPin = 10;

const int rotateLeft = 2;
const int rotateRight = 3;
const int rotateUp = 4;
const int rotateDown = 5;

int oogPos = 90;
int upDownPos = 90;

const int stap = 5;

void setup() {
  eyeServo.attach(eyeServoPin);
  nodServo.attach(nodServoPin);

  eyeServo.write(oogPos);
  nodServo.write(upDownPos);

  pinMode(rotateLeft, INPUT_PULLUP);
  pinMode(rotateRight, INPUT_PULLUP);
  pinMode(rotateUp, INPUT_PULLUP);
  pinMode(rotateDown, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(rotateLeft) == LOW) {
    oogPos = oogPos - stap;
    if (oogPos < 0) oogPos = 0;
    eyeServo.write(oogPos);
    delay(100);
  }

  if (digitalRead(rotateRight) == LOW) {
    oogPos = oogPos + stap;
    if (oogPos > 180) oogPos = 180;
    eyeServo.write(oogPos);
    delay(100);
  }

  if (digitalRead(rotateUp) == LOW) {
    upDownPos = upDownPos - stap;
    if (upDownPos < 0) upDownPos = 0;
    nodServo.write(upDownPos);
    delay(100);
  }

  if (digitalRead(rotateDown) == LOW) {
    upDownPos = upDownPos + stap;
    if (upDownPos > 180) upDownPos = 180;
    nodServo.write(upDownPos);
    delay(100);
  }
}