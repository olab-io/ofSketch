#line 1 "GuiExample"

#include "ofMain.h"
#include "ofxGui.h"


class ofApp: public ofBaseApp
{
public:

#line 2 "GuiExample"

bool bHide;

ofxFloatSlider radius;
ofxColorSlider color;
ofxVec2Slider center;
ofxIntSlider circleResolution;
ofxToggle filled;
ofxButton twoCircles;
ofxButton ringButton;
ofxLabel screenSize;

ofxPanel gui;

ofSoundPlayer ring;

void setup() {
	ofSetVerticalSync(true);
	ofSetWindowShape(1024, 768);

	// we add this listener before setting up so the initial circle resolution is correct
	circleResolution.addListener(this, &ofApp::circleResolutionChanged);
	ringButton.addListener(this,&ofApp::ringButtonPressed);

	gui.setup(); // most of the time you don't need a name
	gui.add(filled.setup("fill", true));
	gui.add(radius.setup( "radius", 140, 10, 300 ));
	gui.add(center.setup("center",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
	gui.add(color.setup("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
	gui.add(circleResolution.setup("circle res", 5, 3, 90));
	gui.add(twoCircles.setup("two circles"));
	gui.add(ringButton.setup("ring"));
	gui.add(screenSize.setup("screen size", ""));

	bHide = true;

	ring.loadSound("ring.wav");
}


void exit() {
	ringButton.removeListener(this,&ofApp::ringButtonPressed);
}


void update() {
 	ofSetCircleResolution(circleResolution);
}


void draw() {
    ofBackgroundGradient(ofColor::white, ofColor::gray);
    
	if( filled ){
		ofFill();
	}else{
		ofNoFill();
	}

	ofSetColor(color);
	if(twoCircles){
		ofCircle(center->x-radius*.5, center->y, radius );
		ofCircle(center->x+radius*.5, center->y, radius );
	}else{
		ofCircle((ofVec2f)center, radius );
	}
	
	// auto draw?
	// should the gui control hiding?
	if( bHide ){
		gui.draw();
	}
}


void circleResolutionChanged(int & circleResolution)
{
	ofSetCircleResolution(circleResolution);
}


void ringButtonPressed()
{
	ring.play();
}


void keyPressed(int key) {
	if( key == 'h' ){
		bHide = !bHide;
	}
	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	if(key == 'l') {
		gui.loadFromFile("settings.xml");
	}
	if(key == ' '){
		color = ofColor(255);
	}
}


void windowResized(int w, int h) {
    screenSize = ofToString(w) + "x" + ofToString(h);
}



};

int main()
{
    ofSetupOpenGL(320, 240, OF_WINDOW);
    ofRunApp(new ofApp());
}

