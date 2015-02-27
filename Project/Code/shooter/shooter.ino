/* SHOOT 

*/

/*---------------- Includes ---------------------------------*/
#include <Servo.h>
#include <Timers.h>

/*---------------- Module Defines ---------------------------*/
#define MAX_POS 160
#define SHOOTER_STEP 1
#define SHOOTER_DELAY 10 
#define SHOOTER_TIMER 0
#define SHOT_CLOCK_TIMER 1
#define servopin      10

/*----------------Module Objects ----------------------------*/
Servo shooter;

/*---------------- Module Function Prototypes ---------------*/
unsigned char ShooterTimerExp(void);
void ChangeShooterPos(void);
unsigned char ShotClockExp(void);
void RespShotClock(void);

void setup()
{
  shooter.attach(servopin);
  shooter.write(0);
  TMRArd_InitTimer(SHOT_CLOCK_TIMER, 3000);
}

void loop()
{
  if (ShotClockExp()) {
    RespShotClock();
  }
  if (ShooterTimerExp()) {
    ChangeShooterPos();
  }
}
  
/*------------- Module Functions -------------------------*/

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
    if (pos >= MAX_POS) {
      dir = 1;
    }
  }
  if (dir == 1) {
    pos -= SHOOTER_STEP;
    shooter.write(pos);
    if (pos <= 0) {
      dir = 0;
    }
  }
  if (pos > 0) {
    TMRArd_InitTimer(SHOOTER_TIMER, SHOOTER_DELAY);
  } 
}
    


