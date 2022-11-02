#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ctime>
#include <cstdlib>

using namespace std;

typedef vector<vector<bool>> BitMatrix;

#include "Definitions.h"
#include "Graphics/Graphics.cpp"
#include "Display.cpp"
#include "Classes.h"
#include "BallBouncing.h"
#include "example_code.h"

#include "Game_of_Life.cpp"


#define GROUND_HEIGHT 7

#include <Tone32.h>

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

void soundLoop(void * parameter){
   for(;;){
      
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

      Serial.println(counter);

      if (counter > 30){
         delay(2000);
         counter = 0;
      }

      
   }
}

void setup() {

   Serial.begin(115200);
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

   xTaskCreatePinnedToCore(soundLoop, "sound", 1000, NULL, 1, &soundTask, 0);
   delay(5000);
}

unsigned long currentTime = 0;
unsigned long lastTime = 0;
unsigned long lastTime2 = 0;

void loop(){
   
   currentTime = millis();

   gameOfLife();
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
      initGameOfLife(0);
      randomSpawns();
   }
}

