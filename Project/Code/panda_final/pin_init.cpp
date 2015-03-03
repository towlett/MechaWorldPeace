/****************************************************
 * 
 *  Initialize the pins for the program
 *
 ****************************************************/

#include "Arduino.h"
#include "defines.h"
#include "drive.h"
#include "shooter.h"
#include "Timers.h"

void initialize_pins(void) {
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
  
  TMRArd_InitTimer(GAME_TIMER, 125000);
  init_shooter();
}