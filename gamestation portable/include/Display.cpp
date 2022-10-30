#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

using namespace std;

typedef vector<vector<bool>> BitMatrix;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// ESP32-S2-WROOM-I
#define OLED_SDA   35 // FSPID
#define OLED_SCK   36 // FSPICLK
#define OLED_DC    8
#define OLED_CS    34 // GND
#define OLED_RESET 21

// ESP32 lolin32
#define OLED_SDA   23
#define OLED_SCK   18
#define OLED_DC    13
#define OLED_CS    5 // GND
#define OLED_RESET 15

BitMatrix screen1( SCREEN_WIDTH , vector<bool> (SCREEN_HEIGHT, 0));
BitMatrix screen2( SCREEN_WIDTH , vector<bool> (SCREEN_HEIGHT, 0));

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_SDA, OLED_SCK, OLED_DC, OLED_RESET, OLED_CS);


void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
   display.drawLine(x0, y0, x1, y1, SSD1306_WHITE);
   display.display();
}

void drawRectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
   display.drawRect(x0, y0, x1, y1, SSD1306_WHITE);
   display.display();
}

void fillRectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
   display.fillRect(x0, y0, x1, y1, SSD1306_WHITE);
   display.display();
}

void setPixel(uint8_t x, uint8_t y, bool bit=1, bool bothScreens=0){
    if (bit){
      display.drawPixel(x, y, SSD1306_WHITE);
      screen2[x][y] = 1;
      if (bothScreens){
         screen1[x][y] = 1;
      }
    }
    else {
      display.drawPixel(x, y, SSD1306_BLACK);
      screen2[x][y] = 0;
      if (bothScreens){
         screen1[x][y] = 0;
      }
    }
}

void clear_display(){
   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         setPixel(j, i, 0, 1);
      }
   }
}

void displayImage(uint8_t x, uint8_t y, BitMatrix& g, bool color=true, bool both=false){
   
   Serial.println("displayImage " + String(color));
   for(int i = 0; i < g.size(); i++){
      for(int j = 0; j < g.size(); j++){
         Serial.println("g[" + String(i) + "][" + String(j) + "] = " + String(g[j][i]));
         if(g[j][i]){
            setPixel(j+x, i+y, color, both);
            
            Serial.println("setPixel(" + String(j+x) + ", " + String(i+y) + ", "+ color +")");
         }
      }
   }
}

void spawnAsset(uint8_t x, uint8_t y, string name, uint8_t rot=0){
   if(name == "beehiveExplosion"){
      displayImage(x, y, beehiveExplosion_graphics, 1, 1);
   }
}

