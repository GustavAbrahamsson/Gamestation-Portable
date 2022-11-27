#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ctime>
#include <cstdlib>
#include <vector>

//using namespace std;

typedef std::vector<std::vector<bool>> BitMatrix;

#include "Definitions.h"
#include "Graphics/Graphics.cpp"
#include "Display.cpp"
#include "Classes.h"
#include "BallBouncing.h"
#include "example_code.h"

#include "Game_of_Life.cpp"

#define GROUND_HEIGHT 7

#include <Tone32.h>

bool btn0 = 0;
bool btn1 = 0;
bool btn2 = 0;
bool btn3 = 0;

bool btn4 = 0;
bool btn5 = 0;
bool btn6 = 0;
bool btn7 = 0;

bool btn[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Buttons pressed
bool old_btn[] = {0, 0, 0, 0, 0, 0, 0, 0}; // Buttons pressed

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
      btn[i] = digitalRead(i+1); // Assign new values
   }
   
   btnSelect = digitalRead(SELECT);
   btnStart = digitalRead(START);
}

void setup() {

   Serial.begin(115200);
   Serial.println("Serial begun");
   Serial.println("Beep");

   setupInputs();
   setupOutputs();

   tone1(440);
   tone2(1000);

   tone1(0);
   tone2(0);
   Serial.println("Beep ended");
   delay(250);

   srand(time(0));

   if(!display.begin(SSD1306_SWITCHCAPVCC)) {
      Serial.println(F("SSD1306 allocation failed"));
      while(1);
   }

   display.clearDisplay();
   initGraphics();
   initGameOfLife(0);
   randomSpawns();
   display.display();
   delay(2000);
}

uint32_t lastTime = 0;
uint32_t lastTime2 = 0;

uint32_t currentTime = 0;
uint32_t mainTimerPeriod = 10;
uint32_t mainLastTime = 0;

void loop(){
   
   currentTime = millis();

   if (currentTime - mainLastTime > mainTimerPeriod){ // Main execution frequency
      mainLastTime = currentTime;

      readButtons();

      for (int i = 0; i < 8; i++){ // Go through all button values

         if (btn[i] != old_btn[i]){ // If one value has changed
            old_btn[i] = btn[i];
            if(btn[i]){ // If it went from 0 to 1
               if (i < 4){ // If it's the left D-pad (0-3)
                  bzr1_btn = i;
                  break;
               }else{ // Right D-pad
                  bzr2_btn = i;
                  break;
               }
            }
            
         }
      }
      String out_string = "";
      for(int i = 0; i < 8; i++){
         out_string += (String)btn[i];
      }
      Serial.println(out_string);
      Serial.println();
      Serial.println((String)bzr1_btn + "    " + (String)bzr2_btn);
      Serial.println();

      if (bzr1_btn != last_bzr1_btn){
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
            default:
               tone1(0);
               break;
         }
      }

      
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
         default:
            tone2(0);
            break;
         
      }
   }
   



   /*
   tone1(82);
   delay(3000);
   tone1(110);
   delay(3000);
   tone1(147);
   delay(3000);
   tone1(196);
   delay(3000);
   tone1(247);
   delay(3000);
   tone1(330);
   delay(3000);*/


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

