// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
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


namespace of {
namespace Sketch {


const std::string ofApp::VERSION_SPECIAL = "RC1";


ofApp::ofApp():
    _editorSettings("Resources/Settings/EditorSettings.json"),
    _ofSketchSettings(),
    _threadPool("ofSketchThreadPool"),
    _taskQueue(ofx::TaskQueue_<std::string>::UNLIMITED_TASKS, _threadPool),
    _compiler(_taskQueue, "Resources/Templates/CompilerTemplates", "openFrameworks"),
    _addonManager("openFrameworks/addons"),
    _projectManager("Projects"),
    _uploadRouter("Projects"),
    _missingDependencies(true)
{
}


ofApp::~ofApp()
{
    _taskQueue.unregisterAllEvents(this);

    server->getWebSocketRoute()->unregisterWebSocketEvents(this);
    server->getPostRoute()->unregisterPostEvents(&_uploadRouter);

    ofSSLManager::unregisterAllEvents(this);
}


void ofApp::setup()
{
    ofSetFrameRate(30);

    ofSetLogLevel("ofThread", OF_LOG_ERROR);
    ofSetLogLevel(OF_LOG_NOTICE);

    // Hack to make sure that the net sybsystem is initialized on windows.
    Poco::Net::initializeNetwork();

#ifdef TARGET_WIN32
	// Set up toolchain path information for Windows.
	std::string pathVar = Poco::Environment::get("PATH","");

	std::string pathToolChain0 = ofToDataPath("Toolchains/ofMinGW/MinGW/msys/1.0/bin", true);
	std::string pathToolChain1 = ofToDataPath("Toolchains/ofMinGW/MinGW/bin", true);

	std::stringstream pathSS;
	pathSS << pathToolChain0 << ";" << pathToolChain1 << ";" << pathVar;
	Poco::Environment::set("PATH", pathSS.str());
#endif

    _editorSettings.load();
    _ofSketchSettings.load();
    _compiler.setup();
    _addonManager.setup();
    _projectManager.setup();
    _uploadRouter.setup();

    try 
	{
        if (hasDependency("make"))
        {
            _missingDependencies = false;
        }

        ofLogNotice("ofApp::App") << "Editor setting's projectDir: " << _ofSketchSettings.getProjectDir();

        _taskQueue.registerAllEvents(this);

        ofLogNotice("ofApp::App") << "Starting server on port: " << _ofSketchSettings.getPort() << " With Websocket Buffer Size: " << _ofSketchSettings.getBufferSize();

        ofx::HTTP::BasicJSONRPCServerSettings settings; // TODO: load from file.
        settings.setBufferSize(_ofSketchSettings.getBufferSize());
        settings.setPort(_ofSketchSettings.getPort());
        settings.setUploadRedirect("");

        unsigned long long maximumFileUploadSize = 5120000000; // 50 GB

        settings.setMaximumFileUploadSize(maximumFileUploadSize);

        ofx::Net::IPAddressRange::IPAddressRange::List whitelist;
        whitelist.push_back(ofx::Net::IPAddressRange("127.0.0.1/32"));
        settings.setWhitelist(whitelist);

        server = ofx::HTTP::BasicJSONRPCServer::makeShared(settings);

        // Must register for all events before initializing server.
        ofSSLManager::registerAllEvents(this);

        server->getPostRoute()->registerPostEvents(&_uploadRouter);
        server->getWebSocketRoute()->registerWebSocketEvents(this);

        // Set up websocket logger.
        // _loggerChannel = WebSocketLoggerChannel::makeShared();
        // _loggerChannel->setWebSocketRoute(server->getWebSocketRoute());
        // ofSetLoggerChannel(_loggerChannel);

        _logo.loadImage("media/openFrameworks.jpg");
        _font.loadFont(OF_TTF_SANS, 20);

		ofSSLManager::initializeServer(new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
															  ofToDataPath("ssl/privateKey.nopassword.pem"),
															  ofToDataPath("ssl/selfSignedCertificate.nopassword.pem"),
															  ofToDataPath("ssl/cacert.pem")));

		// TODO: configure these via settings files
		server->registerMethod("load-project",
							   "Load the requested project.",
							   this,
							   &ofApp::loadProject);

		server->registerMethod("load-template-project",
							   "Load an anonymous project.",
							   this,
							   &ofApp::loadTemplateProject);

		server->registerMethod("save-project",
							   "Save the current project.",
							   this,
							   &ofApp::saveProject);

		server->registerMethod("create-project",
							   "Create a new project.",
							   this,
							   &ofApp::createProject);

		server->registerMethod("delete-project",
							   "Delete the current project.",
							   this,
							   &ofApp::deleteProject);

		server->registerMethod("rename-project",
							   "Rename the current project.",
							   this,
							   &ofApp::renameProject);

		server->registerMethod("notify-project-closed",
							   "Notify the server that project was closed.",
							   this,
							   &ofApp::notifyProjectClosed);

		server->registerMethod("request-project-closed",
							   "Broadcast a project close request to connected clients.",
							   this,
							   &ofApp::requestProjectClosed);

		server->registerMethod("request-app-quit",
							   "Quit the app.",
							   this,
							   &ofApp::requestAppQuit);

		server->registerMethod("create-class",
							   "Create a new class for the current project.",
							   this,
							   &ofApp::createClass);

		server->registerMethod("delete-class",
							   "Delete a select class from for the current project.",
							   this,
							   &ofApp::deleteClass);

		server->registerMethod("rename-class",
							   "Rename a select class from for the current project.",
							   this,
							   &ofApp::renameClass);

		server->registerMethod("run-project",
							   "Run the requested project.",
							   this,
							   &ofApp::runProject);

		server->registerMethod("compile-project",
							   "Run the requested project.",
							   this,
							   &ofApp::compileProject);

		server->registerMethod("stop",
							   "Stop the requested project.",
							   this,
							   &ofApp::stop);

		server->registerMethod("get-project-list",
							   "Get list of all projects in the Project directory.",
							   this,
							   &ofApp::getProjectList);

		server->registerMethod("load-editor-settings",
							   "Get the editor settings.",
							   this,
							   &ofApp::loadEditorSettings);

		server->registerMethod("save-editor-settings",
							   "Save the editor settings.",
							   this,
							   &ofApp::saveEditorSettings);

		server->registerMethod("load-ofsketch-settings",
							   "Get ofSketch settings.",
							   this,
							   &ofApp::loadOfSketchSettings);

		server->registerMethod("save-ofsketch-settings",
							   "Save ofSketch settings.",
							   this,
							   &ofApp::saveOfSketchSettings);

		server->registerMethod("get-addon-list",
							   "Get a list of all addons.",
							   this,
							   &ofApp::getAddonList);

		server->registerMethod("get-project-addon-list",
							   "Get a list of addons for a project.",
							   this,
							   &ofApp::getProjectAddonList);

		server->registerMethod("add-project-addon",
							   "Add an addon to a project.",
							   this,
							   &ofApp::addProjectAddon);

		server->registerMethod("remove-project-addon",
							   "Remove an addon from a project.",
							   this,
							   &ofApp::removeProjectAddon);

		server->registerMethod("export-project",
							   "Export the project for target platform.",
							   this,
							   &ofApp::exportProject);

		server->start();

		ofTargetPlatform arch = SketchUtils::getTargetPlatform();

		if (arch != OF_TARGET_LINUXARMV6L && arch != OF_TARGET_LINUXARMV7L)
		{
			// Launch a browser with the address of the server.
			ofLaunchBrowser(server->getURL() + "?project=HelloWorld");
		}
    }
    catch (const Poco::Exception& exc)
    {
		ofLogError() << exc.displayText();
    }
}


void ofApp::update()
{
}


void ofApp::draw()
{
    ofBackground(255);

    ofSetColor(255);
    _logo.draw(10, 0);

    ofSetColor(80);
    _font.drawString("Launch", 70, 30);
}

void ofApp::exit()
{
    // broadcast requestProjectClosed settings to all connected clients
    Json::Value params;
    params["foo"] = "bar";
    Json::Value json = SketchUtils::toJSONMethod("Server", "appExit", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
    ofLogNotice("ofApp::exit") << "appExit frame broadcasted" << endl;

    // Reset default logger.
    ofLogToConsole();
}


void ofApp::mousePressed(int x, int y, int button)
{
    ofLaunchBrowser(server->getURL());
}


bool ofApp::hasDependency(const std::string& command)
{
	try 
	{
		std::string cmd("which");
		std::vector<std::string> args;
		args.push_back(command);
		Poco::Pipe outPipe;
		Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, 0, &outPipe, 0);
		Poco::PipeInputStream istr(outPipe);
		std::stringstream ostr;
		Poco::StreamCopier::copyStream(istr, ostr);
		std::string result = ostr.str();
		return !result.empty();
	} 
	catch (const Poco::Exception& exc)
	{
		// This probably happened because the which program was not available on Windows.
		ofLogError() << exc.displayText();
		return true;
	}
}


void ofApp::loadProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    if (args.params.isMember("projectName"))
    {
        std::string projectName = args.params["projectName"].asString();

        if (_projectManager.projectExists(projectName))
        {
            _projectManager.loadProject(pSender, args);
        }
        else args.error["message"] = "The requested project does not exist.";
    }
    else args.error["message"] = "Incorrect parameters sent to load-project method.";
}


void ofApp::loadTemplateProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    _projectManager.loadTemplateProject(pSender, args);
}


void ofApp::saveProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectData"]["projectFile"]["name"].asString();

    if (_projectManager.projectExists(projectName))
    {
        _projectManager.saveProject(pSender, args);
        const Project& project = _projectManager.getProject(projectName);
        _compiler.generateSourceFiles(project);
    }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::createProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    if (!_projectManager.projectExists(projectName))
    {
        _projectManager.createProject(pSender, args);
    }
    else args.error["message"] = "That project name already exists.";
}


void ofApp::deleteProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    if (_projectManager.projectExists(projectName))
    {
        _projectManager.deleteProject(pSender, args);
        requestProjectClosed(pSender, args);
    }
    else args.error["message"] = "The project that you are trying to delete does not exist.";
}


void ofApp::renameProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    if (_projectManager.projectExists(projectName))
    {
        _projectManager.renameProject(pSender, args);
        requestProjectClosed(pSender, args);
    }
    else args.error["message"] = "The project that you are trying to delete does not exist.";
}


void ofApp::notifyProjectClosed(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    _projectManager.notifyProjectClosed(projectName);
    ofLogNotice("ofApp::notifyProjectClosed") << projectName << " closed.";
}


void ofApp::requestProjectClosed(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    std::string clientUUID = args.params["clientUUID"].asString();

    // broadcast requestProjectClosed settings to all connected clients
    Json::Value params;
    params["projectName"] = projectName;
    params["clientUUID"] = clientUUID;
    Json::Value json = SketchUtils::toJSONMethod("Server", "requestProjectClosed", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
}


void ofApp::requestAppQuit(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
//    args.result = "App quit";
//    ofExit();
}


void ofApp::createClass(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();

    if (_projectManager.projectExists(projectName))
    {
        std::string className = args.params["className"].asString();
        Project& project = _projectManager.getProjectRef(projectName);
        args.result["classFile"] = project.createClass(className);

    }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::deleteClass(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    if (_projectManager.projectExists(projectName))
    {
        std::string className = args.params["className"].asString();
        Project& project = _projectManager.getProjectRef(projectName);
        if (project.deleteClass(className))
        {
            args.result["message"] = className + "class deleted.";
        }
        else args.error["message"] = "Error deleting the class.";
    }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::renameClass(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    if (_projectManager.projectExists(projectName))
    {
        std::string className = args.params["className"].asString();
        std::string newClassName = args.params["newClassName"].asString();
        Project& project = _projectManager.getProjectRef(projectName);
        if (project.renameClass(className, newClassName))
        {
            args.result["message"] = className + " class renamed to " + newClassName;
        }
        else args.error["message"] = "Error renaming " + className + " class.";
    }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::runProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    if (_projectManager.projectExists(projectName))
    {
        ofLogNotice("ofApp::run") << "Running " << projectName << " project";
        const Project& project = _projectManager.getProject(projectName);
        Poco::UUID taskId = _compiler.run(project);
        ofLogNotice("APP::run") << "Task ID: " << taskId.toString();
        args.result = taskId.toString();
    }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::compileProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    if (_projectManager.projectExists(projectName))
    {
        ofLogNotice("ofApp::compileProject") << "Compiling " << projectName << " project";
        const Project& project = _projectManager.getProject(projectName);
        Poco::UUID taskId = _compiler.compile(project);
        ofLogNotice("ofApp::compileProject") << "Task ID: " << taskId.toString();
        args.result = taskId.toString();
    }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::stop(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    if (args.params.isMember("taskId"))
    {
        Poco::UUID taskId(args.params["taskId"].asString());

        try
        {
            _taskQueue.cancel(taskId);
        }
        catch (const Poco::NotFoundException& exc)
        {
            ofLogWarning("ofApp::stop") << "Task already stopped: " << taskId.toString();
        }

        args.result = taskId.toString();

        ofLogNotice("ofApp::stop") << "Stopped task " << taskId.toString();
    }
    else
    {
        // Invalid.
        args.error = "No task id.";
    }
}


void ofApp::getProjectList(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    _projectManager.getProjectList(pSender, args);
}


void ofApp::getAddonList(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    ofLogVerbose("ofApp::getAddonList") << " sending addon list.";

    Json::Value addonsJSON;
    std::vector<Addon::SharedPtr> addons = _addonManager.getAddons();
    std::vector<Addon::SharedPtr>::const_iterator iter = addons.begin();

    while (iter != addons.end())
    {
        Json::Value addon;
        addon["name"] = (*iter)->getName();
        addon["path"] = (*iter)->getPath().toString();
        addonsJSON.append(addon);
        ++iter;
    }

    args.result = addonsJSON;
}


void ofApp::getProjectAddonList(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    std::string projectName = args.params["projectName"].asString();

    if (_projectManager.projectExists(projectName))
    {
        const Project& project = _projectManager.getProject(projectName);

        if (project.hasAddons())
        {
            std::vector<std::string> addons = project.getAddons();

            for (unsigned int i = 0; i < addons.size(); ++i)
            {
                args.result["addons"][i] = addons[i];
            }

			args.result["hasAddons"] = true;
        }
        else
        {
            args.result["hasAddons"] = false;
        }

        }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::addProjectAddon(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    std::string addon = args.params["addon"].asString();

    if (_projectManager.projectExists(projectName))
    {
        Project& project = _projectManager.getProjectRef(projectName);
        project.addAddon(addon);
    }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::removeProjectAddon(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    std::string projectName = args.params["projectName"].asString();
    std::string addon = args.params["addon"].asString();

    if (_projectManager.projectExists(projectName))
    {
        Project& project = _projectManager.getProjectRef(projectName);
        project.removeAddon(addon);
    }
    else args.error["message"] = "The requested project does not exist.";
}


void ofApp::loadEditorSettings(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    args.result = _editorSettings.getData();
}


void ofApp::saveEditorSettings(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    ofLogVerbose("ofApp::saveEditorSettings") << "Saving editor settings" << endl;
    Json::Value settings = args.params["data"]; // must make a copy
    _editorSettings.update(settings);
    _editorSettings.save();

    // broadcast new editor settings to all connected clients
    Json::Value params;
    params["data"] = settings;
    params["clientUUID"] = args.params["clientUUID"];
    ofLogNotice("ofApp::saveEditorSettings") << "clientUUID: " << params["clientUUID"] << endl;
    Json::Value json = SketchUtils::toJSONMethod("Server", "updateEditorSettings", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
}

void ofApp::loadOfSketchSettings(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    args.result = _ofSketchSettings.getData();
}

void ofApp::saveOfSketchSettings(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    ofLogVerbose("ofApp::saveOfSketchSettings") << "Saving ofSketch settings" << endl;

    Json::Value settings = args.params["data"]; // must make a copy

    _ofSketchSettings.update(settings);
    _ofSketchSettings.save();

    // broadcast new editor settings to all connected clients
    Json::Value params;
    params["data"] = settings;
    params["clientUUID"] = args.params["clientUUID"];

    Json::Value json = SketchUtils::toJSONMethod("Server", "updateOfSketchSettings", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
}

void ofApp::exportProject(const void *pSender, ofx::JSONRPC::MethodArgs &args) 
{
    std::string platform = args.params["platform"].asString();
    std::string projectName = args.params["projectName"].asString();
//    Project& project = _projectManager.getProjectRef(projectName);
}

bool ofApp::onWebSocketOpenEvent(ofx::HTTP::WebSocketOpenEventArgs& args)
{
    ofLogVerbose("ofApp::onWebSocketOpenEvent") << "Connection opened from: " << args.getConnectionRef().getClientAddress().toString();

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
    Json::Value params;
    params["version"] = getVersion();
    params["major"] = getVersionMajor();
    params["minor"] = getVersionMinor();
    params["patch"] = getVersionPatch();
    params["special"] = getVersionSpecial();
    params["target"] = SketchUtils::toString(SketchUtils::getTargetPlatform());

    Json::Value host;
    Json::Value os;
    Json::Value node;

    os["architecture"] = Poco::Environment::osArchitecture();
    os["display_name"] = Poco::Environment::osDisplayName();
    os["name"] = Poco::Environment::osName();
    os["version"] = Poco::Environment::osVersion();

    node["name"] = Poco::Environment::nodeName();
    node["id"] = Poco::Environment::nodeId();

    host["processor_count"] = Poco::Environment::processorCount();
    host["node"] = node;
    host["os"] = os;

    params["host"] = host;

    Json::Value json = SketchUtils::toJSONMethod("Server", "version", params);
    ofx::HTTP::WebSocketFrame frame = ofx::HTTP::WebSocketFrame(SketchUtils::toJSONString(json));

    args.getConnectionRef().sendFrame(frame);

    if (_missingDependencies)
    {
        params = Json::nullValue;
        json = SketchUtils::toJSONMethod("Server", "missingDependencies", params);
        frame = ofx::HTTP::WebSocketFrame(SketchUtils::toJSONString(json));
        args.getConnectionRef().sendFrame(frame);
    }

    return false; // did not handle it
}


bool ofApp::onWebSocketCloseEvent(ofx::HTTP::WebSocketCloseEventArgs& args)
{
    std::stringstream ss;

    ss << "Connection closed from: " << args.getConnectionRef().getClientAddress().toString() << " ";
    ss << "Code: " << args.getCode() << " Reason: " << args.getReason();

    ofLogVerbose("ofApp::onWebSocketCloseEvent") << ss.str();

    return false; // did not handle it
}


bool ofApp::onWebSocketFrameReceivedEvent(ofx::HTTP::WebSocketFrameEventArgs& args)
{
    ofLogVerbose("ofApp::onWebSocketFrameReceivedEvent") << "Frame received from: " << args.getConnectionRef().getClientAddress().toString();
    return false; // did not handle it
}


bool ofApp::onWebSocketFrameSentEvent(ofx::HTTP::WebSocketFrameEventArgs& args)
{
    ofLogVerbose("ofApp::onWebSocketFrameSentEvent") << "Frame sent to: " << args.getConnectionRef().getClientAddress().toString();
    return false; // did not handle it
}


bool ofApp::onWebSocketErrorEvent(ofx::HTTP::WebSocketErrorEventArgs& args)
{
    ofLogError("ofApp::onWebSocketErrorEvent") << "Stop: " << args.getError();
    return false; // did not handle it
}


void ofApp::onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogVerbose("ofofApp::onServerVerificationError") << args.errorMessage();

    // if you want to proceed, you should allow your user to inspect
    // the certificate and set:
    //     args.setIgnoreError(true);
    // if they approve
}


void ofApp::onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args)
{
    ofLogVerbose("ofofApp::onClientVerificationError") << args.errorMessage();

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

    ofLogVerbose("ofofApp::onServerVerificationError") << ss.str();

    // if you want to proceed, you should allow your user to inspect
    // the certificate and set:
    //     args.setIgnoreError(true);
    // if they approve
}


void ofApp::onSSLPrivateKeyPassphraseRequired(std::string& args)
{
    ofLogVerbose("ofofApp::onPrivateKeyPassphraseRequired") << args;

    // if you want to proceed, you should allow your user set the
    // the certificate and set:
    args = "password";
}


bool ofApp::onTaskQueued(const ofx::TaskQueueEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId().toString();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskQueued", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
    return false;
}


bool ofApp::onTaskStarted(const ofx::TaskQueueEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId().toString();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskStarted", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
    return false;
}


bool ofApp::onTaskCancelled(const ofx::TaskQueueEventArgs& args)
{
	Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId().toString();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskCancelled", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
    return false;
}


bool ofApp::onTaskFinished(const ofx::TaskQueueEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId().toString();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskFinished", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
    return false;
}


bool ofApp::onTaskFailed(const ofx::TaskFailedEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId().toString();
    params["exception"] = args.getException().displayText();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskFailed", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
    return false;
}


bool ofApp::onTaskProgress(const ofx::TaskProgressEventArgs& args)
{
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId().toString();
    params["progress"] = args.getProgress();
    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskProgress", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
    return false;
}


bool ofApp::onTaskData(const ProcessTaskQueue::EventArgs& args)
{
    // We use the custom events to send status messages, and other custom
    // events for custom data specific events.
    Json::Value params;
    params["name"] = args.getTaskName();
    params["uuid"] = args.getTaskId().toString();
    params["message"] = args.getData();

    Json::Value error = _compiler.parseError(args.getData());

    if (!error.empty())
    {
        params["compileError"] = error;
    }

    Json::Value json = SketchUtils::toJSONMethod("TaskQueue", "taskMessage", params);
    ofx::HTTP::WebSocketFrame frame(SketchUtils::toJSONString(json));
    server->getWebSocketRoute()->broadcast(frame);
	return false;
}


std::string ofApp::getVersion()
{
    std::stringstream ss;

    ss << VERSION_MAJOR << ".";
    ss << VERSION_MINOR << ".";
    ss << VERSION_PATCH;

    if (!VERSION_SPECIAL.empty())
    {
        ss << "-" << VERSION_SPECIAL;
    }

    return ss.str();
}

int ofApp::getVersionMajor()
{
    return VERSION_MAJOR;
}


int ofApp::getVersionMinor()
{
    return VERSION_MINOR;
}


int ofApp::getVersionPatch()
{
    return VERSION_PATCH;
}


std::string ofApp::getVersionSpecial()
{
    return VERSION_SPECIAL;
}


} } // namespace of::Sketch
