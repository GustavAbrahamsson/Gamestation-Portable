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

typedef std::vector<std::vector<bool>> BitMatrix;

#include "Definitions.h"
#include "Graphics/Graphics.cpp"
#include "Display.h"
#include "Classes.h"
#include "BallBouncing.h"
#include "example_code.h"
#include "ButtonHandler.h"

#include "Game_of_Life.cpp"
#include "Snake.h"
#include "Snake.cpp"

#define GROUND_HEIGHT 7

#include <Tone32.h>

uint32_t lastTime = 0;
uint32_t lastTime2 = 0;

uint32_t currentTime = 0;
uint32_t mainTimerPeriod = 20;
uint32_t mainLastTime = 0;

//Snake* snake = NULL;

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
    delay(3000);

    setupSnake();

    display.display();

}


void loop(){
   
    currentTime = millis();

    if (currentTime - mainLastTime > mainTimerPeriod){ // Main execution frequency
        mainLastTime = currentTime;

        readButtons();

        filterButtons();

        playBzrWithButtons();

    }

    snakeGame(currentTime);

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


