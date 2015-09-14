// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
// Copyright (c) 2014 Brannon Dorsey <http://brannondorsey.com>
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
#include "ofx/Net/IPAddressRange.h"
#include "Serializer.h"


namespace of {
namespace Sketch {


App::App():
    _threadPool("ofSketchThreadPool"),
    _taskQueue(ofx::TaskQueue_<std::string>::UNLIMITED_TASKS, _threadPool),
//    _compiler(_settings, _taskQueue),
    _toolchainManager(_settings),
    _addonManager(_settings),
    _projectManager(_settings),
//    _uploadRouter(_settings),
    _missingDependencies(true)
{
    // Hack to make sure that the net sybsystem is initialized on windows.
    Poco::Net::initializeNetwork();

    ofSSLManager::registerAllEvents(this);
    _taskQueue.registerAllEvents(this);
}


App::~App()
{
    _taskQueue.unregisterAllEvents(this);
    ofSSLManager::unregisterAllEvents(this);
}


void App::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel("ofThread", OF_LOG_ERROR);


    // Load the settings from the user's home directory if available.
    _settings = Serializer::loadSettings();

	const Paths& paths = _settings.paths();

	ofLogVerbose("App::setup()") << "Paths:       Projects: " << paths.getProjectsPath();
	ofLogVerbose("App::setup()") << "Paths:         Addons: " << paths.addonsPath();
	ofLogVerbose("App::setup()") << "Paths: openFrameworks: " << Paths::openFrameworksPath();
	ofLogVerbose("App::setup()") << "Paths:    Core Addons: " << Paths::coreAddonsPath();
	ofLogVerbose("App::setup()") << "Paths:       Examples: " << Paths::examplesPath();
	ofLogVerbose("App::setup()") << "Paths:      Resources: " << Paths::resourcesPath();
	ofLogVerbose("App::setup()") << "Paths:     Toolchains: " << Paths::toolchainsPath();
	ofLogVerbose("App::setup()") << "Paths:   	  Settings: " << Paths::settingsPath();

	// Set up the toolchains.
	_toolchainManager.setup();

	// Set up the addons.
	_addonManager.setup();


	// Set up the project manager.
    _projectManager.setup();

	
//    _uploadRouter.setup();

    try 
	{
        if (SketchUtils::hasDependency("make"))
        {
            _missingDependencies = false;
        }

        ofx::HTTP::JSONRPCServerSettings settings;
        settings.webSocketRouteSettings.setBufferSize(_settings.serverSettings().getWebSocketBufferSize());

        if (0 == _settings.serverSettings().getPort())
        {
            settings.setPort((unsigned short)ofRandom(8888,9999));
        }
        else
        {
            settings.setPort(_settings.serverSettings().getPort());
        }

        settings.postRouteSettings.setUploadRedirect("");
        settings.postRouteSettings.setMaximumFileUploadSize(_settings.serverSettings().getMaxiumFileUploadSize());
        settings.setWhitelist(_settings.serverSettings().getWhitelist());

        // Override for testing.
        settings.setPort(9178);

		_server = std::make_shared<ofx::HTTP::JSONRPCServer>(settings);

//        _server->getPostRoute()->registerPostEvents(&_uploadRouter);
        _server->getWebSocketRoute().registerWebSocketEvents(this);

        // Set up websocket logger.
        // _loggerChannel = WebSocketLoggerChannel::makeShared();
        // _loggerChannel->setWebSocketRoute(_server->getWebSocketRoute());
        // ofSetLoggerChannel(_loggerChannel);

        const SSLSettings& sslSettings = _settings.serverSettings().getSSLSettings();

		ofSSLManager::initializeServer(new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
															  ofToDataPath(sslSettings.getPrivateKeyPath(), true),
															  ofToDataPath(sslSettings.getCertificatePath(), true),
                                                              ofToDataPath(sslSettings.getCACertPath(), true)));

        // TODO: configure these via settings files
        _server->registerMethod("load-project",
                               "Load the requested project.",
                               this,
                               &App::loadProject);

        _server->registerMethod("load-template-project",
                               "Load an anonymous project.",
                               this,
                               &App::loadTemplateProject);

        _server->registerMethod("save-project",
                               "Save the current project.",
                               this,
                               &App::saveProject);

        _server->registerMethod("create-project",
                               "Create a new project.",
                               this,
                               &App::createProject);

        _server->registerMethod("delete-project",
                               "Delete the current project.",
                               this,
                               &App::deleteProject);

        _server->registerMethod("rename-project",
                               "Rename the current project.",
                               this,
                               &App::renameProject);

        _server->registerMethod("notify-project-closed",
                               "Notify the server that project was closed.",
                               this,
                               &App::notifyProjectClosed);

        _server->registerMethod("request-project-closed",
                               "Broadcast a project close request to connected clients.",
                               this,
                               &App::requestProjectClosed);

        _server->registerMethod("request-app-quit",
                               "Quit the app.",
                               this,
                               &App::requestAppQuit);

        _server->registerMethod("create-class",
                               "Create a new class for the current project.",
                               this,
                               &App::createClass);

        _server->registerMethod("delete-class",
                               "Delete a select class from for the current project.",
                               this,
                               &App::deleteClass);

        _server->registerMethod("rename-class",
                               "Rename a select class from for the current project.",
                               this,
                               &App::renameClass);

        _server->registerMethod("run-project",
                               "Run the requested project.",
                               this,
                               &App::runProject);

        _server->registerMethod("compile-project",
                               "Run the requested project.",
                               this,
                               &App::compileProject);

        _server->registerMethod("stop",
                               "Stop the requested project.",
                               this,
                               &App::stop);

        _server->registerMethod("get-project-list",
                               "Get list of all projects in the Project directory.",
                               this,
                               &App::getProjectList);

        _server->registerMethod("load-editor-settings",
                               "Get the editor settings.",
                               this,
                               &App::loadEditorSettings);
        
        _server->registerMethod("save-editor-settings",
                               "Save the editor settings.",
                               this,
                               &App::saveEditorSettings);
        
        _server->registerMethod("load-ofsketch-settings",
                               "Get ofSketch settings.",
                               this,
                               &App::loadOfSketchSettings);
        
        _server->registerMethod("save-ofsketch-settings",
                               "Save ofSketch settings.",
                               this,
                               &App::saveOfSketchSettings);
        
        _server->registerMethod("get-addon-list",
                               "Get a list of all addons.",
                               this,
                               &App::getAddonList);
        
        _server->registerMethod("get-project-addon-list",
                               "Get a list of addons for a project.",
                               this,
                               &App::getProjectAddonList);
        
        _server->registerMethod("add-project-addon",
                               "Add an addon to a project.",
                               this,
                               &App::addProjectAddon);
        
        _server->registerMethod("remove-project-addon",
                               "Remove an addon from a project.",
                               this,
                               &App::removeProjectAddon);
        
        _server->registerMethod("export-project",
                               "Export the project for target platform.",
                               this,
                               &App::exportProject);

		_server->start();

    }
    catch (const Poco::Exception& exc)
    {
		ofLogError() << exc.displayText();
    }

    Serializer::saveSettings(_settings);

}


void App::exit()
{
    // broadcast requestProjectClosed settings to all connected clients
    Json::Value params;
    params["foo"] = "bar";
    Json::Value json = SketchUtils::toJSONMethod("Server", "appExit", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
    ofLogNotice("App::exit") << "appExit frame broadcasted" << endl;

    _server->getWebSocketRoute().unregisterWebSocketEvents(this);
//    _server->getPostRoute()->unregisterPostEvents(&_uploadRouter);

    // Reset default logger.
    ofLogToConsole();
}


void App::loadProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::loadProject") << "";


//    if (args.params.isMember("projectName"))
//    {
//        std::string projectName = args.params["projectName"].asString();
//
//        if (_projectManager.projectExists(projectName))
//        {
//            _projectManager.loadProject(pSender, args);
//        }
//        else
//		{
//
//		}args.error["message"] = "The requested project does not exist.";
//    }
//	else {
//		args.error = ofx::JSONRPC::Error(ofx::JSONRPC::Errors::RPC_ERROR_INVALID_PARAMETERS, "missing project");
//	}args.error["message"] = "Incorrect parameters sent to load-project method.";
}


void App::loadTemplateProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::loadTemplateProject") << "";

	args.result = Serializer::toJSON(_projectManager.getTemplateProject());

	cout << args.result.toStyledString() << endl;

}


void App::saveProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::saveProject") << "";

//    std::string projectName = args.params["projectData"]["projectFile"]["name"].asString();
//
//    if (_projectManager.projectExists(projectName))
//    {
//        _projectManager.saveProject(pSender, args);
//        const Project& project = _projectManager.getProject(projectName);
//        _compiler.generateSourceFiles(project);
//    }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::createProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::createProject") << "";

//    std::string projectName = args.params["projectName"].asString();
//    if (!_projectManager.projectExists(projectName))
//    {
//        _projectManager.createProject(pSender, args);
//    }
//    else args.error["message"] = "That project name already exists.";
}


void App::deleteProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::deleteProject") << "";

//    std::string projectName = args.params["projectName"].asString();
//    if (_projectManager.projectExists(projectName))
//    {
//        _projectManager.deleteProject(pSender, args);
//        requestProjectClosed(pSender, args);
//    }
//    else args.error["message"] = "The project that you are trying to delete does not exist.";
}


void App::renameProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::renameProject") << "";

	//    std::string projectName = args.params["projectName"].asString();
//    if (_projectManager.projectExists(projectName))
//    {
//        _projectManager.renameProject(pSender, args);
//        requestProjectClosed(pSender, args);
//    }
//    else args.error["message"] = "The project that you are trying to delete does not exist.";
}


void App::notifyProjectClosed(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::notifyProjectClosed") << "";

//    std::string projectName = args.params["projectName"].asString();
//    _projectManager.notifyProjectClosed(projectName);
//    ofLogNotice("App::notifyProjectClosed") << projectName << " closed.";
}


void App::requestProjectClosed(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::requestProjectClosed") << "";

    std::string projectName = args.params["projectName"].asString();
    std::string clientUUID = args.params["clientUUID"].asString();

    // broadcast requestProjectClosed settings to all connected clients
    Json::Value params;
    params["projectName"] = projectName;
    params["clientUUID"] = clientUUID;
    Json::Value json = SketchUtils::toJSONMethod("Server", "requestProjectClosed", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
}


void App::requestAppQuit(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
	ofLogNotice("App::requestAppQuit") << "";

//    args.result = "App quit";
//    ofExit();
}


void App::createClass(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::createClass") << "";

//    std::string projectName = args.params["projectName"].asString();
//
//    if (_projectManager.projectExists(projectName))
//    {
//        std::string className = args.params["className"].asString();
//        Project& project = _projectManager.getProjectRef(projectName);
//        args.result["classFile"] = project.createClass(className);
//
//    }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::deleteClass(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::deleteClass") << "";

//    std::string projectName = args.params["projectName"].asString();
//    if (_projectManager.projectExists(projectName))
//    {
//        std::string className = args.params["className"].asString();
//        Project& project = _projectManager.getProjectRef(projectName);
//        if (project.deleteClass(className))
//        {
//            args.result["message"] = className + "class deleted.";
//        }
//        else args.error["message"] = "Error deleting the class.";
//    }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::renameClass(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::renameClass") << "";

//    std::string projectName = args.params["projectName"].asString();
//    if (_projectManager.projectExists(projectName))
//    {
//        std::string className = args.params["className"].asString();
//        std::string newClassName = args.params["newClassName"].asString();
//        Project& project = _projectManager.getProjectRef(projectName);
//        if (project.renameClass(className, newClassName))
//        {
//            args.result["message"] = className + " class renamed to " + newClassName;
//        }
//        else args.error["message"] = "Error renaming " + className + " class.";
//    }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::runProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::runProject") << "";

//    std::string projectName = args.params["projectName"].asString();
//    if (_projectManager.projectExists(projectName))
//    {
//        ofLogNotice("App::run") << "Running " << projectName << " project";
//        const Project& project = _projectManager.getProject(projectName);
//        Poco::UUID taskId = _compiler.run(project);
//        ofLogNotice("APP::run") << "Task ID: " << taskId.toString();
//        args.result = taskId.toString();
//    }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::compileProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::compileProject") << "";

//    std::string projectName = args.params["projectName"].asString();
//    if (_projectManager.projectExists(projectName))
//    {
//        ofLogNotice("App::compileProject") << "Compiling " << projectName << " project";
//        const Project& project = _projectManager.getProject(projectName);
//        Poco::UUID taskId = _compiler.compile(project);
//        ofLogNotice("App::compileProject") << "Task ID: " << taskId.toString();
//        args.result = taskId.toString();
//    }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::stop(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::stop") << "";

    if (args.params.isMember("taskId"))
    {
		std::string taskId(args.params["taskId"].asString());

        try
        {
            _taskQueue.cancel(taskId);
        }
        catch (const Poco::NotFoundException& exc)
        {
            ofLogWarning("App::stop") << "Task already stopped: " << taskId;
        }

        args.result = taskId;

        ofLogNotice("App::stop") << "Stopped task " << taskId;
    }
    else
    {
        // Invalid.
		args.error = ofx::JSONRPC::Error(ofx::JSONRPC::Errors::RPC_ERROR_INVALID_PARAMETERS, "No task id.");
    }
}


void App::getProjectList(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("Project::getProjectList") << "Project list requested.";

	args.result.clear();

	for (auto project: _projectManager.projects())
	{
		Json::Value projectJson;
		projectJson["projectName"] = project.name();
		args.result.append(projectJson);
	}
}


void App::getAddonList(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
	ofLogNotice("App::getAddonList") << "Addon list requested.";

	args.result.clear();

	for (auto addon: _addonManager.addons())
	{
		args.result.append(Serializer::toJSON(addon));
	}
}


void App::getProjectAddonList(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
	ofLogNotice("App::getProjectAddonList") << "";

//    std::string projectName = args.params["projectName"].asString();
//
//    if (_projectManager.projectExists(projectName))
//    {
//        const Project& project = _projectManager.getProject(projectName);
//
//        if (project.hasAddons())
//        {
//            std::vector<std::string> addons = project.getAddons();
//
//            for (unsigned int i = 0; i < addons.size(); ++i)
//            {
//                args.result["addons"][i] = addons[i];
//            }
//
//			args.result["hasAddons"] = true;
//        }
//        else
//        {
//            args.result["hasAddons"] = false;
//        }
//
//        }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::addProjectAddon(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
//    std::string projectName = args.params["projectName"].asString();
//    std::string addon = args.params["addon"].asString();
//
//    if (_projectManager.projectExists(projectName))
//    {
//        Project& project = _projectManager.getProjectRef(projectName);
//        project.addAddon(addon);
//    }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::removeProjectAddon(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
	ofLogNotice("App::removeProjectAddon") << "";

//    std::string projectName = args.params["projectName"].asString();
//    std::string addon = args.params["addon"].asString();
//
//    if (_projectManager.projectExists(projectName))
//    {
//        Project& project = _projectManager.getProjectRef(projectName);
//        project.removeAddon(addon);
//    }
//    else args.error["message"] = "The requested project does not exist.";
}


void App::loadEditorSettings(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
	args.result = _settings.clientSettings().getEditorSettings();
}


void App::saveEditorSettings(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    ofLogNotice("App::saveEditorSettings") << "Saving editor settings.";

    Json::Value settings = args.params["data"]; // must make a copy

    _settings.clientSettings().setEditorSettings(settings);

    // broadcast new editor settings to all connected clients
    Json::Value params;
    params["data"] = settings;
    params["clientUUID"] = args.params["clientUUID"];
    ofLogNotice("App::saveEditorSettings") << "clientUUID: " << params["clientUUID"] << endl;
    Json::Value json = SketchUtils::toJSONMethod("Server", "updateEditorSettings", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
}

void App::loadOfSketchSettings(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
	ofLogNotice("App::loadOfSketchSettings") << "";

    args.result = Serializer::toJSON(_settings.clientSettings());
}

void App::saveOfSketchSettings(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    ofLogNotice("App::saveOfSketchSettings") << "Saving ofSketch settings.";

    Json::Value settings = args.params["data"]; // must make a copy

// \todo
//    _ofSketchSettings.update(settings);
//    _ofSketchSettings.save();

    // broadcast new editor settings to all connected clients
    Json::Value params;
    params["data"] = settings;
    params["clientUUID"] = args.params["clientUUID"];

    Json::Value json = SketchUtils::toJSONMethod("Server", "updateOfSketchSettings", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
}


void App::exportProject(const void *pSender, ofx::JSONRPC::MethodArgs &args) 
{
	ofLogNotice("App::exportProject") << "";

    std::string platform = args.params["platform"].asString();
    std::string projectName = args.params["projectName"].asString();
//    Project& project = _projectManager.getProjectRef(projectName);
}

bool App::onWebSocketOpenEvent(ofx::HTTP::WebSocketOpenEventArgs& args)
{
    ofLogNotice("App::onWebSocketOpenEvent") << "Connection opened from: " << args.getConnection().getClientAddress().toString();

    // Here, we need to send all initial values, settings, etc to the
    // client before any other messages arrive.

//    Json::Value params = _taskQueue.toJson();
//    cout << "Params: " << params.toStyledString() << endl;
//    Json::Value json = Utils::toJSONMethod("TaskQueue", "taskList", params);
//    cout << "Json: " << json.toStyledString() << endl;
//    ofx::HTTP::WebSocketFrame frame(Utils::toJSONString(json));
//
//    // Send the update to the client that just connected.
//    args.getConnectionRef().sendFrame(frame);

    // Send version info.
    // params.clear();

    const ofx::HTTP::WebSocketConnection& webSocket = args.getConnection();

    webSocket.sendFrame(SketchUtils::makeFrame(SketchUtils::toJSONMethod("Server",
                                                                         "systemInfo",
                                                                         SketchUtils::systemInfo())));

    if (_missingDependencies)
    {
        webSocket.sendFrame(SketchUtils::makeFrame(SketchUtils::toJSONMethod("Server",
                                                                             "missingDependencies",
                                                                             Json::nullValue)));
    }

    return false; // did not handle it
}


bool App::onWebSocketCloseEvent(ofx::HTTP::WebSocketCloseEventArgs& args)
{
    std::stringstream ss;

    ss << "Connection closed from: " << args.getConnection().getClientAddress().toString() << " ";
    ss << "Code: " << args.getCode() << " Reason: " << args.getReason();

    ofLogVerbose("App::onWebSocketCloseEvent") << ss.str();

    return false; // did not handle it
}


bool App::onWebSocketFrameReceivedEvent(ofx::HTTP::WebSocketFrameEventArgs& args)
{
    ofLogVerbose("App::onWebSocketFrameReceivedEvent") << "Frame received from: " << args.getConnection().getClientAddress().toString();
    return false; // did not handle it
}


bool App::onWebSocketFrameSentEvent(ofx::HTTP::WebSocketFrameEventArgs& args)
{
    ofLogVerbose("App::onWebSocketFrameSentEvent") << "Frame sent to: " << args.getConnection().getClientAddress().toString();
    return false; // did not handle it
}


bool App::onWebSocketErrorEvent(ofx::HTTP::WebSocketErrorEventArgs& args)
{
    ofLogError("App::onWebSocketErrorEvent") << "Stop: " << args.getError();
    return false; // did not handle it
}


void App::onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogVerbose("ofApp::onServerVerificationError") << args.errorMessage();

    // if you want to proceed, you should allow your user to inspect
    // the certificate and set:
    //     args.setIgnoreError(true);
    // if they approve
}


void App::onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogVerbose("ofApp::onClientVerificationError") << args.errorMessage();

    std::stringstream ss;

    ss << "Error: " << args.errorMessage() << " #" << args.errorNumber() << " depth: " << args.errorDepth() << std::endl;

    ss << "Certificate: " << std::endl;

    ss << "Issued By: " << args.certificate().issuerName() << std::endl;
    ss << "Subject Name: " << args.certificate().issuerName() << std::endl;
    ss << "Common Name: " << args.certificate().issuerName() << std::endl;

    Poco::DateTime ldtValidFrom = args.certificate().validFrom();
    Poco::DateTime ldtExpiresOn = args.certificate().expiresOn();

    ss << "Valid From: " << Poco::DateTimeFormatter::format(ldtValidFrom, "%dd %H:%M:%S.%i") << std::endl;
    ss << "Expires On: " << Poco::DateTimeFormatter::format(ldtExpiresOn, "%dd %H:%M:%S.%i") << std::endl;

    ofLogVerbose("ofApp::onServerVerificationError") << ss.str();

    // if you want to proceed, you should allow your user to inspect
    // the certificate and set:
    //     args.setIgnoreError(true);
    // if they approve
}


void App::onSSLPrivateKeyPassphraseRequired(std::string& args)
{
    ofLogVerbose("ofApp::onPrivateKeyPassphraseRequired") << "Password required.";

    // if you want to proceed, you should allow your user set the
    // the certificate and set:
    args = "password";
}


bool App::onTaskQueued(const ofx::TaskQueueEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskQueued", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
    return false;
}


bool App::onTaskStarted(const ofx::TaskQueueEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskStarted", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
    return false;
}


bool App::onTaskCancelled(const ofx::TaskQueueEventArgs& args)
{
	Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskCancelled", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
    return false;
}


bool App::onTaskFinished(const ofx::TaskQueueEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskFinished", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
    return false;
}


bool App::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId();
    params["exception"] = args.getException().displayText();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskFailed", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
    return false;
}


bool App::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId();
    params["progress"] = args.getProgress();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskProgress", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
    return false;
}


bool App::onTaskData(const ProcessTaskQueue::EventArgs& args)
{
    // We use the custom events to send status messages, and other custom
    // events for custom data specific events.
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId();
    params["message"] = args.getData();

//    Json::Value error = _compiler.parseError(args.getData());

//    if (!error.empty())
//    {
//        params["compileError"] = error;
//    }
//
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskMessage", params);
    ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));
    _server->getWebSocketRoute().broadcast(frame);
	return false;
}


} } // namespace of::Sketch
