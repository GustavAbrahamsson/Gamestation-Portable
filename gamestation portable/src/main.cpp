#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ctime>
#include <cstdlib>

#include "Graphics/Graphics.h"
#include "Display.h"
#include "Classes.h"
#include "BallBouncing.h"

#include "example_code.h"


using namespace std;


vector<vector<uint8_t> > neighbours( SCREEN_WIDTH , vector<uint8_t> (SCREEN_HEIGHT, 0));

#define GROUND_HEIGHT 7

void drawEnvironment(){
   //drawLine(0, SCREEN_HEIGHT - 1 - GROUND_HEIGHT, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 - GROUND_HEIGHT);
   fillRectangle(0, SCREEN_HEIGHT - 1 - GROUND_HEIGHT, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 - GROUND_HEIGHT);
   display.display();
}

void initGameOfLife(uint8_t percent){
   //Serial.println("initGameOfLife");
   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         screen1[j][i] = 0;
         display.drawPixel(j, i, SSD1306_BLACK);
         screen2[j][i] = 0;
      }
   }

   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         if(rand() % 100 + 1 < percent){
            screen1[j][i] = 1;
            display.drawPixel(j, i, SSD1306_WHITE);
         } 
      }
   }
   display.display();
}

uint8_t giveNeighbours(uint8_t x, uint8_t y){
   //Serial.println("giveNeighbours");
   uint8_t neighbours = 0;
   bool leftEdge = 0;
   bool rightEdge = 0;
   bool topEdge = 0;
   bool bottomEdge = 0;

   if(x <= 0) leftEdge = 1;
   if(y <= 0) topEdge = 1;
   if(x >= SCREEN_WIDTH - 1) rightEdge = 1;
   if(y >= SCREEN_HEIGHT - 1) bottomEdge = 1;

   if (!bottomEdge) if(screen1[x][y+1]) neighbours++;
   if (!rightEdge) if(screen1[x+1][y]) neighbours++;
   if (!topEdge) if(screen1[x][y-1]) neighbours++;
   if (!leftEdge) if(screen1[x-1][y]) neighbours++;

   if (!rightEdge && !bottomEdge) if(screen1[x+1][y+1]) neighbours++;
   if (!leftEdge && !topEdge) if(screen1[x-1][y-1]) neighbours++;
   if (!rightEdge && !topEdge) if(screen1[x+1][y-1]) neighbours++;
   if (!leftEdge && !bottomEdge) if(screen1[x-1][y+1]) neighbours++;

   return neighbours;
}

void incrementNeighbours(uint8_t x, uint8_t y){
   bool leftEdge = 0;
   bool rightEdge = 0;
   bool topEdge = 0;
   bool bottomEdge = 0;

   if(x <= 0) leftEdge = 1;
   if(y <= 0) topEdge = 1;
   if(x >= SCREEN_WIDTH - 1) rightEdge = 1;
   if(y >= SCREEN_HEIGHT - 1) bottomEdge = 1;
   
   if (!bottomEdge) neighbours[x][y+1]++;
   if (!rightEdge) neighbours[x+1][y]++;
   if (!topEdge) neighbours[x][y-1]++;
   if (!leftEdge) neighbours[x-1][y]++;

   if (!rightEdge && !bottomEdge) neighbours[x+1][y+1]++;
   if (!leftEdge && !topEdge) neighbours[x-1][y+1]++;
   if (!rightEdge && !topEdge) neighbours[x+1][y+1]++;
   if (!leftEdge && !bottomEdge) neighbours[x-1][y+1]++;
}

void oldGameOfLife(){
   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         neighbours[j][i] = 0;
      }
   }
   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         incrementNeighbours(j, i);
      }
   }
   
   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         //uint8_t neighbours = giveNeighbours(j, i, screen1);
         
         uint8_t n = neighbours[j][i];

         bool pixel = screen1[j][i];

         
         if(pixel && (n < 2 || n > 3)){
            screen2[j][i] = 0;
            display.drawPixel(j, i, SSD1306_BLACK);
         }
         else if (!pixel && n == 3){
            screen2[j][i] = 1;
            display.drawPixel(j, i, SSD1306_WHITE);
         }

         /*
         if(!display.getPixel(j, i) && neighbours == 3) display.drawPixel(j, i, SSD1306_WHITE);
         else if(display.getPixel(j, i)){
            display.drawPixel(j, i, SSD1306_BLACK);
         }*/
      }
   }
   screen1.swap(screen2);
   
  
   
   /*
   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         if(newScreen[j][i]) display.drawPixel(j, i, SSD1306_WHITE);
         else display.drawPixel(j, i, SSD1306_BLACK);
      }
   }
   */
   display.display();
}

void gameOfLife(){
   //Serial.println("gameOfLife");
   
   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         //uint8_t neighbours = giveNeighbours(j, i, screen1);
         
         uint8_t n = giveNeighbours(j, i);

         bool pixel = screen1[j][i];
         
         if(pixel){ 
            if(n < 2 || n > 3){
               screen2[j][i] = 0;
               setPixel(j, i, 0);
            }
            else{
               screen2[j][i] = 1;
               setPixel(j, i);
            }
         }
         else if (!pixel){
            if(n == 3){
               screen2[j][i] = 1;
               setPixel(j, i);
            }
            else{
               screen2[j][i] = 0;
               setPixel(j, i, 0);
            }
         }
      }
   }
   screen1.swap(screen2);
   
   display.display();
}

void setupGraphics(){

}

void randomSpawns(){
   for(int i = 0; i < 6; i++){
      for(int j = 0; j < 12; j++){
         spawn(10*j, 10*i, "GliderDL", 0);
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

   setupGraphics();

   initGraphics();

   initGameOfLife(0);

   randomSpawns();
   display.display();
   delay(1000);
}

unsigned long currentTime = 0;
unsigned long lastTime = 0;

void loop(){
   
   currentTime = millis();
   if(currentTime - lastTime > 25){
      lastTime = currentTime;
      gameOfLife();
   }
}  
