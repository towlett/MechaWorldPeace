/****************************************************
 * 
 *  Basic driving functions
 *
 ****************************************************/

#include "Arduino.h"
#include "defines.h"
#include "drive.h"

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