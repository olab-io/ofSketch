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


App::App(): _currentConnection(0)
{
    // Must register for all events before initializing server.
    ofSSLManager::registerAllEvents(this);

    server.getPostRoute()->registerPostEvents(this);
    server.getWebSocketRoute()->registerWebSocketEvents(this);
}


App::~App()
{
    server.getWebSocketRoute()->unregisterWebSocketEvents(this);
    server.getPostRoute()->unregisterPostEvents(this);

    ofSSLManager::unregisterAllEvents(this);
}


void App::setup()
{
    ofSetFrameRate(30);

    ofSetLogLevel(OF_LOG_VERBOSE);

    ofSSLManager::initializeServer(new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
                                                          ofToDataPath("ssl/privateKey.nopassword.pem"),
                                                          ofToDataPath("ssl/selfSignedCertificate.nopassword.pem"),
                                                          ofToDataPath("ssl/cacert.pem")));




    // TODO: configure these via settings files
    _projectManager = ProjectManager::makeShared(ofToDataPath("Projects"));
    _addonManager = AddonManager::makeShared(ofToDataPath("openFrameworks/addons"));

    HTTP::BasicJSONRPCServerSettings settings; // TODO: load from file.

    // settings.setUseSSL(true);

    server.setup(settings);

    server.registerMethod("load",
                          "Load the requested project.",
                          this,
                          &App::load);

    server.registerMethod("play",
                          "Play the requested project.",
                          this,
                          &App::play);

    server.registerMethod("stop",
                          "Stop the requested project.",
                          this,
                          &App::stop);

    // start the server
    server.start();

    // Launch a browser with the address of the server.
    ofLaunchBrowser(server.getURL());
}

void App::update()
{
}


void App::draw()
{
    ofBackground(0);
}


void App::load(const void* pSender, JSONRPC::MethodArgs& args)
{
    ofBuffer project = ofBufferFromFile("Projects/HelloWorld/src/main.cpp");
    args.result["source"] = project.getText();
}


void App::play(const void* pSender, JSONRPC::MethodArgs& args)
{
    if (args.params.isMember("source"))
    {
        ofBuffer sourceBuffer(args.params["source"].asString());
        ofBufferToFile("Projects/HelloWorld/src/main.cpp", sourceBuffer);

        std::string cmd("make");

        std::vector<std::string> args;

        args.push_back("--directory=" + ofToDataPath("Projects/HelloWorld/", true));
//        if(_settings.numProcessors > 1)
//        {
//            args.push_back("-j" + ofToString(_settings.numProcessors));
//        }

//        if(_settings.isSilent)
//        {
//            args.push_back("-s");
//        }

//        args.push_back(_target);

        args.push_back("OF_ROOT=" + ofToDataPath("openFrameworks/", true));

        Poco::Pipe inPipe; // this needs to be passed in
        Poco::Pipe outAndErrPipe;
        //    Poco::Pipe errPipe;

        Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, &inPipe, &outAndErrPipe, &outAndErrPipe);

        Poco::PipeInputStream istr(outAndErrPipe);

//        const std::size_t bufferSize = 8192;
//        char buffer[bufferSize];
//
//        while(istr.good() && !istr.fail())
//        {
//            if(isCancelled())
//            {
//                Poco::Process::kill(ph);
//            }
//            
//            istr.getline(buffer,bufferSize);
//            cout << "LINE>>" << buffer << "<<LINE" << endl;
//        }

        Poco::StreamCopier::copyStream(istr, std::cout);

        
        int exitCode = ph.wait();

        cout << "exit code: " << exitCode << endl;

        args.push_back("run");

        ph = Poco::Process::launch(cmd, args, &inPipe, &outAndErrPipe, &outAndErrPipe);

        Poco::PipeInputStream istr2(outAndErrPipe);
        Poco::StreamCopier::copyStream(istr2, std::cout);

        exitCode = ph.wait();

        cout << "exit code: " << exitCode << endl;

    }
    else
    {
        // Pass error in this case.
    }
}


void App::stop(const void* pSender, JSONRPC::MethodArgs& args)
{
    std::cout << "stop: " << std::endl;
    std::cout << args.params.toStyledString() << std::endl;
}


bool App::onWebSocketOpenEvent(HTTP::WebSocketEventArgs& args)
{
//    ofLogVerbose("App::onWebSocketOpenEvent") << "Connection opened from: " << args.getConnectionRef().getClientAddress().toString();
    return false; // did not handle it
}


bool App::onWebSocketCloseEvent(HTTP::WebSocketEventArgs& args)
{
//    ofLogVerbose("App::onWebSocketCloseEvent") << "Connection closed from: " << args.getConnectionRef().getClientAddress().toString();
    return false; // did not handle it
}


bool App::onWebSocketFrameReceivedEvent(HTTP::WebSocketFrameEventArgs& args)
{
//    ofLogVerbose("App::onWebSocketFrameReceivedEvent") << "Frame received from: " << args.getConnectionRef().getClientAddress().toString();
    return false; // did not handle it
}


bool App::onWebSocketFrameSentEvent(HTTP::WebSocketFrameEventArgs& args)
{
//    ofLogVerbose("App::onWebSocketFrameSentEvent") << "Frame sent to: " << args.getConnectionRef().getClientAddress().toString();
    return false; // did not handle it
}


bool App::onWebSocketErrorEvent(HTTP::WebSocketEventArgs& args)
{
//    ofLogVerbose("App::onWebSocketErrorEvent") << "Error on: " << args.getConnectionRef().getClientAddress().toString();
    return false; // did not handle it
}


bool App::onHTTPPostEvent(HTTP::PostEventArgs& args)
{
//    ofLogNotice("ofApp::onHTTPPostEvent") << "Data: " << args.getBuffer().getText();
    return false;
}


bool App::onHTTPFormEvent(HTTP::PostFormEventArgs& args)
{
//    ofLogNotice("ofApp::onHTTPFormEvent") << "";
//    HTTP::Utils::dumpNameValueCollection(args.getForm(), ofGetLogLevel());
    return false;
}


bool App::onHTTPUploadEvent(HTTP::PostUploadEventArgs& args)
{
//    std::string stateString = "";
//
//    switch (args.getState())
//    {
//        case HTTP::PostUploadEventArgs::UPLOAD_STARTING:
//            stateString = "STARTING";
//            break;
//        case HTTP::PostUploadEventArgs::UPLOAD_PROGRESS:
//            stateString = "PROGRESS";
//            break;
//        case HTTP::PostUploadEventArgs::UPLOAD_FINISHED:
//            stateString = "FINISHED";
//            break;
//    }
//
//    ofLogNotice("ofApp::onHTTPUploadEvent") << "";
//    ofLogNotice("ofApp::onHTTPUploadEvent") << "         state: " << stateString;
//    ofLogNotice("ofApp::onHTTPUploadEvent") << " formFieldName: " << args.getFormFieldName();
//    ofLogNotice("ofApp::onHTTPUploadEvent") << "orig. filename: " << args.getOriginalFilename();
//    ofLogNotice("ofApp::onHTTPUploadEvent") <<  "      filename: " << args.getFilename();
//    ofLogNotice("ofApp::onHTTPUploadEvent") <<  "      fileType: " << args.getFileType().toString();
//    ofLogNotice("ofApp::onHTTPUploadEvent") << "# bytes xfer'd: " << args.getNumBytesTransferred();
    return false;
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
    ofLogVerbose("ofApp::onPrivateKeyPassphraseRequired") << args;
    
    // if you want to proceed, you should allow your user set the
    // the certificate and set:
    args = "password";
}


} } // namespace of::Sketch
