/**************************************************************
  File:     Lab2Part1.ino
  Contents: This program explores the provided library functions.
            The program gets characters from the keyboard to 
            control the motors, print values from the serial
            monitor, read the light sensor, and test the bumper
            switches.
  Notes:    Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  01/23/15  TMO  program created
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Roachlib.h>
#include <Timers.h>

/*---------------- Module Function Prototypes ---------------*/
unsigned char TestForKey(void);
void RespToKey(void);
void GoForward(void);
void GoBackward(void);
void GoRight(void);
void GoLeft(void);
void StopMotors(void);
void PrintLight(void);
void KeyErr(void);
void BumpTest(void);

/*---------------- Arduino Main Functions -------------------*/
void setup() {  // setup() function required for Arduino
  Serial.begin(9600);
  Serial.println("Hello world!");
  RoachInit();
}

void loop() {  // loop() function required for Arduino
  if (TestForKey()) RespToKey();
}

/*---------------- Module Functions -------------------------*/


unsigned char TestForKey(void) {
  unsigned char KeyEventOccurred;
  KeyEventOccurred = Serial.available();
  return KeyEventOccurred;
}

/*------- Function that gets character from keyboard -------*/
void RespToKey(void) {
  unsigned char theKey;
  theKey = Serial.read();
  
  if(theKey == 'w') {
    GoForward();
  } else if (theKey == 's') {
    GoBackward();
  } else if (theKey == 'd') {
    GoRight();
  } else if (theKey == 'a') {
    GoLeft();
  } else if (theKey == 'x') {
    StopMotors();
  } else if (theKey == 'l') {
    PrintLight();
  } else if (theKey == 'b') {
    TestBump();
  } else {
    KeyErr();
  }
}

/* ------- Functions that explore motor control -------------*/
void GoForward(void) {
  LeftMtrSpeed(10);
  RightMtrSpeed(10);
}

void GoBackward(void) {
  LeftMtrSpeed(-10);
  RightMtrSpeed(-10);
}

void GoLeft(void) {
  LeftMtrSpeed(-10);
  RightMtrSpeed(10);
}

void GoRight(void) {
  LeftMtrSpeed(10);
  RightMtrSpeed(-10);
}

void StopMotors(void) {
  LeftMtrSpeed(0);
  RightMtrSpeed(0);
}

/*----- Function that reads the light sensor --------------*/
void PrintLight(void) {
  unsigned int Light = LightLevel();
  Serial.print("Light level is ");
  Serial.println(Light);
}

/*-------- Error if key was not specified above -----------*/
void KeyErr(void) {
  Serial.println("Invalid key. Try Again");
}

/*----------------- Test Bumper Switches ------------------*/
void TestBump(void) {
  unsigned char bumper = ReadBumpers();
  if (bumper == 0xF0) {
    Serial.println("No bumpers hit");
  } else {
    Serial.print("Bumper output: ");
    Serial.println(bumper,HEX);
  }
}
