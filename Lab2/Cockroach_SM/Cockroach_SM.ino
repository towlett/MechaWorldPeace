// Written for #8 Ultra Magnus

/*---------------- Includes ---------------------------------*/
#include <Roachlib.h>
#include <Timers.h>

/*---------------- Module Defines ---------------------------*/
#define HI_LIGHT_THRESHOLD    125
#define LO_LIGHT_THRESHOLD    100
#define ONE_SEC            1000
#define TIME_INTERVAL      ONE_SEC
#define HEARTBEAT_LED      13
#define BACKUPTIME         ONE_SEC
#define FORWARD            1
#define BACKWARD           2
#define STOP               3

/*----------------Module Variables --------------------------*/
static unsigned char state = FORWARD; // Global variable for the state 

/*---------------- Module Function Prototypes ---------------*/
unsigned char TestForLightOn(void);
void RespToLightOn(void);
unsigned char TestForLightOff(void);
void RespToLightOff(void);
unsigned char TestForBump(void);
void RespToBump(void);
unsigned char TestBackupExpired(void);

/*---------------- Arduino Main Functions -------------------*/
void setup() {  // setup() function required for Arduino
  Serial.begin(9600);
  Serial.println("Starting Program...");
  RoachInit();
}

void loop() {  
  switch(state) {
    case(FORWARD):
      if (TestForBump()) {
        RespToBump();
        state = BACKWARD;
      }
      if (TestForLightOff()) {
        RespToLightOff();
        state = STOP;
      }
      break;
    case(STOP):
      if (TestForLightOn()) {
        RespToLightOn();
        state = FORWARD;
      }
      break;
    case(BACKWARD):
      if (TestBackupExpired()) {
        if (TestForLightOn()) {
          RespToLightOn();
          state = FORWARD;
        } else {
          RespToLightOff();
          state = STOP;
        }
      }
      break;  
  }
}

unsigned char TestBackupExpired(void) {
  return (TMRArd_IsTimerExpired(2) == TMRArd_EXPIRED);
}

unsigned char TestForLightOn(void) {
  char EventOccurred;
  unsigned int ThisLight;  
  ThisLight = LightLevel();
  EventOccurred = (ThisLight >= HI_LIGHT_THRESHOLD);
  return EventOccurred;
}

void RespToLightOn(void) {
  Serial.println("  ON");
   LeftMtrSpeed(10);
   RightMtrSpeed(10);
  state = FORWARD;
}

unsigned char TestForLightOff(void) {
  char EventOccurred;
  unsigned int ThisLight;
  ThisLight = LightLevel();
  EventOccurred = (ThisLight < LO_LIGHT_THRESHOLD);
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
  TMRArd_InitTimer(2, BACKUPTIME);
  unsigned char bumper;
  bumper = GET_SHARED_BYTE();
  
  switch (bumper) {
    case (0xD0):  Serial.println("  Front Right..."); 
      LeftMtrSpeed(10);
      RightMtrSpeed(0);
      break;
    case (0xE0):  Serial.println("  Front Left...");
      LeftMtrSpeed(0);
      RightMtrSpeed(10);
      break;
    case (0x70):  Serial.println("  Back Left..."); 
      LeftMtrSpeed(-10);
      RightMtrSpeed(0);
      break;
    case (0xB0):  Serial.println("  Back Right..."); 
      LeftMtrSpeed(0);
      RightMtrSpeed(-10);
      break;

    case (0xC0):  Serial.println("  Both Front ...");
      LeftMtrSpeed(10);
      RightMtrSpeed(0);
      break;
    case (0x30):  Serial.println("  Both Back...");
      LeftMtrSpeed(-10);
      RightMtrSpeed(0);
      break;
    case (0x90):  Serial.println("  Both Right...");
      LeftMtrSpeed(-10);
      RightMtrSpeed(0);
      break;
    case (0x60):  Serial.println("  Both Left...");
      LeftMtrSpeed(0);
      RightMtrSpeed(10);
      break;

    // should never get here unless borked
    default:      Serial.print("  What is this I don�t even->");
                  Serial.println(bumper,HEX);
  }
}
