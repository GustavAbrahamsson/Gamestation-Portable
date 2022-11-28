#include <Arduino.h>
#include <deque>
#include "Display.cpp"

#define PIXEL_WIDTH = 2 // How many pixels wide on the screen

#define STARTING_POS_X = 20
#define STARTING_POS_Y = 40

const uint8_t STARTING_LENGTH = 3;

std::pair<uint8_t, uint8_t> foodPosition;

class Snake{
    private:
        std::deque< std::pair<uint8_t,uint8_t> > snakeTrain;
        std::pair<uint8_t, uint8_t> direction = {0, -1};
        uint8_t size = STARTING_LENGTH;

        void display(){
            spawnSnake();
            
        }

        void createSnake(){
            // Create pixels and add them to snakeTrain
        }

    public:
    
        void spawnSnake(){
            // Go through 'snakeTrain' and print each pixel
            for(int i = 0; i < snakeTrain.size(); i++){
                setPixel(snakeTrain[i].first, snakeTrain[i].second);
            }
        }   

};


void displayFood(){
    // Use foodPosition to spawn a square with width PIXEL_WIDTH
}

void initFood(){
    // Spawn food at a random position and update foodPosition
    displayFood();
}

void setupSnake(){
   initFood();
}

void displaySnake(){
    //
}

void snakeIteration(){

}