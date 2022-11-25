#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

using namespace std;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

vector<vector<uint8_t> > neighbours( SCREEN_WIDTH , vector<uint8_t> (SCREEN_HEIGHT, 0));

// x_min, x_max, y_min, y_max. Defaults to entire screen
//uint8_t renderRange[] = {0, SCREEN_WIDTH - 1, 0, SCREEN_HEIGHT - 1};
const int16_t defaultRenderRange[] = {0, SCREEN_WIDTH - 1, 0, SCREEN_HEIGHT - 1};
int16_t renderRange[] = {0, SCREEN_WIDTH - 1, 0, SCREEN_HEIGHT - 1};


void initGameOfLife(uint8_t percent){
   //Serial.println("initGameOfLife");
   clear_display();
   
   for(int i = 0; i < 4; i++) renderRange[i] = defaultRenderRange[i];

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

const uint8_t renderOffset = 2;
const uint8_t renderSensitivity = 2;

void updateRendering(uint8_t x, uint8_t y){
   if(x < renderRange[0] + renderSensitivity && renderRange[0] > 0) renderRange[0] = x - renderOffset;
   if(x > renderRange[1] - renderSensitivity && renderRange[1] < SCREEN_WIDTH - 1) renderRange[1] = x + renderOffset;
   if(y < renderRange[2] + renderSensitivity && renderRange[2] > 0) renderRange[2] = y - renderOffset;
   if(y > renderRange[3] - renderSensitivity && renderRange[3] < SCREEN_HEIGHT - 1) renderRange[3] = y + renderOffset;
}

void gameOfLife(){
   //Serial.println("gameOfLife");
   
   for(uint8_t i = renderRange[2]; i <= renderRange[3]; i++){
      for(uint8_t j = renderRange[0]; j <= renderRange[1]; j++){
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
         //updateRendering(j, i);
      }
   }
   screen1.swap(screen2);
   
   display.display();
}