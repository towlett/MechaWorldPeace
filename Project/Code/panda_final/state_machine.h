/****************************************************
 * 
 *  State Machine Functions
 *
 ****************************************************/

#ifndef STATE_MACHINE
#define STATE_MACHINE

void set_state(unsigned char next_state);
unsigned char get_state(void);
void game_timer(void);

#endif