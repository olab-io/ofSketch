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
#include "ProcessTaskQueue.h"
#include "EditorSettings.h"
#include "OfSketchSettings.h"


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

    void exit();

    void mousePressed(int x, int y, int button);

    void loadProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void loadTemplateProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void saveProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void createProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void deleteProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void renameProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void createClass(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void deleteClass(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void renameClass(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void runProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void compileProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void stop(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void getProjectList(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void loadEditorSettings(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void saveEditorSettings(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void loadOfSketchSettings(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void saveOfSketchSettings(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void requestProjectClosed(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void notifyProjectClosed(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void requestAppQuit(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void getAddonList(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void getProjectAddonList(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void addProjectAddon(const void* pSender, ofx::JSONRPC::MethodArgs& args);
    void removeProjectAddon(const void* pSender, ofx::JSONRPC::MethodArgs& args);

    bool onWebSocketOpenEvent(ofx::HTTP::WebSocketOpenEventArgs& args);
    bool onWebSocketCloseEvent(ofx::HTTP::WebSocketCloseEventArgs& args);
    bool onWebSocketFrameReceivedEvent(ofx::HTTP::WebSocketFrameEventArgs& args);
    bool onWebSocketFrameSentEvent(ofx::HTTP::WebSocketFrameEventArgs& args);
    bool onWebSocketErrorEvent(ofx::HTTP::WebSocketErrorEventArgs& args);

    bool onHTTPPostEvent(ofx::HTTP::PostEventArgs& args);
    bool onHTTPFormEvent(ofx::HTTP::PostFormEventArgs& args);
    bool onHTTPUploadEvent(ofx::HTTP::PostUploadEventArgs& args);

    void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    void onSSLPrivateKeyPassphraseRequired(std::string& args);

    bool onTaskQueued(const ofx::TaskQueuedEventArgs& args);
    bool onTaskStarted(const ofx::TaskStartedEventArgs& args);
    bool onTaskCancelled(const ofx::TaskCancelledEventArgs& args);
    bool onTaskFinished(const ofx::TaskFinishedEventArgs& args);
    bool onTaskFailed(const ofx::TaskFailedEventArgs& args);
    bool onTaskProgress(const ofx::TaskProgressEventArgs& args);
    bool onTaskData(const ofx::TaskDataEventArgs<std::string>& args);

    // TODO: Move this.
    // Wraps a json method in the ofSketch protocol headers.
    static Json::Value toJSONMethod(const std::string& module,
                                    const std::string& method,
                                    const Json::Value& params);

    // TODO: Move this.
    // This is a utility method for quickly converting a json value to a string.
    static std::string toJSONString(const Json::Value& json);

    // TODO: Move this.
    static std::string getVersion();
    static int getVersionMajor();
    static int getVersionMinor();
    static int getVersionPatch();
    static std::string getVersionSpecial();

    // TODO: HACK while openFrameworks core is updated.
    // - https://github.com/openframeworks/openFrameworks/issues/2162
    // - https://github.com/openframeworks/openFrameworks/pull/3109
    static ofTargetPlatform getTargetPlatform();
    
    // TODO: Move this.
    static std::string toString(ofTargetPlatform targetPlatform);

    enum Version
    {
        VERSION_MAJOR = 0,
        VERSION_MINOR = 3,
        VERSION_PATCH = 0
    };

    static const std::string VERSION_SPECIAL;

private:
    ofx::HTTP::BasicJSONRPCServer::SharedPtr server;

//    WebSocketLoggerChannel::SharedPtr _loggerChannel;

    EditorSettings      _editorSettings;
    OfSketchSettings    _ofSketchSettings;

    Poco::ThreadPool    _threadPool;
    ProcessTaskQueue    _taskQueue;
    Compiler            _compiler;
    AddonManager        _addonManager;
    ProjectManager      _projectManager;

    ofImage _logo;
    ofTrueTypeFont _font;

};


} } // namespace of::Sketch
