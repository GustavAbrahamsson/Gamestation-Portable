
#include <Arduino.h>
#include <deque>

#define PIXEL_WIDTH = 2 // How many pixels wide on the screen

#define STARTING_POS_X = 20
#define STARTING_POS_Y = 40

const uint32_t snakePeriod = 50; // ms

const uint16_t STARTING_LENGTH = 20;

typedef std::pair<int8_t, int8_t> snakePair;


bool snakePairsOverlap(snakePair a_pos, snakePair b_pos){
    return a_pos.first == b_pos.first && a_pos.second == b_pos.second;
}


class Snake{
    private:
        
        snakePair startingPosition = {STARTING_LENGTH, 32};
        std::deque< snakePair > snakeTrain; // front: head of snake, back: tail of snake
        snakePair direction = {1, 0};
        uint32_t snakeSize;
        bool alive = 1;
        snakePair foodPos;

        snakePair calcStep(snakePair snakePart, snakePair direction_vector){
            int snakeX = snakePart.first;
            int snakeY = snakePart.second;
            int8_t dx = direction_vector.first;
            int8_t dy = direction_vector.second;

            snakePair outputPair;
            if(snakeX == 0 && dx == -1){
                snakeX = SCREEN_WIDTH - 1; // Make it come back into the screen from the top
            }else if(snakeX == SCREEN_WIDTH - 1 && dx == 1){
                snakeX = 0; // Make it come back into the screen from the bottom
            }else{
                snakeX += dx;
            }

            if(snakeY < 0 && dy == -1){
                snakeY = SCREEN_HEIGHT - 1; // Make it come back into the screen from the right
            }else if(snakeY == SCREEN_HEIGHT - 1 && dy == 1){
                snakeY = 0; // Make it come back into the screen from the left
            }else{
                snakeY += dy;
            }

            outputPair.first = snakeX;
            outputPair.second = snakeY;
            return outputPair;
        }

        void displaySnake(){
            // Go through all snakePairs and show them all on the display
            for (int i = 0; i < snakeSize; i++){
                setPixel(snakeTrain[i].first, snakeTrain[i].second);
            }
            
        }


        void hideSnake(){
            // Go through all snakePairs and hide them all on the display
            for (int i = 0; i < snakeSize; i++){
                setPixel(snakeTrain[i].first, snakeTrain[i].second, 0);
            }
        }

        void createSnake(){
            // Create pixels and add them to snakeTrain
            snakeHead = snakeTrain.front();
            for(int i = 1; i < snakeSize; i++){ // Start at 1 to skip the head
                snakePair inFront = snakeTrain.back();
                int8_t newX = inFront.first - direction.first; // Add it backwards
                int8_t newY = inFront.second - direction.second; // Add it backwards
                snakePair newTail = {newX, newY};
                snakeTrain.push_back(newTail); // Add it to the back of the snake
            }
        }

    public:
        snakePair snakeHead;

        void initializeSnake(int startingX, int startingY, int dx, int dy, uint16_t size=STARTING_LENGTH){
            // Warning: start the snake in a way where it fits in the
            startingPosition.first = startingX;
            startingPosition.second = startingY;
            snakeTrain.push_back(startingPosition);
            direction = {dx, dy};
            snakeSize = size;
            createSnake();
            displaySnake();
        }

        void extendTail(uint8_t extending){
            for(int i = 0; i < extending; i++) {
                snakePair back = snakeTrain.back();

                back = calcStep(back, {-direction.first, -direction.second});

                snakeTrain.push_back(back);
            }
            snakeSize += extending;
        }

        bool isAlive(){
            return alive;
        }

        void kill(){
            // Kills snake
            alive = 0;
        }

        bool headIsOnPosition(snakePair pos){
            return snakePairsOverlap(snakeHead, pos);
        }

        void updateFoodPos(snakePair newFoodPos){
            foodPos = newFoodPos;
        }

        void snakeStep(int8_t dx, int8_t dy){

            if(!alive){
                Serial.println("Snek ded, GAME OVER");
                for(int i = 0; i < 5; i++){
                    hideSnake();
                    display.display();
                    delay(100);
                    displaySnake();
                    display.display();
                    delay(100);
                }
            }else{

                

                if ((dx != 0 || dy != 0) && -direction.first != dx && -direction.second != dy){
                    direction = {dx, dy}; // Update the direction
                }
                
                snakePair newHead = calcStep(snakeTrain.front(), direction);

                int8_t newX = newHead.first; // Hours of debugging and this was the solution
                int8_t newY = newHead.second;

                if(activePixels[newX][newY] && !snakePairsOverlap(newHead, foodPos)){
                    alive = 0;
                }
                /*
                Serial.println("snakeStep(), dx = " + (String)direction.first + ", dy = " + (String)direction.second);
                Serial.println("Old head:" + (String)snakeTrain.front().first + "  " + (String)snakeTrain.front().second);
                
                Serial.println("New head:" + (String)newHead.first + "  " +  (String)newHead.second);
                */



                
                snakeHead = newHead;
                snakeTrain.push_front(snakeHead); // Move the head to new heading position
                // Remove the tail
                
                snakePair tailPiece = snakeTrain.back();
                snakeTrain.pop_back();
                
                //Serial.println("New head: " + (String)newX + "  " +  (String)newY);
                setPixel(newX, newY); // Make the new head pixel visible
                int8_t tailX = tailPiece.first;
                int8_t tailY = tailPiece.second;

                //Serial.println("New end: " + (String)tailX + "  " +  (String)tailY);
                setPixel(tailX, tailY, 0); // Cut off the tail bit
            }
        }
    
        void spawnSnake(){
            // Go through 'snakeTrain' and print each pixel
            
        }   

};

extern Snake snake;