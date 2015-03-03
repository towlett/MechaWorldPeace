/****************************************************
 * 
 *	Global Definitions
 *
 ****************************************************/

//STATES
#define drivingbackwall     1
#define findingsidewall     2
#define drivingsidewall     3
#define findIR              4
#define shooting            5
#define game_over           6
#define resetting           7

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
#define MAX_POS             160
#define SHOOTER_STEP        1
#define SHOOTER_DELAY       10 
#define SHOOTER_TIMER       0
#define SHOT_CLOCK_TIMER    1
#define b_press_pin         3


//LEDs
#define redled1            A0        
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
#define RESET_TURN_TIMER    10
#define TURN_TIMER          11

#define ADDITIONAL_TURN_TIME 250