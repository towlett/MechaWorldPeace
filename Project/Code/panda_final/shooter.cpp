/****************************************************
 * 
 *  Shooter functions
 *
 ****************************************************/

#include <Servo.h>

#include "defines.h"
#include "drive.h"
#include "shooter.h"
#include "state_machine.h"
#include "status_LEDs.h"
#include "Timers.h"

Servo shooter;
Servo presser;

void init_shooter(void) {
  shooter.attach(servopin);
  presser.attach(b_press_pin);
  reset_shooter();
}

void reset_shooter(void) {
  shooter.write(0);
  presser.write(180);
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

void shoot(void) {
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
  if (TMRArd_IsTimerExpired(RESET_TURN_TIMER))
  {
    set_state(resetting);
    shooter.write(0);
    presser.write(180);
    spin_left();
    TMRArd_InitTimer(TURN_TIMER, 1000);
  }
}

void reset_turn(void) {
  game_timer();
  if (TMRArd_IsTimerExpired(TURN_TIMER)) {
    drive_stop();
    spin_right();
    TMRArd_InitTimer(MAX_SEARCH_TIMER, 4000);
    TMRArd_InitTimer(RESET_TURN_TIMER, 45000);
    set_state(findIR);
  }
}