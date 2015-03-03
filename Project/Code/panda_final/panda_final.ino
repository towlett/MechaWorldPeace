/**************************************************************
  File:     panda_final.ino

  Contents: This program runs the robot for ME210 2015 basketball
            competition

  Team:     Tom Owlett
            Jared Ostdiek
            Tim Martin
            Kesia Alfred       

**************************************************************/

/*---------------- Includes ---------------------------------*/
#include "defines.h"
#include "drive.h"
#include "driving_algos.h"
#include "pin_init.h"
#include "shooter.h"
#include "state_machine.h"
#include "status_LEDs.h"

void setup() {  
  initialize_pins();
}

void loop() {
  
unsigned char state = get_state();
  
  switch(state) {
    case(drivingbackwall):
      //game_timer();
      drive_back_wall();
      LockedOnLED();
      break;
      
    case(findingsidewall):
      //game_timer();
      find_side_wall();
      break;
      
    case(drivingsidewall):
      //game_timer();
      drive_side_wall();
      break;
      
    case(findIR):
      //game_timer();
      ScanningLED();
      FindIR();
      break;
      
    case(shooting):
      shoot();
      break;
     
    case(resetting):
      reset_turn();        
      break;
     
    case(game_over):
     drive_stop();
     break;   
  }
}