
#include <Arduino.h>
#include <deque>

#define PIXEL_WIDTH = 2 // How many pixels wide on the screen

#define STARTING_POS_X = 20
#define STARTING_POS_Y = 40

const uint32_t snakePeriod = 20; // ms

const uint16_t STARTING_LENGTH = 20;

class Snake{
    private:
        typedef std::pair<int16_t, int16_t> snakePair;
        snakePair startingPosition = {STARTING_LENGTH, 32};
        std::deque< snakePair > snakeTrain; // front: head of snake, back: tail of snake
        snakePair direction = {1, 0};
        uint16_t snakeSize;

        snakePair constrainMovement(snakePair snakePart){
            int snakeX = snakePart.first;
            int snakeY = snakePart.second;
            snakePair outputPair;
            if(snakeX < 0){
                snakeX += SCREEN_WIDTH; // Make it come back into the screen from the top
            }else if(snakeX > SCREEN_WIDTH - 1){
                snakeX -= SCREEN_WIDTH; // Make it come back into the screen from the bottom
            }
            if(snakeY < 0){
                snakeY += SCREEN_HEIGHT; // Make it come back into the screen from the right
            }else if(snakeY > SCREEN_HEIGHT - 1){
                snakeY -= SCREEN_HEIGHT; // Make it come back into the screen from the left
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

        bool selfBite(snakePair new_head){
            return 0;
        }

        void hideSnake(){
            // Go through all snakePairs and hide them all on the display
        }

        void createSnake(){
            // Create pixels and add them to snakeTrain
            for(int i = 1; i < snakeSize; i++){ // Start at 1 to skip the head, which is already
                snakePair inFront = snakeTrain.back();
                int newX = inFront.first - direction.first; // Add it backwards
                int newY = inFront.second - direction.second; // Add it backwards
                snakePair newTail = {newX, newY};
                snakeTrain.push_back(newTail); // Add it to the back of the snake
            }
        }

    public:
        Snake(){

        }

        void initializeSnake(int startingX, int startingY, int dx, int dy, uint16_t size=STARTING_LENGTH){
            snakeTrain.push_back(startingPosition);
            direction = {dx, dy};
            snakeSize = size;
            createSnake();
            displaySnake();
        }

        void snakeStep(int16_t dx, int16_t dy){
            if (dx != 0 || dy != 0 && -direction.first != dx && -direction.second != dy){
                direction = {dx, dy}; // Update the direction
            }
            
            int oldX = snakeTrain.front().first;
            int oldY = snakeTrain.front().second;

            int newX = oldX + direction.first;
            int newY = oldY + direction.second;
            snakePair newHead = {newX, newY};
            newHead = constrainMovement(newHead);
            /*
            Serial.println("snakeStep(), dx = " + (String)direction.first + ", dy = " + (String)direction.second);
            Serial.println("Old head:" + (String)snakeTrain.front().first + "  " + (String)snakeTrain.front().second);
            
            Serial.println("New head:" + (String)newHead.first + "  " +  (String)newHead.second);
            */
            if (selfBite(newHead)){
                // game over
            }

            snakeTrain.push_front(newHead); // Move the head to new heading position
             // Remove the tail
            
            snakePair tailPiece = snakeTrain.back();
            snakeTrain.pop_back();

            setPixel(newX, newY); // Make the new head pixel visible
            int16_t tailX = tailPiece.first;
            int16_t tailY = tailPiece.second;

            Serial.println("New pos:" + (String)tailX + "  " +  (String)tailY);
            setPixel(tailX, tailY, 0); // Cut off the tail bit
        
        }
    
        void spawnSnake(){
            // Go through 'snakeTrain' and print each pixel
            
        }   

};

extern Snake snake;