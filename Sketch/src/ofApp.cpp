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


#include "ofApp.h"

#include "Poco/Process.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"


//------------------------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);

    // set up file system route
    FileSystemRouteSettings fsSettings;
    fileSystemRoute = FileSystemRoute::makeShared(fsSettings);

    // set up websocket route
    WebSocketRouteSettings wsSettings;
    webSocketRoute = WebSocketRoute::makeShared(wsSettings);
    webSocketRoute->registerWebSocketEvents(this); // listen for events here

    // set up server
    BaseServerSettings settings;
    //settings.setPort(8080);
    server = std::shared_ptr<BaseServer>(new BaseServer(settings));

    // add routes to server (the last is evalulated first)
    server->addRoute(fileSystemRoute);
    server->addRoute(webSocketRoute);

    // start the server
    server->start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}

//------------------------------------------------------------------------------
void ofApp::update()
{
    
}

//------------------------------------------------------------------------------
void ofApp::draw()
{
}

//------------------------------------------------------------------------------
void ofApp::keyPressed(int key)
{
//
//    string arg = "all";
//    
//    if(key == 'c') {
//        arg = "clean";
//    }
//
//    if(key == 'r') {
//        arg = "run";
//    }
//
//
//    std::string cmd("make");
//    std::vector<std::string> args;
//    args.push_back("--directory=" + ofToDataPath("HelloWorldSketch",true));
//    args.push_back("-j10");
//    args.push_back("-s");
//    args.push_back(arg);
//    Poco::Pipe outPipe;
//    Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, 0, &outPipe, 0);
//
//
//    Poco::PipeInputStream istr(outPipe);
//
//    //    std::ofstream ostr("processes.txt");
//
//    Poco::StreamCopier::copyStream(istr,cout);
//
//
//
//    cout << "FINISHED! with " << ph.wait() << endl;
}

//------------------------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//------------------------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    
}

//------------------------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//------------------------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//------------------------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//------------------------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//------------------------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

//------------------------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketOpenEvent(WebSocketEventArgs& evt)
{
    std::string sketchPath = ofToDataPath("HelloWorldSketch/src/main.cpp",true);

    ofBuffer buffer = ofBufferFromFile(sketchPath);

    Json::Value setEditorSource;
    setEditorSource["method"] = "setEditorSource";
    Json::Value editorSource;
    editorSource["source"] = buffer.getText();
    setEditorSource["data"] = editorSource;

    cout << setEditorSource.toStyledString() << endl;

    evt.getConnectionRef().sendFrame(WebSocketFrame(setEditorSource.toStyledString()));

    cout << "Connection opened from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketCloseEvent(WebSocketEventArgs& evt)
{
    cout << "Connection closed from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketFrameReceivedEvent(WebSocketFrameEventArgs& evt)
{
//    cout << "Frame from: " << evt.getConnectionRef().getClientAddress().toString() << endl;

    ofxJSONElement json;

    if(json.parse(evt.getFrameRef().getText()))
    {
        if(json.isMember("command"))
        {
            std::string command = json["command"].asString();
            
            if(command == "RUN")
            {
                cout << json << endl;

                if(json.isMember("data"))
                {
                    std::string sketchPath = ofToDataPath("HelloWorldSketch",true);

                    ofBuffer buffer(json["data"].asString());

                    ofBufferToFile(sketchPath + "/src/main.cpp",
                                   buffer);

                    cout << "got the following text from editor" << endl;
                    cout << json["data"].asString() << endl;

                    make(sketchPath,"Debug");
                    make(sketchPath,"RunDebug");
                }
            }
            else
            {
                ofLogError("ofApp::onWebSocketFrameReceivedEvent") << "Unknown command: " << evt.getFrameRef().getText();
            }
        }
    }
    else
    {
        ofLogError("ofApp::onWebSocketFrameReceivedEvent") << "Unable to parse JSON: "  << evt.getFrameRef().getText();
    }
}

void ofApp::make(const std::string& sketchPath, const std::string& target)
{

    // TODO: lots
    std::string cmd("make");
    std::vector<std::string> args;
    args.push_back("--directory=" + sketchPath);
    // args.push_back("-j10");
    args.push_back("-s");
    args.push_back(target);

    Poco::Pipe outPipe;
    Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, 0, &outPipe, 0);

    Poco::PipeInputStream istr(outPipe);
    // could also save to text and return to web.
    // std::ofstream ostr("processes.txt");
    Poco::StreamCopier::copyStream(istr,cout);

}


//------------------------------------------------------------------------------
void ofApp::onWebSocketFrameSentEvent(WebSocketFrameEventArgs& evt)
{
    // frame was sent to clients
}

//------------------------------------------------------------------------------
void ofApp::onWebSocketErrorEvent(WebSocketEventArgs& evt)
{
    cout << "Error from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}


