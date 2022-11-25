#include <Arduino.h>
using namespace std;

bool firstBallBouncing = true;

void initBallBouncing(){

}

void runBallBouncing(){
    if(firstBallBouncing) initBallBouncing();
}