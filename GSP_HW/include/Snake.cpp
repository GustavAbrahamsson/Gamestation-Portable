#include <Arduino.h>
#include <deque>

uint32_t snakeTime = 0; // ms

std::pair<uint16_t, uint16_t> foodPosition;

Snake snake;
//Snake* snake = &Snake(10, 32, 1, 0, 5); // Create the snake object as as extern variable so that it's accessable globally(?)

void displayFood(){
    // Use foodPosition to spawn a square with width PIXEL_WIDTH
}

void initFood(){
    // Spawn food at a random position and update foodPosition
    displayFood();
}

void setupSnake(){
    initFood();
    snake.initializeSnake(10, 32, 1, 0); // Init the snake with the constructor parameters
}

std::pair<int16_t, int16_t> dir;
void snakeIteration(){
    dir = dPad_direction(dPad1_btn);
    snake.snakeStep(dir.first, dir.second);
    //Serial.println("Snakeiteration()");
}

void snakeGame(uint32_t current_time){
    if(current_time - snakeTime > snakePeriod){ // main snake execution
        snakeTime = current_time;
        snakeIteration();
        display.display();
    }
}