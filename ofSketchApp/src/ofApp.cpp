// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"


void ofApp::setup()
{
#ifndef NO_WINDOW
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    _logo.loadImage("media/openFrameworks.jpg");
    _font.loadFont(OF_TTF_SANS, 24);
#endif

    app.setup();

    ofTargetPlatform arch = of::Sketch::SketchUtils::getTargetPlatform();

    if (arch != OF_TARGET_LINUXARMV6L && arch != OF_TARGET_LINUXARMV7L)
    {
        // Launch a browser with the address of the server.
        // ofLaunchBrowser(app.getServer()->getURL());
    }

}


void ofApp::draw()
{
#ifndef NO_WINDOW
    ofBackground(255);

    ofSetColor(255);
    _logo.draw(10, 0);

    ofSetColor(80);
    _font.drawString("Launch", 70, 30);
#endif
    
}


void ofApp::mousePressed(int x, int y, int button)
{
//    ofLaunchBrowser(app.getServer()->getURL());
}
