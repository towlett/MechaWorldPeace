/****************************************************
 * 
 *  Shooter Functions
 *
 ****************************************************/

#ifndef SHOOTING_HEADER
#define SHOOTING_HEADER

void init_shooter(void);
void reset_shooter(void);
void shoot(void);
void reset_turn(void);
unsigned char ShooterTimerExp(void);
void ChangeShooterPos(void);
unsigned char ShotClockExp(void);
void RespShotClock(void);

#endif