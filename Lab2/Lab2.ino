/**************************************************************
  File:      Lab2Part1.ino
  Contents:  This program demonstrates the use of the software
              library for interfacing withe the ME210 Cockroach
              Roachlib.cpp and the Timer.cpp library.
  Notes:    Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  01/21/12  RMO  program created
  01/29/12  RNO  fixed bug in TestForLightOff and TestForLightOn
                  with data types (changed char to int for returns
                  from LightLevel() function).
  20150119  KLG  simplified TestForLightOff and TestForLightOn
  01/21/15  TMO  Modified to include simple test programs
**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <Roachlib.h>
#include <Timers.h>

/*---------------- Module Defines ---------------------------*/
#define LIGHT_THRESHOLD    125
#define ONE_SEC            1000
#define TIME_INTERVAL      ONE_SEC
#define HEARTBEAT_LED      13

/*---------------- Module Function Prototypes ---------------*/
unsigned char TestForKey(void);
void RespToKey(void);
unsigned char TestForLightOn(void);
void RespToLightOn(void);
unsigned char TestForLightOff(void);
void RespToLightOff(void);
unsigned char TestForBump(void);
void RespToBump(void);
unsigned char TestTimerExpired(void);
void RespTimerExpired(void);

/*---------------- Arduino Main Functions -------------------*/
void setup() {  // setup() function required for Arduino
  Serial.begin(9600);
  Serial.println("Starting RoachProof...");
  RoachInit();
  TMRArd_InitTimer(0, TIME_INTERVAL);
}

void loop() {  // loop() function required for Arduino
  if (TestForKey()) RespToKey();
  if (TestForLightOn()) RespToLightOn();
  if (TestForLightOff()) RespToLightOff();
  if (TestForBump()) RespToBump();
  if (TestTimerExpired()) RespTimerExpired();
}

/*---------------- Module Functions -------------------------*/
unsigned char TestForKey(void) {
  unsigned char KeyEventOccurred;
  
  KeyEventOccurred = Serial.available();
  return KeyEventOccurred;
}

void RespToKey(void) {
  unsigned char theKey;
  
  theKey = Serial.read();
  
  Serial.print(theKey);
  Serial.print(", ASCII=");
  Serial.println(theKey,HEX);
  
  if(theKey == 'w') {
    LeftMtrSpeed(10);
    RightMtrSpeed(10);
  } else if (theKey == 's') {
    LeftMtrSpeed(-10);
    RightMtrSpeed(-10);
  } else if (theKey == 'd') {
    LeftMtrSpeed(10);
    RightMtrSpeed(-10);
  } else if (theKey == 'a') {
    LeftMtrSpeed(-10);
    RightMtrSpeed(10);
  } else if (theKey == 'x') {
    LeftMtrSpeed(0);
    RightMtrSpeed(0);
  }
  
}

unsigned char TestForLightOn(void) {
  char EventOccurred;
  unsigned int ThisLight;
  static unsigned int LastLight = 0;
  
  ThisLight = LightLevel();
  EventOccurred = ((ThisLight >= LIGHT_THRESHOLD)
                       && (LastLight < LIGHT_THRESHOLD));

  LastLight = ThisLight;
  return EventOccurred;
}

void RespToLightOn(void) {
  Serial.println("  ON");
  LeftMtrSpeed(10);
  RightMtrSpeed(-10);
}

unsigned char TestForLightOff(void) {
  char EventOccurred;
  unsigned int ThisLight;
  static unsigned int LastLight = 0;

  ThisLight = LightLevel();
  EventOccurred = ((ThisLight < LIGHT_THRESHOLD)
                       && (LastLight >= LIGHT_THRESHOLD));

  LastLight = ThisLight;
  return EventOccurred;
}

void RespToLightOff(void) {
  Serial.println("  OFF");
  LeftMtrSpeed(0);
  RightMtrSpeed(0);
}

unsigned char TestForBump(void) {
  static unsigned char LastBumper = 0xF0;
  unsigned char bumper;
  unsigned char EventOccurred;
  
  bumper = ReadBumpers();
  EventOccurred = ((bumper != 0xF0) && (bumper != LastBumper));
  if (EventOccurred) {
    SET_SHARED_BYTE_TO(bumper);
    LastBumper = bumper;
  }
  return EventOccurred;
}

void RespToBump(void) {
    unsigned char bumper;

    bumper = GET_SHARED_BYTE();

    // display which bumper(s) were hit
    switch (bumper) {
        case (0xD0):  Serial.println("  Front Right..."); break;
        case (0xE0):  Serial.println("  Front Left..."); break;
        case (0x70):  Serial.println("  Back Left..."); break;
        case (0xB0):  Serial.println("  Back Right..."); break;

        case (0xC0):  Serial.println("  Both Front ..."); break;
        case (0x30):  Serial.println("  Both Back..."); break;
        case (0x90):  Serial.println("  Both Right..."); break;
        case (0x60):  Serial.println("  Both Left..."); break;

		// should never get here unless borked
        default:      Serial.print("  What is this I don�t even->");
                      Serial.println(bumper,HEX);
    }
}

unsigned char TestTimerExpired(void) {
  return (unsigned char)TMRArd_IsTimerExpired(0);
}

void RespTimerExpired(void) {
  static int Time = 0;

  TMRArd_InitTimer(0, TIME_INTERVAL);

  Serial.print(" time:");
  Serial.print(++Time);
  Serial.print(" light:");
  Serial.println(LightLevel(),DEC);
}

