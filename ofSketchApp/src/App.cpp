// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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


#include "App.h"


namespace of {
namespace Sketch {


void App::setup()
{
    ofSetFrameRate(30);

    _currentProject.reset();
    _currentConnection = 0;

    _projectManager = ProjectManager::makeShared(ofToDataPath("Projects"));
    _addonManager = AddonManager::makeShared(ofToDataPath("openFrameworks/addons"));


    // set up file upload route
    FileUploadRouteSettings fuSettings;
    fileUploadRoute = FileUploadRoute::makeShared(fuSettings);
    fileUploadRoute->registerFileUploadEvents(this);

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
    server->addRoute(fileUploadRoute);
    server->addRoute(fileSystemRoute);
    server->addRoute(webSocketRoute);

    // start the server
    server->start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server->getURL());
}

void App::update()
{
}

void App::draw()
{
    ofBackground(0);
}

void App::onWebSocketOpenEvent(WebSocketEventArgs& evt)
{
}

void App::onWebSocketCloseEvent(WebSocketEventArgs& evt)
{
    cout << "Connection closed from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

void App::onWebSocketFrameReceivedEvent(WebSocketFrameEventArgs& evt)
{
//    cout << "Frame from: " << evt.getConnectionRef().getClientAddress().toString() << endl;

    if(evt.getFrameRef().isBinary())
    {
        sendError(evt.getConnectionRef(),"Binary frames are not supported.");
        return;
    }

    std::string text = evt.getFrameRef().getText();

    ofxJSONElement json;

    if(json.parse(text))
    {
        std::string module;

        if(json.isMember("module"))
        {
            module = json["module"].asString();
//            if(json.isMember("command"))
//            {
//                std::string command = json["command"].asString();
//
//                if(command == "RUN")
//                {
//                    cout << json << endl;
//
//                    if(json.isMember("data"))
//                    {
//                        std::string sketchPath = ofToDataPath("HelloWorldSketch",true);
//
//                        ofBuffer buffer(json["data"].asString());
//
//                        ofBufferToFile(sketchPath + "/src/main.cpp",
//                                       buffer);
//
//                        cout << "got the following text from editor" << endl;
//                        cout << json["data"].asString() << endl;
//
//                        //                    make(sketchPath,"Debug");
//                        //                    make(sketchPath,"RunDebug");
//                    }
//                }
//                else
//                {
//                    ofLogError("ofApp::onWebSocketFrameReceivedEvent") << "Unknown command: " << evt.getFrameRef().getText();
//                }
//            }
        }
        else
        {
            // invalid module specified
            sendError(evt.getConnectionRef(),"Module is not specified: " + json.toStyledString());
            return;
        }
    }
    else
    {
        ofLogError("ofApp::onWebSocketFrameReceivedEvent") << "Unable to parse JSON: "  << evt.getFrameRef().getText();
    }
}

void App::onWebSocketFrameSentEvent(WebSocketFrameEventArgs& evt)
{
    // frame was sent to clients
}

void App::onWebSocketErrorEvent(WebSocketEventArgs& evt)
{
    cout << "Error from: " << evt.getConnectionRef().getClientAddress().toString() << endl;
}

void App::sendError(const WebSocketConnection& connection, std::string error)
{
    Json::Value json;
    json["method"] = "error";
    json["data"] = error;

    connection.sendFrame(json.toStyledString());

    ofLogError("App::sendError") << error;
}


void App::onFileUploadStarted(FileUploadEventArgs& args)
{
    uploadProgress[args.getFileName()] = 0;
}


void App::onFileUploadProgress(FileUploadEventArgs& args)
{
    // get normalized progress
    uploadProgress[args.getFileName()] = args.getNumBytesTransferred() / (float) args.getFileSize();
}


void App::onFileUploadFinished(FileUploadEventArgs& args)
{
    uploadProgress[args.getFileName()] = 1;
    // TODO: do something with it and delete it from the upload progres
}


} } // namespace of::Sketch
