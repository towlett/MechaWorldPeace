/****************************************************
 * 
 *  State Machine Functions
 *
 ****************************************************/

#include "defines.h"
#include "shooter.h"
#include "state_machine.h"
#include "status_LEDs.h"
#include "Timers.h"

static unsigned char current_state = drivingbackwall;

void set_state(unsigned char next_state) {
  switch (next_state) {
      case(drivingbackwall):
        current_state = drivingbackwall;
        break;
      case(findingsidewall):
        current_state = findingsidewall;
        break;
      case(drivingsidewall):
        current_state = drivingsidewall;
        break;
      case(findIR):
        current_state = findIR;
        break;
      case(shooting):
        current_state = shooting;
        break;
      case(game_over):
        current_state = game_over;
        break;
      case(resetting):
        current_state = resetting;
        break;
  }
}

unsigned char get_state(void) {
	return current_state;
}

void game_timer(void) {
 if (TMRArd_IsTimerExpired(GAME_TIMER)  == TMRArd_EXPIRED) {
   set_state(game_over);
   reset_shooter();
   OffLED();
 }
}