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

const int micLeftPin = A0;
const int micRightPin = A1;

int oogPos = 90;
int targetPos = 90;
int upDownPos = 90;

const int stap = 5;

int leftOffset = 0;
int rightOffset = 0;

int noiseThreshold = 5;
int dominanceGap = 5;

int winner = 0;

unsigned long lastSwitchTime = 0;
int lockTime = 300; 

int smoothSpeed = 2; 

void calibrateMics() {
  long leftSum = 0;
  long rightSum = 0;

  for (int i = 0; i < 100; i++) {
    leftSum += analogRead(micLeftPin);
    rightSum += analogRead(micRightPin);
    delay(10);
  }

  leftOffset = leftSum / 100;
  rightOffset = rightSum / 100;
}

void setup() {
  eyeServo.attach(eyeServoPin);
  nodServo.attach(nodServoPin);

  eyeServo.write(oogPos);
  nodServo.write(upDownPos);

  pinMode(rotateLeft, INPUT_PULLUP);
  pinMode(rotateRight, INPUT_PULLUP);
  pinMode(rotateUp, INPUT_PULLUP);
  pinMode(rotateDown, INPUT_PULLUP);

  Serial.begin(9600);

  calibrateMics();
}

void loop() {

  if (digitalRead(rotateLeft) == LOW) {
    targetPos -= stap;
    if (targetPos < 0) targetPos = 0;
    delay(100);
  }

  if (digitalRead(rotateRight) == LOW) {
    targetPos += stap;
    if (targetPos > 180) targetPos = 180;
    delay(100);
  }

  if (digitalRead(rotateUp) == LOW) {
    upDownPos -= stap;
    if (upDownPos < 0) upDownPos = 0;
    nodServo.write(upDownPos);
    delay(100);
  }

  if (digitalRead(rotateDown) == LOW) {
    upDownPos += stap;
    if (upDownPos > 180) upDownPos = 180;
    nodServo.write(upDownPos);
    delay(100);
  }

  int leftRaw = analogRead(micLeftPin);
  int rightRaw = analogRead(micRightPin);

  int leftLevel = abs(leftRaw - leftOffset);
  int rightLevel = abs(rightRaw - rightOffset);

  leftLevel *= 1.2;

  leftLevel = max(0, leftLevel - noiseThreshold);
  rightLevel = max(0, rightLevel - noiseThreshold);

  unsigned long now = millis();

  if (now - lastSwitchTime > lockTime) {

    if (leftLevel > rightLevel + dominanceGap && winner != -1) {
      winner = -1;
      lastSwitchTime = now;
    }
    else if (rightLevel > leftLevel + dominanceGap && winner != 1) {
      winner = 1;
      lastSwitchTime = now;
    }
  }

  if (winner == -1) {
    targetPos = 20;
  }
  else if (winner == 1) {
    targetPos = 160;
  }

  if (oogPos < targetPos) {
    oogPos += smoothSpeed;
    if (oogPos > targetPos) oogPos = targetPos;
    eyeServo.write(oogPos);
  }
  else if (oogPos > targetPos) {
    oogPos -= smoothSpeed;
    if (oogPos < targetPos) oogPos = targetPos;
    eyeServo.write(oogPos);
  }

  // Debug
  Serial.print("L: ");
  Serial.print(leftLevel);
  Serial.print(" R: ");
  Serial.print(rightLevel);
  Serial.print(" Winner: ");
  if (winner == -1) Serial.print("Left");
  else if (winner == 1) Serial.print("Right");
  else Serial.print("None");
  Serial.print(" Pos: ");
  Serial.println(oogPos);

  delay(20);
}