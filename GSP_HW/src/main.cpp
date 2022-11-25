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

bool btn1 = 0;
bool btn2 = 0;
bool btn3 = 0;
bool btn4 = 0;

bool btn5 = 0;
bool btn6 = 0;
bool btn7 = 0;
bool btn8 = 0;

bool btnSelect = 0;
bool btnStart = 0;

uint8_t bzr1_btn = BTN1;
uint8_t last_bzr1_btn = BTN1;


uint8_t bzr2_btn = BTN2;
uint8_t last_bzr2_btn = BTN2;

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
   pinMode(BTN1, INPUT_PULLDOWN);
   pinMode(BTN2, INPUT_PULLDOWN);
   pinMode(BTN3, INPUT_PULLDOWN);
   pinMode(BTN4, INPUT_PULLDOWN);
   
   pinMode(BTN5, INPUT_PULLDOWN);
   pinMode(BTN6, INPUT_PULLDOWN);
   pinMode(BTN7, INPUT_PULLDOWN);
   pinMode(BTN8, INPUT_PULLDOWN);
   
   pinMode(SELECT, INPUT_PULLDOWN);
   pinMode(START, INPUT_PULLDOWN);
}

void readButtons(){
   btn1 = digitalRead(BTN1);
   btn2 = digitalRead(BTN2);
   btn3 = digitalRead(BTN3);
   btn4 = digitalRead(BTN4);

   btn5 = digitalRead(BTN5);
   btn6 = digitalRead(BTN6);
   btn7 = digitalRead(BTN7);
   btn8 = digitalRead(BTN8);
   
   btnSelect = digitalRead(SELECT);
   btnStart = digitalRead(START);
}

void setup() {

   Serial.begin(115200);
   Serial.println("Serial begun");
   //delay(1000);
   Serial.println("Beep");

   setupInputs();

   ledcAttachPin(BUZZER_PIN, BZR_CHANNEL);
   ledcAttachPin(BUZZER_PIN_2, BZR_2_CHANNEL);
   tone1(440);
   tone2(1000);

   Serial.println("BUZZ1: " + analogGetChannel(BUZZER_PIN));
   Serial.println("BUZZ2: " + analogGetChannel(BUZZER_PIN_2));

   //tone(BUZZER_PIN, 440, 1000, 0);
   //tone(BUZZER_PIN_2, 800, 1000, 0);
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

   //xTaskCreate(soundLoop, "sound", 10000, NULL, 1, &soundTask);
   delay(3000);
}

unsigned long currentTime = 0;
unsigned long lastTime = 0;
unsigned long lastTime2 = 0;

uint16_t mainTimerPeriod = 100;
uint16_t mainLastTime = 0;

void loop(){
   
   currentTime = millis();
   readButtons();

   if (currentTime - mainLastTime > mainTimerPeriod){
      mainLastTime = currentTime;
      /*
      Serial.println(btn1);
      Serial.print(btn2);
      Serial.print(btn3);
      Serial.print(btn4);
      Serial.print(btn5);
      Serial.print(btn6);
      Serial.print(btn7);
      Serial.print(btn8);*/

      /*
      if       (btn1 && bzr1_btn != 1) bzr1_btn = 1;
      else if  (btn2 && bzr1_btn != 2) bzr1_btn = 2;
      else if  (btn3 && bzr1_btn != 3) bzr1_btn = 3;
      else if  (btn4 && bzr1_btn != 4) bzr1_btn = 4;
      
      if       (btn5 && bzr2_btn != 5) bzr2_btn = 5;
      else if  (btn6 && bzr2_btn != 6) bzr2_btn = 6;
      else if  (btn7 && bzr2_btn != 7) bzr2_btn = 7;
      else if  (btn8 && bzr2_btn != 8) bzr2_btn = 8;*/
      
      if       (btn1) bzr1_btn = 1;
      else if  (btn2) bzr1_btn = 2;
      else if  (btn3) bzr1_btn = 3;
      else if  (btn4) bzr1_btn = 4;
      else bzr1_btn = 0;
      
      if       (btn5) bzr2_btn = 5;
      else if  (btn6) bzr2_btn = 6;
      else if  (btn7) bzr2_btn = 7;
      else if  (btn8) bzr2_btn = 8;
      else bzr2_btn = 0;


      if (bzr1_btn != last_bzr1_btn){
         last_bzr1_btn = bzr1_btn;
         switch (bzr1_btn){
            case 1:
               tone1(82);
               break;
            case 2:
               tone1(110);
               break;
            case 3:
               tone1(147);
               break;
            case 4:
               tone1(196);
               break;
            default:
               tone1(0);
               break;
         }
      }


      if (bzr2_btn != last_bzr2_btn){
         last_bzr2_btn = bzr2_btn;
         switch (bzr2_btn){
            case 5:
               tone2(247);
               break;
            case 6:
               tone2(330);
               break;
            case 7:
               tone2(800);
               break;
            case 8:
               tone2(1500);
               break;
            default:
               tone2(0);
               break;
         }
      }
   
      /*
      if       (btn1) tone1(82);
      else if  (btn2) tone1(110);
      else if  (btn3) tone1(147);
      else if  (btn4) tone1(196);
      else tone1(0);
      
      if       (btn5) tone2(247);
      else if  (btn6) tone2(330);
      else if  (btn7) tone2(400);
      else if  (btn8) tone2(500);
      else tone2(0);*/
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
   
   if(currentTime - lastTime > 1000){
      lastTime = currentTime;

   }if(currentTime - lastTime2 > 10000){
      lastTime2 = currentTime;
      clear_display();
      //initGameOfLife(0);
      //randomSpawns();
   }
}

