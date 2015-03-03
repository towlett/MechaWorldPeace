/****************************************************
 * 
 *  Code that uses the front mounted LEDs to display info
 *
 ****************************************************/

#include "Arduino.h"
#include "defines.h"
#include "status_LEDs.h"
#include "Timers.h"

void ScanningLED(void) {
    unsigned int blinktime = 300;
    
    if(digitalRead(blueled) == LOW && digitalRead(whiteled) == LOW){
      TMRArd_InitTimer(BLUE_LED_TIMER, blinktime);
      digitalWrite(blueled, HIGH); 
    }
    
    if(TMRArd_IsTimerExpired(BLUE_LED_TIMER)  == TMRArd_EXPIRED){
       TMRArd_InitTimer(WHITE_LED_TIMER, blinktime);
       TMRArd_ClearTimerExpired(BLUE_LED_TIMER);
       digitalWrite(whiteled, HIGH);
       digitalWrite(blueled, LOW);
    }
    
     if(TMRArd_IsTimerExpired(WHITE_LED_TIMER)  == TMRArd_EXPIRED){
       TMRArd_InitTimer(BLUE_LED_TIMER, blinktime);
       TMRArd_ClearTimerExpired(WHITE_LED_TIMER);
       digitalWrite(blueled, HIGH);
       digitalWrite(whiteled, LOW);
    }
}

unsigned char LockedOnLED(void) {
    static unsigned char ledcyclecount = 0; // number of flashes for LockedOnLED function

    unsigned int blinktime = 175;
    
    if(digitalRead(redled1) == LOW && digitalRead(redled2) == LOW && digitalRead(redled3) == LOW && ledcyclecount == 0) {
      TMRArd_InitTimer(RED_LED_TIMER, blinktime);
      digitalWrite(redled1, HIGH); 
      digitalWrite(redled2, HIGH); 
      digitalWrite(redled3, HIGH); 
      ledcyclecount += 1;
    }
    
    if(TMRArd_IsTimerExpired(RED_LED_TIMER)  == TMRArd_EXPIRED && digitalRead(redled1) == HIGH){
       TMRArd_ClearTimerExpired(RED_LED_TIMER);
       digitalWrite(redled1, LOW); 
       digitalWrite(redled2, LOW); 
       digitalWrite(redled3, LOW); 
       TMRArd_InitTimer(RED_LED_TIMER, blinktime);
       ledcyclecount += 1;
    }
    
    if(TMRArd_IsTimerExpired(RED_LED_TIMER)  == TMRArd_EXPIRED && digitalRead(redled1) == LOW){
       TMRArd_ClearTimerExpired(RED_LED_TIMER);
       digitalWrite(redled1, HIGH); 
       digitalWrite(redled2, HIGH); 
       digitalWrite(redled3, HIGH); 
       TMRArd_InitTimer(RED_LED_TIMER, blinktime);
       ledcyclecount += 1;
    }
    
    return ledcyclecount;
}

void OffLED(void){
    digitalWrite(redled1, LOW);
    digitalWrite(blueled, LOW);
    digitalWrite(redled2, LOW);
    digitalWrite(blueled, LOW);
    digitalWrite(redled3, LOW);
}