// #pragma once

#include "ofMain.h"

class ofApp: public ofBaseApp
{
public:
    float scale;

    void setup()
    {
        ofSetFrameRate(30);
        scale = 1;
    }

    void draw()
    {
        scale += 0.2;
        if(scale > 2) {
            scale = 0;
        }

        ofBackgroundGradient(ofMap(scale,0,2,0,255),ofMap(scale,0,2,255,0));
        ofDrawBitmapStringHighlight("Hello!",110,120,ofColor(200),ofColor(0));
    }
};

int main()
{
    ofSetupOpenGL(320,240,OF_WINDOW);
    ofRunApp(new ofApp());
}

