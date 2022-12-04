#include <Arduino.h>
#include "Definitions.h"

bool raw_btn[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Buttons pressed
bool btn[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Buttons pressed, filtered
bool old_btn[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Old buttons pressed

uint32_t btnTime[] = {0, 0, 0, 0, 0, 0, 0, 0};
uint32_t btnTransientTime = 50;

uint8_t buttonsPressed = 0;

bool btnSelect = 0;
bool btnStart = 0;

int8_t dPad1_btn = -1;
int8_t last_dPad1_btn = -1;

int8_t dPad2_btn = -1;
int8_t last_dPad2_btn = -1;

extern uint32_t currentTime;

void tone1(uint16_t freq){
   ledcWriteTone(BZR_CHANNEL, freq);
}

void tone2(uint16_t freq){
   ledcWriteTone(BZR_2_CHANNEL, freq);
}

void readButtons(){
   for (int i = 0; i < 8; i++){ // Go through all button values
      raw_btn[i] = digitalRead(i+1); // Assign new values
   }
   
   btnSelect = digitalRead(SELECT);
   btnStart = digitalRead(START);
}

void filterButtons(){
   for (int i = 0; i < 8; i++){ // Go through all button values

         if (raw_btn[i] && !old_btn[i] && !btn[i]){ // Went from 0 to 1
            btnTime[i] = currentTime; // Note down when it was pressed
            buttonsPressed++;
            btn[i] = 1; // Assign the value to the true array
            //Serial.println("btn pressed: " + (String)i);

            if (i < 4){ // If it's the left D-pad (0-3)
               dPad1_btn = i;
               //Serial.println("bzr1 = " + (String)i);
            }else{ // Right D-pad
               dPad2_btn = i;
               //Serial.println("bzr2 = " + (String)i);
            }
         }

         if(currentTime - btnTime[i] > btnTransientTime && btn[i] && !raw_btn[i]){
            // If the button has been pressed, 'btnTransientTime' ms has surpassed and
            // the raw value is 0. ASSUMPTION: the button transient is over.
            btnTime[i] = currentTime;
            buttonsPressed--;
            btn[i] = 0; // Assign the value to the true array
            //Serial.println("btn RESET: " + (String)i);
            
         }
         if(buttonsPressed == 0){ // If no buttons are being pressed
            dPad1_btn = -1;
            dPad2_btn = -1;
         }

      }
}

std::pair<int8_t,int8_t> dPad_direction(int8_t dPadButton){
    if (dPadButton == -1) return {0,0}; // Return the zero-vector if no button is pressed
    else if (dPadButton < 4){ // If it's the left D-pad (0-3)
        switch(dPadButton){
            case 0:
                return {0, -1};
            case 1:
                return {1, 0};
            case 2:
                return {-1, 0};
            case 3:
                return {0, 1};
            default:
                return {0, 0};
        }
    }else{ // Right D-pad
        
    }
}

void playBzrWithButtons(){
   if(dPad1_btn != last_dPad1_btn){
      last_dPad1_btn = dPad1_btn;
      switch (dPad1_btn){
         case 0:
            tone1(82);
            break;
         case 1:
            tone1(110);
            break;
         case 2:
            tone1(147);
            break;
         case 3:
            tone1(196);
            break;
         case -1:
            tone1(0);
            break;
      }
   }
   
     if(dPad2_btn != last_dPad2_btn){
      last_dPad2_btn = dPad2_btn;
      switch (dPad2_btn){
         case 4:
            tone2(247);
            break;
         case 5:
            tone2(330);
            break;
         case 6:
            tone2(800);
            break;
         case 7:
            tone2(1500);
            break;
         case -1:
            tone2(0);
            break;
      }
   } 
}
