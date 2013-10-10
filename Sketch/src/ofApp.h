// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
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


#pragma once


#include "ofMain.h"
#include "ofxJSONElement.h"
#include "BaseServer.h"
#include "FileSystemRoute.h"
#include "FileSystemRouteSettings.h"
#include "WebSocketEvents.h"
#include "WebSocketFrame.h"
#include "WebSocketRoute.h"
#include "WebSocketRouteSettings.h"


using ofx::HTTP::BaseServer;
using ofx::HTTP::BaseServerSettings;
using ofx::HTTP::FileSystemRoute;
using ofx::HTTP::FileSystemRouteSettings;
using ofx::HTTP::WebSocketEventArgs;
using ofx::HTTP::WebSocketFrameEventArgs;
using ofx::HTTP::WebSocketFrame;
using ofx::HTTP::WebSocketRoute;
using ofx::HTTP::WebSocketRouteSettings;


class ofApp: public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);


    void onWebSocketOpenEvent(WebSocketEventArgs& evt);
    void onWebSocketCloseEvent(WebSocketEventArgs& evt);
    void onWebSocketFrameReceivedEvent(WebSocketFrameEventArgs& evt);
    void onWebSocketFrameSentEvent(WebSocketFrameEventArgs& evt);
    void onWebSocketErrorEvent(WebSocketEventArgs& evt);

    void make(const std::string& sketchPath, const std::string& target);

private:
    BaseServer::SharedPtr         server;
    FileSystemRoute::SharedPtr    fileSystemRoute;
    WebSocketRoute::SharedPtr     webSocketRoute;


};
