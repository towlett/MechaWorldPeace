/****************************************************
 * 
 *  Driving Algorithms - Robot navigation
 *
 ****************************************************/

#include <NewPing.h>

#include "Arduino.h"
#include "defines.h"
#include "drive.h"
#include "driving_algos.h"
#include "state_machine.h"
#include "status_LEDs.h"
#include "Timers.h"

//ULTRASOUND
static unsigned char last_ping;

static NewPing left_ping(trigPinleft, echoPinleft, 150);
static NewPing right_ping(trigPinright, echoPinright, 150);

//IR
static unsigned long period_total;
static unsigned long pulse_freq;
static unsigned int period_high;
static unsigned int period_low;

//ULTRASONIC
unsigned int ultrasonicPing_right(void) {
  unsigned int distanceright = right_ping.ping()/US_ROUNDTRIP_CM;
  if (distanceright == 0) {
    distanceright = 150;
  }
  return distanceright;
}

unsigned int ultrasonicPing_left(void) {
  unsigned int distanceleft = left_ping.ping()/US_ROUNDTRIP_CM;
  if (distanceleft == 0) {
    distanceleft = 150;
  }
  return distanceleft;
}

//MOTORS  

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
    set_state(findingsidewall);
  }
}

void find_side_wall(void) {
  static unsigned int distanceleft;
  static unsigned int distanceright;
  
  if (TMRArd_IsTimerExpired(minturntimer)) {
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
      set_state(drivingsidewall);
    }
  }
  
  if (TMRArd_IsTimerExpired(maxturntimer)) {
    spin_left();
    if ((last_ping == last_ping_right) && TMRArd_IsTimerExpired(pingtimer)) {
      distanceleft = ultrasonicPing_left();
      TMRArd_InitTimer(pingtimer, 50);
      set_state(last_ping_left);
    }
    if ((last_ping == last_ping_left) && TMRArd_IsTimerExpired(pingtimer)) {
      distanceright = ultrasonicPing_right();
      TMRArd_InitTimer(pingtimer, 50);
      last_ping = last_ping_right;
    }
    if ( abs(distanceleft - distanceright) <=1 ) {
      drive_forward();
      set_state(drivingsidewall);
    }
  }
}

void drive_side_wall(void) {
  static unsigned int distanceleft = 150;
  static unsigned int distanceright = 150;
  
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
    drive_brake_backup();
    spin_right();
    TMRArd_InitTimer(MAX_SEARCH_TIMER, 4000);
    set_state(findIR);
  }
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
  if (count>2) { //will return true once it has registered 3 consecutive highs
    delay(ADDITIONAL_TURN_TIME);
    drive_stop();
    TMRArd_InitTimer(SHOT_CLOCK_TIMER, 10);
    OffLED();
    TMRArd_InitTimer(RESET_TURN_TIMER, 30000);
    set_state(shooting);       
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