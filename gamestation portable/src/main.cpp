#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ctime>
#include <cstdlib>

using namespace std;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA   23
#define OLED_SCK   18
#define OLED_DC    13
#define OLED_CS    5 // GND
#define OLED_RESET 15

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_SDA, OLED_SCK, OLED_DC, OLED_RESET, OLED_CS);

vector<vector<bool> > screen1( SCREEN_WIDTH , vector<bool> (SCREEN_HEIGHT, 0));
vector<vector<bool> > screen2( SCREEN_WIDTH , vector<bool> (SCREEN_HEIGHT, 0));
   vector<vector<uint8_t> > neighbours( SCREEN_WIDTH , vector<uint8_t> (SCREEN_HEIGHT, 0));

class Ball{
   public:
      uint8_t x;
      uint8_t y;
      uint8_t r;
      vector<uint8_t> vel_vector;;
      Ball(uint8_t x_pos, uint8_t y_pos, uint8_t radius, vector<uint8_t> v){
         x = x_pos;
         y = y_pos;
         r = radius;
         vel_vector = v;
      }
};

void testdrawline() {
   int16_t i;

   display.clearDisplay(); // Clear display buffer

   for(i=0; i<display.width(); i+=4) {
      display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
      display.display(); // Update screen with each newly-drawn line
      delay(1);
   }
   for(i=0; i<display.height(); i+=4) {
      display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   delay(250);

   display.clearDisplay();

   for(i=0; i<display.width(); i+=4) {
      display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   for(i=display.height()-1; i>=0; i-=4) {
      display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   delay(250);

   display.clearDisplay();

   for(i=display.width()-1; i>=0; i-=4) {
      display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   for(i=display.height()-1; i>=0; i-=4) {
      display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   delay(250);

   display.clearDisplay();

   for(i=0; i<display.height(); i+=4) {
      display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }
   for(i=0; i<display.width(); i+=4) {
      display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
      display.display();
      delay(1);
   }

   delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
   display.clearDisplay();

   for(int16_t i=0; i<display.height()/2; i+=2) {
      display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
      display.display(); // Update screen with each newly-drawn rectangle
      delay(1);
   }

   delay(2000);
}

void testfillrect(void) {
   display.clearDisplay();

   for(int16_t i=0; i<display.height()/2; i+=3) {
      // The INVERSE color is used so rectangles alternate white/black
      display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
      display.display(); // Update screen with each newly-drawn rectangle
      delay(1);
   }

   delay(2000);
}

void testdrawcircle(void) {
   display.clearDisplay();

   for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
      display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testfillcircle(void) {
   display.clearDisplay();

   for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
      // The INVERSE color is used so circles alternate white/black
      display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
      display.display(); // Update screen with each newly-drawn circle
      delay(1);
   }

   delay(2000);
}

void testdrawroundrect(void) {
   display.clearDisplay();

   for(int16_t i=0; i<display.height()/2-2; i+=2) {
      display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
         display.height()/4, SSD1306_WHITE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testfillroundrect(void) {
   display.clearDisplay();

   for(int16_t i=0; i<display.height()/2-2; i+=2) {
      // The INVERSE color is used so round-rects alternate white/black
      display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
         display.height()/4, SSD1306_INVERSE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testdrawtriangle(void) {
   display.clearDisplay();

   for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
      display.drawTriangle(
         display.width()/2  , display.height()/2-i,
         display.width()/2-i, display.height()/2+i,
         display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testfilltriangle(void) {
   display.clearDisplay();

   for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
      // The INVERSE color is used so triangles alternate white/black
      display.fillTriangle(
         display.width()/2  , display.height()/2-i,
         display.width()/2-i, display.height()/2+i,
         display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
      display.display();
      delay(1);
   }

   delay(2000);
}

void testdrawchar(void) {
   display.clearDisplay();

   display.setTextSize(1);      // Normal 1:1 pixel scale
   display.setTextColor(SSD1306_WHITE); // Draw white text
   display.setCursor(0, 0);     // Start at top-left corner
   display.cp437(true);         // Use full 256 char 'Code Page 437' font

   // Not all the characters will fit on the display. This is normal.
   // Library will draw what it can and the rest will be clipped.
   for(int16_t i=0; i<256; i++) {
      if(i == '\n') display.write(' ');
      else          display.write(i);
   }

   display.display();
   delay(2000);
}

void testdrawstyles(void) {
   display.clearDisplay();

   display.setTextSize(1);             // Normal 1:1 pixel scale
   display.setTextColor(SSD1306_WHITE);        // Draw white text
   display.setCursor(0,0);             // Start at top-left corner
   display.println(F("Hello, world!"));

   display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
   display.println(3.141592);

   display.setTextSize(2);             // Draw 2X-scale text
   display.setTextColor(SSD1306_WHITE);
   display.print(F("0x")); display.println(0xDEADBEEF, HEX);

   display.display();
   delay(2000);
}

void testscrolltext(void) {
   display.clearDisplay();

   display.setTextSize(2); // Draw 2X-scale text
   display.setTextColor(SSD1306_WHITE);
   display.setCursor(10, 0);
   display.println(F("scroll"));
   display.display();      // Show initial text
   delay(100);

   // Scroll in various directions, pausing in-between:
   display.startscrollright(0x00, 0x0F);
   delay(2000);
   display.stopscroll();
   delay(1000);
   display.startscrollleft(0x00, 0x0F);
   delay(2000);
   display.stopscroll();
   delay(1000);
   display.startscrolldiagright(0x00, 0x07);
   delay(2000);
   display.startscrolldiagleft(0x00, 0x07);
   delay(2000);
   display.stopscroll();
   delay(1000);
}

void demo(){
   // Show initial display buffer contents on the screen --
   // the library initializes this with an Adafruit splash screen.
   display.display();
   delay(2000); // Pause for 2 seconds

   // Clear the buffer
   display.clearDisplay();

   // Draw a single pixel in white
   display.drawPixel(10, 10, SSD1306_WHITE);

   // Show the display buffer on the screen. You MUST call display() after
   // drawing commands to make them visible on screen!
   display.display();
   delay(2000);
   // display.display() is NOT necessary after every single drawing command,
   // unless that's what you want...rather, you can batch up a bunch of
   // drawing operations and then update the screen all at once by calling
   // display.display(). These examples demonstrate both approaches...

   testdrawline();      // Draw many lines
   testdrawrect();      // Draw rectangles (outlines)
   testfillrect();      // Draw rectangles (filled)
   testdrawcircle();    // Draw circles (outlines)
   testfillcircle();    // Draw circles (filled)
   testdrawroundrect(); // Draw rounded rectangles (outlines)
   testfillroundrect(); // Draw rounded rectangles (filled)
   testdrawtriangle();  // Draw triangles (outlines)
   testfilltriangle();  // Draw triangles (filled)
   testdrawchar();      // Draw characters of the default font
   testdrawstyles();    // Draw 'stylized' characters
   testscrolltext();    // Draw scrolling text
   
   // Invert and restore display, pausing in-between
   display.invertDisplay(true);
   delay(1000);
   display.invertDisplay(false);
   delay(1000);
}

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
         display.drawPixel(j, i, SSD1306_BLACK);
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
   Serial.println("gameOfLife");
   
   for(uint8_t i = 0; i < SCREEN_HEIGHT; i++){
      for(uint8_t j = 0; j < SCREEN_WIDTH; j++){
         //uint8_t neighbours = giveNeighbours(j, i, screen1);
         
         uint8_t n = giveNeighbours(j, i);

         bool pixel = screen1[j][i];

         
         if(pixel){ 
            if(n < 2 || n > 3){
               screen2[j][i] = 0;
               display.drawPixel(j, i, SSD1306_BLACK);
            }
            else{
               screen2[j][i] = 1;
               display.drawPixel(j, i, SSD1306_WHITE);
            }
         }
         else if (!pixel){
            if(n == 3){
               screen2[j][i] = 1;
               display.drawPixel(j, i, SSD1306_WHITE);
            }
            else{
               screen2[j][i] = 0;
               display.drawPixel(j, i, SSD1306_BLACK);
            }
         }
      }
   }
   screen1.swap(screen2);
   
   display.display();
}

void setup() {
   //Serial.begin(115200);
   srand(time(0));
   if(!display.begin(SSD1306_SWITCHCAPVCC)) {
      Serial.println(F("SSD1306 allocation failed"));
      while(1);
   }

   display.clearDisplay();

   initGameOfLife(25);
}

unsigned long currentTime = 0;
unsigned long lastTime = 0;

void loop(){
   
   currentTime = millis();
   if(currentTime - lastTime > 100){
      lastTime = currentTime;
      gameOfLife();
   }
   
}  