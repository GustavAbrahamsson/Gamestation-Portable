#include <Arduino.h>
#include <map>
using namespace std;

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
        void rotate(uint16_t deg){
            rotation += deg;
        }
        void setRotation(uint16_t deg){
            rotation = deg;
        }
    private:
        bool visible = 1;
        bool alive = 1;
        uint16_t rotation = 0;

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
        Glider(uint8_t x_pos, uint8_t y_pos, uint16_t rot){
            pos.x = x_pos;
            pos.y = y_pos;
            old_pos.x = x_pos;
            old_pos.y = y_pos;
            vel.x = 0;
            vel.y = 0;
            setRotation(rot);
            spawn();
            setVisible();
            initGraphics();
            display();
            Serial.println("Created Glider");
        }
        void initGraphics(){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 3; j++){
                    graphics[j][i] = gliderDL[j][i];
                }
            }
        }
        void clear(){
            displayImage(old_pos.x, old_pos.y, 3, graphics, 0, 1);
        }
        void display(){
            clear();
            displayImage(pos.x, pos.y, 3, graphics, 1, 1);
        }
    private:
        bool graphics[3][3];
};

std::map<uint16_t, Glider*> gliderMap;
uint8_t gliderIndex = 0;

void spawn(uint8_t x, uint8_t y, string name, uint8_t rot){
    Serial.println("Spawn");
   if(name == "GliderDL"){
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