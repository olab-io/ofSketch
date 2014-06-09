// #pragma once


#include "ofMain.h"
#include "Ball.h"

class ofApp: public ofBaseApp
{
public:



int numBalls = 75; // The total number of balls we'll use.

vector<Ball> balls; // A collection of all balls.

void setup() {
    // Enable vertical sync.
    ofSetVerticalSync(true);
    
    // Set the window size.
    ofSetWindowShape(500, 500);
    
    // Set the background color.
    ofBackground(255);

    // Create the balls
    for (int i = 0; i < numBalls; i++) {
        balls.push_back(Ball());
    }
}

void update() {
    // Update all balls.
    for (int i = 0; i < balls.size(); i++) {
        balls[i].update();
    }
}

void draw() {
    // Draw all balls.
    for (int i = 0; i < balls.size(); i++) {
        balls[i].draw();
    }
    
    // Log a message.
    ofLogNotice("draw()") << ofGetTimestampString();
}


};

int main()
{
    ofSetupOpenGL(320,240,OF_WINDOW);
    ofRunApp(new ofApp());
}


