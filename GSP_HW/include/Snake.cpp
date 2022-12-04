#include <Arduino.h>
#include <deque>

uint32_t snakeTime = 0; // ms

std::pair<int8_t, int8_t> foodPosition = {0, 0};

const uint8_t foodExtending = 30;

Snake snake;

void displayFood(){
    // Use foodPosition to spawn the food
    setPixel(foodPosition.first, foodPosition.second);
}

void hideFood(){
    setPixel(foodPosition.first, foodPosition.second, 0);
}

void spawnFood(){
    // Spawn food at a random position and update foodPosition
    int foodX = rand() % (SCREEN_WIDTH - 1);
    int foodY = rand() % (SCREEN_HEIGHT - 1);
    foodPosition.first = foodX;
    foodPosition.second = foodY;
    displayFood();
    Serial.println("Spawned food at " + (String)foodPosition.first + ", "+ (String)foodPosition.second);
}

void setupSnake(){
    spawnFood();
    snake.initializeSnake(40, 40, 1, 0); // Init the snake with the constructor parameters
}

std::pair<int8_t, int8_t> dir;
void snakeIteration(){
    dir = dPad_direction(dPad1_btn);
    snake.snakeStep(dir.first, dir.second);
    if (snake.snakeHead.first == foodPosition.first && snake.snakeHead.second == foodPosition.second){
        Serial.println("Nom");
        snake.extendTail(foodExtending);
        spawnFood();
    }
}

void snakeGame(uint32_t current_time){
    if(current_time - snakeTime > snakePeriod){ // main snake execution
        snakeTime = current_time;
        snakeIteration();
        display.display();
    }
}