#include <Arduino.h>
#include <map>

using namespace std;

typedef vector<vector<bool>> BitMatrix;


class XandY{
    public:
        XandY(){
            x = 0;
            y = 0;
        }
        uint8_t x;
        uint8_t y;    
};

class Actor{
    public:
        XandY pos;
        XandY vel;
        XandY old_pos;
        XandY old_vel;
        int16_t rotation = 0;
        void spawn(){
            setVisible(1);
            alive = 1;
        }
        void kill(){
            setVisible(0);
            alive = 0;
        }
        void setPos(uint8_t X, uint8_t Y){
            pos.x = X;
            pos.y = Y;
        }
        void setVisible(bool bit=1){
            visible = bit;
        }
        void rotate(int16_t deg){
            rotation += deg;
            rotateGraphics();
        }
        void setRotation(int16_t deg){
            rotation = deg;
            rotateGraphics();
        }
    private:
        virtual void rotateGraphics();
        bool visible = 1;
        bool alive = 1;
};

class Ball: public Actor{
   public:
        uint8_t r;
        Ball(uint8_t x_pos, uint8_t y_pos, uint8_t radius){
            pos.x = x_pos;
            pos.y = y_pos;
            vel.x = 0;
            vel.y = 0;
            r = radius;
        }
};

class Glider: public Actor{
   public:
        Glider(uint8_t x_pos, uint8_t y_pos, int16_t rot){
            pos.x = x_pos;
            pos.y = y_pos;
            old_pos.x = x_pos;
            old_pos.y = y_pos;
            vel.x = 0;
            vel.y = 0;
            initGraphics();
            setRotation(rot);
            spawn();
            setVisible();
            display();
            Serial.println("Created Glider");
        }

        void clear(){
            displayImage(old_pos.x, old_pos.y, graphics, 0, true);
        }
        void display(){
            clear();
            displayImage(pos.x, pos.y, graphics, 1, true);
        }
    private:
        void rotateGraphics(){
            BitMatrix temp(3, vector<bool> (3, 0));
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    temp[j][i] = graphics[j][i];
                }
            }
            switch(rotation){
                case 90:
                    for(int i = 0; i < 3; i++){
                        for(int j = 0; j < 3; j++){
                            graphics[i][j] = temp[j][2-i];
                        }
                    }
                    break;
                case 180:
                    for(int i = 0; i < 3; i++){
                        for(int j = 0; j < 3; j++){
                            graphics[j][i] = temp[i][j];
                        }
                    }
                    break;
                case -90:
                    for(int i = 0; i < 3; i++){
                        for(int j = 0; j < 3; j++){
                            graphics[i][j] = temp[2-j][i];
                        }
                    }
                    break;
                default:
                    break;
            }

        }
        void initGraphics(){
            graphics = BitMatrix(height,vector<bool>(width,0));
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    graphics[j][i] = gliderUR[j][i];
                }
            }
        }
        uint8_t height = 3;
        uint8_t width = 3;
        BitMatrix graphics;
};

std::map<uint16_t, Glider*> gliderMap;
uint8_t gliderIndex = 0;

void spawnObject(uint8_t x, uint8_t y, string name, int16_t rot=0){
    Serial.println("Spawn");
    if(name == "GliderUR"){
        if (gliderIndex >= 255) gliderIndex = 0;
        Glider* createdGlider = new Glider(x, y, rot);
        Serial.println("new Glider()");
        pair<uint16_t, Glider*> p = make_pair(gliderIndex, createdGlider);
        Serial.println("pair");
        
        gliderMap.erase(gliderIndex);
        Serial.println("Removed Glider with index " + gliderIndex);
        gliderMap.insert(p);
        Serial.println("insert p");
        

        Serial.println("Added Glider with index " + gliderIndex);
        gliderIndex++;
   }

}