// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
//               2014 Brannon Dorsey <http://brannondorsey.com>
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


#include <json/json.h>
#include "Poco/Pipe.h"
#include "Poco/Process.h"
#include "Poco/StreamCopier.h"
#include "Poco/PipeStream.h"
#include "Poco/TaskManager.h"
#include "ofMain.h"
#include "ofxHTTP.h"
#include "ofxJSONRPC.h"
#include "AddonManager.h"
#include "Project.h"
#include "ProjectManager.h"
#include "Compiler.h"
#include "WebSocketLoggerChannel.h"


using namespace ofx;


namespace of {
namespace Sketch {


class App: public ofBaseApp
{
public:
    App();
    ~App();
    void setup();
    void update();
    void draw();

    void loadProject(const void* pSender, JSONRPC::MethodArgs& args);
    void loadTemplateProject(const void* pSender, JSONRPC::MethodArgs& args);
    void saveProject(const void* pSender, JSONRPC::MethodArgs& args);
    void createProject(const void* pSender, JSONRPC::MethodArgs& args);
    void deleteProject(const void* pSender, JSONRPC::MethodArgs& args);
    void renameProject(const void* pSender, JSONRPC::MethodArgs& args);
    void createClass(const void* pSender, JSONRPC::MethodArgs& args);
    void deleteClass(const void* pSender, JSONRPC::MethodArgs& args);
    void renameClass(const void* pSender, JSONRPC::MethodArgs& args);
    void run(const void* pSender, JSONRPC::MethodArgs& args);
    void stop(const void* pSender, JSONRPC::MethodArgs& args);
    void getProjectList(const void* pSender, JSONRPC::MethodArgs& args);

    bool onWebSocketOpenEvent(HTTP::WebSocketEventArgs& args);
    bool onWebSocketCloseEvent(HTTP::WebSocketEventArgs& args);
    bool onWebSocketFrameReceivedEvent(HTTP::WebSocketFrameEventArgs& args);
    bool onWebSocketFrameSentEvent(HTTP::WebSocketFrameEventArgs& args);
    bool onWebSocketErrorEvent(HTTP::WebSocketEventArgs& args);

    bool onHTTPPostEvent(HTTP::PostEventArgs& args);
    bool onHTTPFormEvent(HTTP::PostFormEventArgs& args);
    bool onHTTPUploadEvent(HTTP::PostUploadEventArgs& args);

    void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLPrivateKeyPassphraseRequired(std::string& args);

private:
    HTTP::BasicJSONRPCServer::SharedPtr server;

    Poco::TaskManager _taskManager;

    ProjectManager::SharedPtr _projectManager;
    AddonManager::SharedPtr _addonManager;

    Project::SharedPtr _currentProject;
    Compiler _compiler;

    WebSocketLoggerChannel::SharedPtr _loggerChannel;

};


} } // namespace of::Sketch
