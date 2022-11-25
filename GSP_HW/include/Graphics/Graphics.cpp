#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

using namespace std;

typedef vector<vector<bool>> BitMatrix;

bool gliderUR[3][3] = {
    {0, 0, 1},
    {1, 1, 0},
    {0, 1, 1}
};

bool beehiveExplosion[5][5] = {
    {0, 1, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 1, 0},
    {0, 0, 1, 1, 0}
};

#include <stdint.h>

uint8_t image_data_GliderUR[3] = {
    // ∙ ∙ █
    // █ █ ∙
    // ∙ █ █
    0x80,   // 1 0 0 0  0 0 0 0
    0x60,   // 0 1 1 0  0 0 0 0
    0xc0    // 1 1 0 0  0 0 0 0
};

BitMatrix gliderUR_graphics(3,vector<bool>(3,0));
BitMatrix beehiveExplosion_graphics(5,vector<bool>(5,0));

void image_data_to_matrix(uint8_t* image_data, uint8_t s, BitMatrix& targetArray){
    for(int i = 0; i < s; i++){
        image_data[i] >> (4 + 4-s);
        for(int j = 0; j < s; j++){
            targetArray[i][j] = image_data[i] & 0x1;
        }
    }
}

uint8_t matrix_size = 0;
/*
void fillMatrixFromData(BitMatrix& target, bool data[matrix_size][matrix_size], uint8_t s){

}*/

void initGraphics(){
    uint8_t size = sizeof(image_data_GliderUR);
    image_data_to_matrix(image_data_GliderUR, size, gliderUR_graphics);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            gliderUR_graphics[i][j] = gliderUR[i][j];
        }
    }
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            beehiveExplosion_graphics[i][j] = beehiveExplosion[i][j];
        }
    }


}