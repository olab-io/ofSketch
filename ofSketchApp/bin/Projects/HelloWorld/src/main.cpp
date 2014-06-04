// #pragma once


#include "ofMain.h"
#include "Ball.h"

class ofApp: public ofBaseApp
{
public:



int numBalls;
vector<Ball> balls; 

void setup() {
    
    ofSetWindowShape(500, 500);
    ofBackground(255);
    numBalls = 75;
    
    for (int i = 0; i < numBalls; i++) {
        balls.push_back(Ball());
    }
}

void draw() {
    
    ofDrawBitmapStringHighlight("Hello World!", 200, 200);
    
    for (int i = 0; i < numBalls; i++){
        balls[i].update(mouseX, mouseY);
        balls[i].draw();
    }

}


};

int main()
{
    ofSetupOpenGL(320,240,OF_WINDOW);
    ofRunApp(new ofApp());
}


