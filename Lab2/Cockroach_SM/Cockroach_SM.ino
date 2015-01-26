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
static unsigned char state; // Global variable for the state 

/*---------------- Module Function Prototypes ---------------*/
unsigned char TestForLightOn(void);
void RespToLightOn(void);
unsigned char TestForLightOff(void);
void RespToLightOff(void);
unsigned char TestForBump(void);
void RespToBump(void);
unsigned char TestBackupExpired(void);
void RespBackupExpired(void);

/*---------------- Arduino Main Functions -------------------*/
void setup() {  // setup() function required for Arduino
  Serial.begin(9600);
  Serial.println("Starting Program...");
  RoachInit();
  TMRArd_InitTimer(0, TIME_INTERVAL);
}

void loop() {
  if (TestForLightOn()) RespToLightOn();
  if (TestForLightOff()) RespToLightOff();
  if (TestForBump()) RespToBump();
  if (TestBackupExpired()) RespBackupExpired();
  //if (TMRArd_IsTimerExpired(2) == TMRArd_EXPIRED) RespBackupExpired();
  
  switch(state) {
    case(FORWARD):
      LeftMtrSpeed(10);
      RightMtrSpeed(10);
      break;
    case(STOP):
      LeftMtrSpeed(0);
      RightMtrSpeed(0);
      break;
    case(BACKWARD):
        unsigned char bumper;
        bumper = GET_SHARED_BYTE();
        state = BACKWARD;
        // display which bumper(s) were hit
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
      break;  
  }
}

unsigned char TestBackupExpired(void) {
  return (TMRArd_IsTimerExpired(2) == TMRArd_EXPIRED);
}

unsigned char TestForLightOn(void) {
  char EventOccurred;
  unsigned int ThisLight;
  static unsigned int LastLight = 0;
  
  ThisLight = LightLevel();
  EventOccurred = ((ThisLight >= HI_LIGHT_THRESHOLD)
                       && (LastLight < HI_LIGHT_THRESHOLD));

  LastLight = ThisLight;
  return EventOccurred;
}

void RespToLightOn(void) {
  Serial.println("  ON");
  state = FORWARD;
}

unsigned char TestForLightOff(void) {
  char EventOccurred;
  unsigned int ThisLight;
  static unsigned int LastLight = 0;

  ThisLight = LightLevel();
  EventOccurred = ((ThisLight < LO_LIGHT_THRESHOLD)
                       && (LastLight >= LO_LIGHT_THRESHOLD));

  LastLight = ThisLight;
  return EventOccurred;
}

void RespToLightOff(void) {
  Serial.println("  OFF");
  state = STOP;
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
  state = BACKWARD;
}

void RespBackupExpired(void) {
  Serial.println("Backup Timer Expired");
  LeftMtrSpeed(10);
  RightMtrSpeed(10);
  TMRArd_ClearTimerExpired(2);
  if (LightLevel() >= LO_LIGHT_THRESHOLD) {
    state = FORWARD;
  } else {
    state = STOP;
  }
}
