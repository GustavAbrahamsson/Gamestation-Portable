#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA   23
#define OLED_SCK   18
#define OLED_DC    13
#define OLED_CS    5 // GND
#define OLED_RESET 15

vector<vector<bool> > screen1( SCREEN_WIDTH , vector<bool> (SCREEN_HEIGHT, 0));
vector<vector<bool> > screen2( SCREEN_WIDTH , vector<bool> (SCREEN_HEIGHT, 0));

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

void displayImage(uint8_t x, uint8_t y, uint8_t s, bool g[3][3], bool color=1, bool both=0){
   
   Serial.println("displayImage " + String(color));
   for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
         Serial.println("g[" + String(i) + "][" + String(j) + "] = " + String(g[j][i]));
         if(g[j][i]){
            setPixel(j+x, i+y, color, both);
            
            Serial.println("setPixel(" + String(j+x) + ", " + String(i+y) + ", "+ color +")");
         }
      }
   }
}
void doThing(){
   bool graphics[3][3];
   for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
         graphics[i][j] = gliderDL[i][j];
      }
   }  
   displayImage(1, 2, 3, graphics, 1);
}
