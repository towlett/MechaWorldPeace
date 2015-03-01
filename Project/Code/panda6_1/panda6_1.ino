
/**************************************************************
  File:      panda3.ino
  Contents:  A boss ass state machine
  Notes:    Target: Arduino Uno R1 & R2
            Arduino IDE version: 1.0.6 & 1.5.8 BETA

  History:
  when      who  what/why
  ----      ---  -------------------------------------------
  02/22/15  TM  program created

**************************************************************/

/*---------------- Includes ---------------------------------*/
#include <NewPing.h>
#include <Timers.h>
#include <Servo.h>

/*---------------- Module Defines ---------------------------*/
//STATES
#define drivingbackwall     1
#define findingsidewall     2
#define drivingsidewall     3
#define findIR              4
#define shooting            5
#define game_over           6
#define spray               7

//MOTORS
#define enable_pin_right    5
#define enable_pin_left     6
#define dir_pin_right       7
#define dir_pin_left        8

//IR
#define Beacon_Pin          9

//ULTRASOUND
#define trigPinleft         2
#define echoPinleft         4
#define trigPinright        12
#define echoPinright        13
#define last_ping_right     0
#define last_ping_left      1

//SERVOS
#define servopin            10
#define MAX_POS 160
#define SHOOTER_STEP 1
#define SHOOTER_DELAY 10 
#define SHOOTER_TIMER 0
#define SHOT_CLOCK_TIMER 1
#define b_press_pin        3


//LEDs
#define redled1            A0        // NEW added code
#define blueled            A1
#define redled2            A2
#define whiteled           A3
#define redled3            A4

//TIMERS
#define minturntimer        0
#define maxturntimer        1
#define pingtimer           2
#define SHOOTER_TIMER       3
#define SHOT_CLOCK_TIMER    4
#define WHITE_LED_TIMER     5
#define BLUE_LED_TIMER      6
#define RED_LED_TIMER       7
#define MAX_SEARCH_TIMER    8
#define GAME_TIMER          9
#define SPRAY_TIMER         10

#define ADDITIONAL_TURN_TIME 250

/*---------------- Module Variables --------------------------*/
//STATES
static unsigned char state;

//ULTRASOUND
static unsigned char last_ping;

//IR
static unsigned long period_total;
static unsigned long pulse_freq;
static unsigned int period_high;
static unsigned int period_low;

Servo shooter;
Servo presser;

/*---------------- Module Function Prototypes ---------------*/
//MOTORS
void drive_forward(void);
void drive_backward(void);
void drive_stop(void);
void spin_left(void);
void spin_right(void);
void swerve_left(void);
void swerve_right(void);
void drive_brake(void);
void driving_algo(unsigned int distanceright, unsigned int distanceleft);
void drive_back_wall(void);
void find_side_wall(void);
void drive_side_wall(void);
void drive_brake_backup(void);
void spin_right_fast(void);

//SHOOTING
unsigned char ShooterTimerExp(void);
void ChangeShooterPos(void);
unsigned char ShotClockExp(void);
void RespShotClock(void);

//ULTRASOUND
NewPing left_ping(trigPinleft, echoPinleft, 150);
NewPing right_ping(trigPinright, echoPinright, 150);
unsigned int ultrasonicPing_left(void);
unsigned int ultrasonicPing_right(void);

//IR Finding
void FindIR(void);
void spin_reverse(void);

// LEDs
void ScanningLED(void);
unsigned char LockedOnLED(void);
void OffLED(void);

void game_timer(void);

/*---------------- Arduino Main Functions -------------------*/

void setup() {  // setup() function required for Arduino 
  // MOTOR SETUP 
  pinMode(dir_pin_right, OUTPUT);
  pinMode(dir_pin_left, OUTPUT);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 0);
  
  // Ultrasonic SETUP
  pinMode(trigPinright, OUTPUT);
  pinMode(echoPinright, INPUT);
  pinMode(trigPinleft, OUTPUT);
  pinMode(echoPinleft, INPUT);
  
  // LED SETUP
  pinMode(redled1, OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(redled2, OUTPUT);
  pinMode(whiteled, OUTPUT);
  pinMode(redled3, OUTPUT);
  digitalWrite(redled1, LOW);
  digitalWrite(blueled, LOW);
  digitalWrite(redled2, LOW);
  digitalWrite(blueled, LOW);
  digitalWrite(redled3, LOW);
  
  drive_forward();
  pinMode(Beacon_Pin, INPUT);
  
  Serial.begin(9600);
  Serial.println("Starting Panda");
  state = 1;
  shooter.attach(servopin);
  shooter.write(0);
  presser.attach(b_press_pin);
  presser.write(180);
  TMRArd_InitTimer(GAME_TIMER, 125000);
  TMRArd_InitTimer(SPRAY_TIMER, 60000);
}


void loop() {
//static unsigned int distanceleft;
//static unsigned int distanceright;
  
  switch(state) {
    case(drivingbackwall):
      //game_timer();
      drive_back_wall();
      LockedOnLED();
      break;
      
    case(findingsidewall):
      //game_timer();
      find_side_wall();
      break;
      
    case(drivingsidewall):
      //game_timer();
      drive_side_wall();
      break;
      
    case(findIR):
      //game_timer();
      ScanningLED();
      TMRArd_InitTimer(MAX_SEARCH_TIMER, 4000);
      FindIR();
      break;
      
    case(shooting):
      game_timer();
      if(LockedOnLED() < 5){
        LockedOnLED(); 
      }
      if (ShotClockExp()) {
        RespShotClock();
      }
      if (ShooterTimerExp()) {
        ChangeShooterPos();
      }
      if (TMRArd_IsTimerExpired(SPRAY_TIMER))
      {
        state = spray;
      }
     break;
     
     case(spray):
       static unsigned char count = 0;
       static unsigned char shotoccurred = 0;
       static unsigned char startedspray = 0;
       
       game_timer();
       
//       if (startedspray == 0) {
//         spin_right();
//         delay(500);
//         drive_stop();
//         startedspray = 1;
//       }

       if ((count%10 == 0) && (shotoccurred == 0)) {
         Serial.println("RIGHT");
         spin_right();
         delay(750);
         drive_stop();
         shotoccurred = 1;
       } else if (shotoccurred == 0) {
         Serial.println("LEFT");
         spin_left();
         delay(200);
         drive_stop();
         shotoccurred = 1;
       }
       
//       if ((count%8 == 0) && (occurred == 0)) {
//         spin_right();
//         delay(400);
//         drive_stop();
//         occurred = 1;
//       } else if ((count%4 == 0) && (occurred == 0)) {
//         spin_left();
//         delay(200);
//         drive_stop();
//         occurred = 1;
//       }
       
       
       if (ShotClockExp()) {
         count++;
         RespShotClock();
         shotoccurred = 0;
       }
       if (ShooterTimerExp()) {
        ChangeShooterPos();
       }
       
     break;
     
    case(game_over):
     drive_stop();
     break;   
  }
}

/*---------------- Module Functions -------------------------*/


//ULTRASONIC
unsigned int ultrasonicPing_right(void) {
  unsigned int distanceright = right_ping.ping()/US_ROUNDTRIP_CM;
  if (distanceright == 0) {
    distanceright = 150;
  }
  //Serial.print("right");
  //Serial.println(distanceright);
  return distanceright;
}

unsigned int ultrasonicPing_left(void) {
  unsigned int distanceleft = left_ping.ping()/US_ROUNDTRIP_CM;
  if (distanceleft == 0) {
    distanceleft = 150;
  }
  //Serial.print("left");
  //Serial.println(distanceleft);
  return distanceleft;
}

//MOTORS
void drive_forward(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 250);
  analogWrite(enable_pin_left, 250);
}

void drive_backward(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 200);
  analogWrite(enable_pin_left, 200);
}

void drive_stop(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 0);
}

void spin_right(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 150);
  analogWrite(enable_pin_left, 150);
}

void spin_right_fast(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 175);
  analogWrite(enable_pin_left, 175);
}

void spin_left(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 150);
  analogWrite(enable_pin_left, 150);
}

void swerve_right(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 200);
}

void swerve_left(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 200);
  analogWrite(enable_pin_left, 0);
}

void drive_brake(void) {
  digitalWrite(dir_pin_right, LOW);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 250);
  analogWrite(enable_pin_left, 250);
  delay(10);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 0);
}

void drive_brake_backup(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, HIGH);
  analogWrite(enable_pin_right, 250);
  analogWrite(enable_pin_left, 250);
  delay(10);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 0);
}
  

void driving_algo(unsigned int distanceright, unsigned int distanceleft) {
  if (distanceright > distanceleft) {
    swerve_left();
  } else if (distanceleft > distanceright) {
    swerve_right();
  } else {
    drive_forward();
  }
}

void drive_back_wall(void) {
  static unsigned int distanceleft;
  static unsigned int distanceright;
  
  if ((last_ping == last_ping_right) && TMRArd_IsTimerExpired(pingtimer)) {
    distanceleft = ultrasonicPing_left();
    TMRArd_InitTimer(pingtimer, 50);
    last_ping = last_ping_left;    
  }
  if ((last_ping == last_ping_left) && TMRArd_IsTimerExpired(pingtimer)) {
    distanceright = ultrasonicPing_right();
    TMRArd_InitTimer(pingtimer, 50);
    last_ping = last_ping_right;
  }
  
  driving_algo(distanceright, distanceleft);
  
  if ( distanceleft <= 12 && distanceright <= 12 && abs(distanceleft - distanceright < 2)) {
    drive_brake();
    spin_right_fast();
    TMRArd_InitTimer(minturntimer, 1600);
    TMRArd_InitTimer(maxturntimer, 4000);
    Serial.println("on to 2");
    state = findingsidewall;
  }
}

void find_side_wall(void) {
  static unsigned int distanceleft;
  static unsigned int distanceright;
  
  if (TMRArd_IsTimerExpired(minturntimer)) {
    Serial.println("expired");
    if ((last_ping == last_ping_right) && TMRArd_IsTimerExpired(pingtimer)) {
      distanceleft = ultrasonicPing_left();
      TMRArd_InitTimer(pingtimer, 50);
      last_ping = last_ping_left;
    }
    if ((last_ping == last_ping_left) && TMRArd_IsTimerExpired(pingtimer)) {
      distanceright = ultrasonicPing_right();
      TMRArd_InitTimer(pingtimer, 50);
      last_ping = last_ping_right;
    }
    if ( abs(distanceleft - distanceright) <=2 ) {
      drive_forward();
      Serial.println("on to 3");
      state = drivingsidewall;
    }
  }
  
  if (TMRArd_IsTimerExpired(maxturntimer)) {
    spin_left();
    if ((last_ping == last_ping_right) && TMRArd_IsTimerExpired(pingtimer)) {
      distanceleft = ultrasonicPing_left();
      TMRArd_InitTimer(pingtimer, 50);
      last_ping = last_ping_left;
    }
    if ((last_ping == last_ping_left) && TMRArd_IsTimerExpired(pingtimer)) {
      distanceright = ultrasonicPing_right();
      TMRArd_InitTimer(pingtimer, 50);
      last_ping = last_ping_right;
    }
    if ( abs(distanceleft - distanceright) <=1 ) {
      drive_forward();
      Serial.println("on to 3");
      state = drivingsidewall;
    }
  }
}

void drive_side_wall(void) {
  static unsigned int distanceleft = 150;
  static unsigned int distanceright = 150;
  //Serial.println("made it to drive function");
  
  if ((last_ping == last_ping_right) && TMRArd_IsTimerExpired(pingtimer)) {
    distanceleft = ultrasonicPing_left();
    TMRArd_InitTimer(pingtimer, 50);
    last_ping = last_ping_left;
  }
  if ((last_ping == last_ping_left) && TMRArd_IsTimerExpired(pingtimer)) {
    distanceright = ultrasonicPing_right();
    TMRArd_InitTimer(pingtimer, 50);
    last_ping = last_ping_right;
  }
  
  driving_algo(distanceright, distanceleft);
  
  if ( distanceleft <= 12 && distanceright <= 12 ) {
    while (distanceleft < 9 && distanceright < 9) {
      drive_backward();
      if ((last_ping == last_ping_right) && TMRArd_IsTimerExpired(pingtimer)) {
        distanceleft = ultrasonicPing_left();
        TMRArd_InitTimer(pingtimer, 50);
        last_ping = last_ping_left;    
      }
      if ((last_ping == last_ping_left) && TMRArd_IsTimerExpired(pingtimer)) {
        distanceright = ultrasonicPing_right();
        TMRArd_InitTimer(pingtimer, 50);
        last_ping = last_ping_right;
      }
    }
    Serial.println("distance tripped");
    drive_brake_backup();
    spin_right();
    state = findIR;
  }
}

unsigned char ShooterTimerExp(void) {
  return (TMRArd_IsTimerExpired(SHOOTER_TIMER) == TMRArd_EXPIRED);
}

unsigned char ShotClockExp(void) {
  return (TMRArd_IsTimerExpired(SHOT_CLOCK_TIMER) == TMRArd_EXPIRED);
}

void RespShotClock(void) {
  TMRArd_InitTimer(SHOT_CLOCK_TIMER, 3000);
  ChangeShooterPos();
}

void ChangeShooterPos(void) {
  static int pos = 0;
  static unsigned char dir = 0;
  TMRArd_ClearTimerExpired(SHOOTER_TIMER);
  
  if (dir == 0) {
    pos += SHOOTER_STEP;
    shooter.write(pos);
    presser.write(180 - pos);
    if (pos >= MAX_POS) {
      dir = 1;
    }
  }
  if (dir == 1) {
    pos -= SHOOTER_STEP;
    shooter.write(pos);
    presser.write(180 - pos);
    if (pos <= 0) {
      dir = 0;
    }
  }
  if (pos > 0) {
    TMRArd_InitTimer(SHOOTER_TIMER, SHOOTER_DELAY);
  } 
}

void ScanningLED(void) {
    unsigned int blinktime = 300;
    
    if(digitalRead(blueled) == LOW && digitalRead(whiteled) == LOW){
      TMRArd_InitTimer(BLUE_LED_TIMER, blinktime);
      digitalWrite(blueled, HIGH); 
    }
    
    if(TMRArd_IsTimerExpired(BLUE_LED_TIMER)  == TMRArd_EXPIRED){
       TMRArd_InitTimer(WHITE_LED_TIMER, blinktime);
       TMRArd_ClearTimerExpired(BLUE_LED_TIMER);
       digitalWrite(whiteled, HIGH);
       digitalWrite(blueled, LOW);
    }
    
     if(TMRArd_IsTimerExpired(WHITE_LED_TIMER)  == TMRArd_EXPIRED){
       TMRArd_InitTimer(BLUE_LED_TIMER, blinktime);
       TMRArd_ClearTimerExpired(WHITE_LED_TIMER);
       digitalWrite(blueled, HIGH);
       digitalWrite(whiteled, LOW);
    }
}

unsigned char LockedOnLED(void) {
    static unsigned char ledcyclecount = 0; // number of flashes for LockedOnLED function

    unsigned int blinktime = 175;
    
    if(digitalRead(redled1) == LOW && digitalRead(redled2) == LOW && digitalRead(redled3) == LOW && ledcyclecount == 0) {
      TMRArd_InitTimer(RED_LED_TIMER, blinktime);
      digitalWrite(redled1, HIGH); 
      digitalWrite(redled2, HIGH); 
      digitalWrite(redled3, HIGH); 
      ledcyclecount += 1;
    }
    
    if(TMRArd_IsTimerExpired(RED_LED_TIMER)  == TMRArd_EXPIRED && digitalRead(redled1) == HIGH){
       TMRArd_ClearTimerExpired(RED_LED_TIMER);
       digitalWrite(redled1, LOW); 
       digitalWrite(redled2, LOW); 
       digitalWrite(redled3, LOW); 
       TMRArd_InitTimer(RED_LED_TIMER, blinktime);
       ledcyclecount += 1;
    }
    
    if(TMRArd_IsTimerExpired(RED_LED_TIMER)  == TMRArd_EXPIRED && digitalRead(redled1) == LOW){
       TMRArd_ClearTimerExpired(RED_LED_TIMER);
       digitalWrite(redled1, HIGH); 
       digitalWrite(redled2, HIGH); 
       digitalWrite(redled3, HIGH); 
       TMRArd_InitTimer(RED_LED_TIMER, blinktime);
       ledcyclecount += 1;
    }
    
    return ledcyclecount;
}

void OffLED(void){
    digitalWrite(redled1, LOW);
    digitalWrite(blueled, LOW);
    digitalWrite(redled2, LOW);
    digitalWrite(blueled, LOW);
    digitalWrite(redled3, LOW);
}

void FindIR(void) {
  static int count = 0;
  period_high = pulseIn(Beacon_Pin, HIGH);
  period_low = pulseIn(Beacon_Pin, LOW);
  period_total = period_high + period_low;
  pulse_freq = 1000000/period_total;
  if (pulse_freq>750 && pulse_freq<950) {
	  count++;
  }
  else  {
    count = 0;
  }
  if (count>2) { //will return true once it has registered 5 consecutive highs
    delay(ADDITIONAL_TURN_TIME);
    drive_stop();
    TMRArd_InitTimer(SHOT_CLOCK_TIMER, 3000);
    OffLED();
    state = shooting;       
  }
  if (TMRArd_IsTimerExpired(MAX_SEARCH_TIMER)  == TMRArd_EXPIRED) {
    spin_reverse();
    TMRArd_InitTimer(MAX_SEARCH_TIMER, 4000);
  }
}

void spin_reverse(void) {
  static unsigned char spin_dir = 0;
  if (spin_dir == 0) {
    spin_left();
    spin_dir = 1;
  } else {
    spin_right();
    spin_dir = 0;
  }
}

void game_timer(void) {
 if (TMRArd_IsTimerExpired(GAME_TIMER)  == TMRArd_EXPIRED) {
   state = game_over;
   presser.write(180);
   shooter.write(0);
   OffLED();
 }
}


  


