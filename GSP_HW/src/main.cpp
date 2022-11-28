#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>

std::pair<uint8_t,uint8_t> pos;

//using namespace std;

typedef std::vector<std::vector<bool>> BitMatrix;

#include "Definitions.h"
#include "Graphics/Graphics.cpp"
#include "Display.cpp"
#include "Classes.h"
#include "BallBouncing.h"
#include "example_code.h"

#include "Game_of_Life.cpp"
#include "Snake.cpp"

#define GROUND_HEIGHT 7

#include <Tone32.h>

bool raw_btn[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Buttons pressed
bool btn[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Buttons pressed, filtered
bool old_btn[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Old buttons pressed

uint32_t lastTime = 0;
uint32_t lastTime2 = 0;

uint32_t currentTime = 0;
uint32_t mainTimerPeriod = 20;
uint32_t mainLastTime = 0;

uint32_t btnTime[] = {0, 0, 0, 0, 0, 0, 0, 0};
uint32_t btnTransientTime = 50;
bool btnPressed[] = {0, 0, 0, 0, 0, 0, 0, 0};

uint8_t buttonsPressed = 0;

bool btnSelect = 0;
bool btnStart = 0;

int8_t bzr1_btn = -1;
int8_t last_bzr1_btn = -1;

int8_t bzr2_btn = -1;
int8_t last_bzr2_btn = -1;

TaskHandle_t soundTask;

void drawEnvironment(){
   //drawLine(0, SCREEN_HEIGHT - 1 - GROUND_HEIGHT, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 - GROUND_HEIGHT);
   fillRectangle(0, SCREEN_HEIGHT - 1 - GROUND_HEIGHT, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 - GROUND_HEIGHT);
   display.display();
}

void randomSpawns(){
   /*
   for(int i = 0; i < 2; i++){
      for(int j = 0; j < 4; j++){
         spawnAsset(20 + 30*j, 15 + 30 * i, "beehiveExplosion");
      }
   }*/
   spawnObject(70, 20, "GliderUR");
   spawnObject(70, 40, "GliderUR", 90);
   spawnObject(60, 40, "GliderUR", 180);
   spawnObject(60, 20, "GliderUR", -90);
   /*
   for(int i = 0; i < 6; i++){
      for(int j = 0; j < 12; j++){
         spawnObject(10*j, 10*i, "GliderDL", 0);
      }
   }*/
}


uint16_t rampTone = 2000;
uint16_t rampTone2 = 5;
uint16_t rampTone3 = 10000;

uint16_t counter = 0;

void soundIteration(){
      rampTone -= 100;
      tone(BUZZER_PIN, rampTone, 5, 0);
      if (rampTone < 5){
         rampTone = 2000;
         counter++;
      }

      rampTone2 += 100;
      //tone(BUZZER_PIN, rampTone2, 5, 0);
      if (rampTone2 > 5000) rampTone2 = 5;

      rampTone3 -= 100;
      //tone(BUZZER_PIN, rampTone3, 5, 0);
      if (rampTone3 < 5000) rampTone3 = 10000;

      //Serial.println(counter);

      if (counter > 30){
         delay(2000);
         counter = 0;
      }

}

void soundLoop(void * parameter){
   // TaskCreate works really good on e.g. lolin32 since it has seveal cores, but
   // the ESP32-S2 only has one core. Having two tasks on the same core really slowed
   // things down.
   for(;;){
      soundIteration();
   }
}

void tone1(uint16_t freq){
   ledcWriteTone(BZR_CHANNEL, freq);
}

void tone2(uint16_t freq){
   ledcWriteTone(BZR_2_CHANNEL, freq);
}

void setupInputs(){
   pinMode(BTN0, INPUT_PULLDOWN);
   pinMode(BTN1, INPUT_PULLDOWN);
   pinMode(BTN2, INPUT_PULLDOWN);
   pinMode(BTN3, INPUT_PULLDOWN);

   pinMode(BTN4, INPUT_PULLDOWN);
   pinMode(BTN5, INPUT_PULLDOWN);
   pinMode(BTN6, INPUT_PULLDOWN);
   pinMode(BTN7, INPUT_PULLDOWN);
   
   pinMode(SELECT, INPUT_PULLDOWN);
   pinMode(START, INPUT_PULLDOWN);
}

void setupOutputs(){
   ledcAttachPin(BUZZER_PIN, BZR_CHANNEL);
   ledcAttachPin(BUZZER_PIN_2, BZR_2_CHANNEL);
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

         if (raw_btn[i] && !old_btn[i] && !btnPressed[i]){ // Went from 0 to 1
            btnTime[i] = currentTime; // Note down when it was pressed
            btnPressed[i] = 1;
            buttonsPressed++;
            btn[i] = 1; // Assign the value to the true array
            Serial.println("btn pressed: " + (String)i);

            if (i < 4){ // If it's the left D-pad (0-3)
               bzr1_btn = i;
               Serial.println("bzr1 = " + (String)i);
            }else{ // Right D-pad
               bzr2_btn = i;
               Serial.println("bzr2 = " + (String)i);
            }
         }

         if(currentTime - btnTime[i] > btnTransientTime && btnPressed[i] && !raw_btn[i]){
            // If the button has been pressed, 'btnTransientTime' ms has surpassed and
            // the raw value is 0. ASSUMPTION: the button transient is over.
            btnTime[i] = currentTime;
            btnPressed[i] = 0;
            buttonsPressed--;
            btn[i] = 0; // Assign the value to the true array
            Serial.println("btn RESET: " + (String)i);
            
         }
         if(buttonsPressed == 0){ // If no buttons are being pressed
            bzr1_btn = -1;
            bzr2_btn = -1;
         }

      }
}

void playBzrWithButtons(){
   if(bzr1_btn != last_bzr1_btn){
      last_bzr1_btn = bzr1_btn;
      switch (bzr1_btn){
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
   
     if(bzr2_btn != last_bzr2_btn){
      last_bzr2_btn = bzr2_btn;
      switch (bzr2_btn){
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


void setup() {

   Serial.begin(115200);
   Serial.println("Serial begun");
   setupInputs();
   setupOutputs();

   // Beep beep
   tone1(1750);
   delay(100);
   tone1(0);
   delay(25);
   tone1(1750);
   delay(100);
   tone1(0);

   delay(2000);

   srand(time(0));

   if(!display.begin(SSD1306_SWITCHCAPVCC)) {
      Serial.println(F("SSD1306 allocation failed"));
      while(1);
   }

   display.clearDisplay();
   //initGraphics();
   //initGameOfLife(0);
   //randomSpawns();
   display.display();

   setupSnake();
}


void loop(){
   
   currentTime = millis();

   if (currentTime - mainLastTime > mainTimerPeriod){ // Main execution frequency
      mainLastTime = currentTime;

      readButtons();

      filterButtons();

      playBzrWithButtons();
      
      snakeIteration();

   }



   //gameOfLife();
   /*
   drawLine(renderRange[0], renderRange[2], renderRange[1], renderRange[2]);
   drawLine(renderRange[1], renderRange[2], renderRange[1], renderRange[3]);
   drawLine(renderRange[1], renderRange[3], renderRange[0], renderRange[3]);
   drawLine(renderRange[0], renderRange[3], renderRange[0], renderRange[2]);
   */
   
   /*
   if(currentTime - lastTime > 1000){
      lastTime = currentTime;

   }if(currentTime - lastTime2 > 10000){
      lastTime2 = currentTime;
      clear_display();
      //initGameOfLife(0);
      //randomSpawns();
   }*/
}

