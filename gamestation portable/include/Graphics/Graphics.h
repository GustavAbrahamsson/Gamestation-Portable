
using namespace std;

bool gliderDL[3][3] = {
    {0, 0, 1},
    {1, 1, 0},
    {0, 1, 1}
};

#include <stdint.h>

uint8_t image_data_GliderDL[3] = {
    // ∙ ∙ █
    // █ █ ∙
    // ∙ █ █
    0x80,   // 1 0 0 0  0 0 0 0
    0x60,   // 0 1 1 0  0 0 0 0
    0xc0    // 1 1 0 0  0 0 0 0
};

bool gliderDL_graphics[3][3];

void image_data_to_matrix(uint8_t* image_data, uint8_t s, bool targetArray[sizeof(image_data)-1][sizeof(image_data)-1]){
    for(int i = 0; i < s; i++){
        image_data[i] >> (4 + 4-s);
        for(int j = 0; j < s; j++){
            targetArray[i][j] = image_data[i] & 0x1;
        }
    }
}

void initGraphics(){
   uint8_t size = sizeof(image_data_GliderDL);
   image_data_to_matrix(image_data_GliderDL, size, gliderDL_graphics);
}