#include <NewPing.h>
#include <Timers.h>

#define drivingbackwall  1
#define findingsidewall  2
#define drivingsidewall  3
#define state4  4

#define enable_pin_right    5
#define enable_pin_left     6
#define dir_pin_right       7
#define dir_pin_left        8

#define Beacon_Pin          9

#define trigPinleft         10
#define echoPinleft         11
#define trigPinright        12
#define echoPinright        13

#define minturntimer 0
#define maxturntimer 1
#define pingtimer 2

#define last_ping_right 0
#define last_ping_left 1

static unsigned char state;
static unsigned char last_ping;

static unsigned long period_total;
static unsigned long pulse_freq;
static int count = 0;
static unsigned int period_high;
static unsigned int period_low;

unsigned int ultrasonicPing_left(void);
unsigned int ultrasonicPing_right(void);
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

NewPing left_ping(trigPinleft, echoPinleft, 150);
NewPing right_ping(trigPinright, echoPinright, 150);


void setup() {  // setup() function required for Arduino 
  // MOTOR SETUP 
  pinMode(dir_pin_right, OUTPUT);
  pinMode(dir_pin_left, OUTPUT);
  analogWrite(enable_pin_right, 0);
  analogWrite(enable_pin_left, 0);
  
  pinMode(trigPinright, OUTPUT);
  pinMode(echoPinright, INPUT);
  pinMode(trigPinleft, OUTPUT);
  pinMode(echoPinleft, INPUT);
  
  drive_forward();
  pinMode(Beacon_Pin, INPUT);
  
  Serial.begin(9600);
  Serial.println("Starting Panda");
  state = 1;
}


void loop() {
static unsigned int distanceleft;
static unsigned int distanceright;
  
  switch(state) {
    case(drivingbackwall):
      drive_back_wall();
      break;
      
    case(findingsidewall):
      find_side_wall();
      break;
      
    case(drivingsidewall):
      drive_side_wall();
      break;
      
      case(state4):
        period_high = pulseIn(Beacon_Pin, HIGH);
        period_low = pulseIn(Beacon_Pin, LOW);
        Serial.print(period_high);
        Serial.println(" ");
        Serial.println(period_low);
        period_total = period_high + period_low;
        pulse_freq = 1000000/period_total;
        if (pulse_freq>750 && pulse_freq<950) {
	  count++;
          Serial.println(count);
        }
        else  {
          count = 0;
        }
        if (count>5) { //will return true once it has registered 5 consecutive highs
          drive_stop();
        }
        break;
  }
}

unsigned int ultrasonicPing_right(void) {
  unsigned int distanceright = right_ping.ping()/US_ROUNDTRIP_CM;
  if (distanceright == 0) {
    distanceright = 150;
  }
  Serial.print("right");
  Serial.println(distanceright);
  return distanceright;
}

unsigned int ultrasonicPing_left(void) {
  unsigned int distanceleft = left_ping.ping()/US_ROUNDTRIP_CM;
  if (distanceleft == 0) {
    distanceleft = 150;
  }
  Serial.print("left");
  Serial.println(distanceleft);
  return distanceleft;
}

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
  analogWrite(enable_pin_right, 175);
  analogWrite(enable_pin_left, 175);
}

void spin_left(void) {
  digitalWrite(dir_pin_right, HIGH);
  digitalWrite(dir_pin_left, LOW);
  analogWrite(enable_pin_right, 175);
  analogWrite(enable_pin_left, 175);
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
  analogWrite(enable_pin_right, 250);
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
  
  if ( distanceleft <= 10 && distanceright <= 10 ) {
    drive_brake();
    spin_right();
    TMRArd_InitTimer(minturntimer, 1000);
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
    if ( abs(distanceleft - distanceright) <=2 ) {
      drive_forward();
      Serial.println("on to 3");
      state = drivingsidewall;
    }
  }
}

void drive_side_wall(void) {
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
  if ( distanceleft <= 10 && distanceright <= 10 ) {
    drive_brake();
    state = state4;
  }
}


