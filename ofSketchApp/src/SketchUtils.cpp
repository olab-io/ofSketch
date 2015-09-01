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


#include "SketchUtils.h"
#include "Poco/Environment.h"
#include "Poco/Process.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include "ofxIO.h"
#include "Version.h"
#include "Serializer.h"


namespace of {
namespace Sketch {


Json::Value SketchUtils::systemInfo()
{
    Json::Value json;

    json["version"]["major"] = Version::major();
    json["version"]["minor"] = Version::minor();
    json["version"]["patch"] = Version::patch();
    json["version"]["special"] = Version::special();
    json["version"]["version"] = Version::asString();

    json["os"]["architecture"] = Poco::Environment::osArchitecture();
    json["os"]["displayName"] = Poco::Environment::osDisplayName();
    json["os"]["name"] = Poco::Environment::osName();
    json["os"]["version"] = Poco::Environment::osVersion();

    json["environment"]["node"]["name"] = Poco::Environment::nodeName();

    try
    {
        json["environment"]["node"]["id"] = Poco::Environment::nodeId();
    }
    catch (const Poco::SystemException& exc)
    {
        ofLogWarning("SketchUtils::hostInfo") << exc.displayText();
    }

    json["environment"]["processorCount"] = Poco::Environment::processorCount();

    return json;
}

ofx::HTTP::WebSocketFrame SketchUtils::makeFrame(const Json::Value& json)
{
    return ofx::HTTP::WebSocketFrame(Serializer::toString(json));
}


Json::Value SketchUtils::toJSONMethod(const std::string& module,
                                      const std::string& method,
                                      const Json::Value& params)
{
    Json::Value json;
    json["ofSketch"] = "1.0";
    json["module"] = module;
    json["method"] = method;
    json["params"] = params;
    return json;
}


ofTargetPlatform SketchUtils::getTargetPlatform()
{
#ifdef TARGET_LINUX
    std::string cmd("uname");
    std::vector<std::string> args;
    args.push_back("-m");
    Poco::Pipe outPipe;
    Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, 0, &outPipe, 0);
    Poco::PipeInputStream istr(outPipe);

    std::stringstream ostr;

    Poco::StreamCopier::copyStream(istr, ostr);

    std::string arch = ostr.str();

    if(ofIsStringInString(arch,"x86_64"))
    {
        return OF_TARGET_LINUX64;
    }
    else if(ofIsStringInString(arch,"armv6l"))
    {
        return OF_TARGET_LINUXARMV6L;
    }
    else if(ofIsStringInString(arch,"armv7l"))
    {
        return OF_TARGET_LINUXARMV7L;
    }
    else
    {
        return OF_TARGET_LINUX;
    }

#elif defined(TARGET_OSX)
        return OF_TARGET_OSX;
#elif defined(TARGET_WIN32)
#if (_MSC_VER)
        return OF_TARGET_WINVS;
#else
        return OF_TARGET_WINGCC;
#endif
#elif defined(TARGET_ANDROID)
        return OF_TARGET_ANDROID;
#elif defined(TARGET_OF_IOS)
        return OF_TARGET_IOS;
#elif defined(TARGET_EMSCRIPTEN)
        return OF_TARGET_EMSCRIPTEN;
#endif
}


std::string SketchUtils::TargetPlatformToString(ofTargetPlatform targetPlatform)
{
    switch (targetPlatform)
    {
        case OF_TARGET_OSX:
            return "OSX";
        case OF_TARGET_WINGCC:
            return "WINGCC";
        case OF_TARGET_WINVS:
            return "WINVS";
        case OF_TARGET_IOS:
            return "IOS";
        case OF_TARGET_ANDROID:
            return "ANDROID";
        case OF_TARGET_LINUX:
            return "LINUX";
        case OF_TARGET_LINUX64:
            return "LINUX64";
        case OF_TARGET_LINUXARMV6L: // arm v6 little endian
            return "LINUXARMV6L";
        case OF_TARGET_LINUXARMV7L: // arm v7 little endian
            return "LINUXARMV7L";
        default:
            return "UNKNOWN";
    }
}


ofTargetPlatform SketchUtils::TargetPlatformFromString(const std::string& targetPlatform)
{
    if ("OSX" == targetPlatform)
    {
        return OF_TARGET_OSX;
    }
    else if ("WINGCC" == targetPlatform)
    {
        return OF_TARGET_WINGCC;
    }
    else if ("WINVS" == targetPlatform)
    {
        return OF_TARGET_WINVS;
    }
    else if ("IOS" == targetPlatform)
    {
        return OF_TARGET_IOS;
    }
    else if ("ANDROID" == targetPlatform)
    {
        return OF_TARGET_ANDROID;
    }
    else if ("LINUX" == targetPlatform)
    {
        return OF_TARGET_LINUX;
    }
    else if ("LINUX64" == targetPlatform)
    {
        return OF_TARGET_LINUX;
    }
    else if ("LINUXARMV6L" == targetPlatform)
    {
        return OF_TARGET_LINUXARMV6L;
    }
    else if ("LINUXARMV7L" == targetPlatform)
    {
        return OF_TARGET_LINUXARMV7L;
    }
    else
    {
        ofLogError("SketchUtils::TargetPlatformFromString") << "Unknown targetPlatform: " << targetPlatform;
        return getTargetPlatform();
    }
}

    /// \brief Check to see if the current build system contains a commend.
    /// \param command The command to check for.
    /// \returns true iff the current build system contains the command.
    static bool hasDependency(const std::string& command);

bool SketchUtils::hasDependency(const std::string& command)
{
    try
    {
        std::string cmd("which");
        std::vector<std::string> args;
        args.push_back(command);
        Poco::Pipe outPipe;
        Poco::ProcessHandle ph = Poco::Process::launch(cmd,
                                                       args,
                                                       0,
                                                       &outPipe,
                                                       0);
        Poco::PipeInputStream istr(outPipe);
        std::stringstream ostr;
        Poco::StreamCopier::copyStream(istr, ostr);
        std::string result = ostr.str();
        return !result.empty();
    }
    catch (const Poco::Exception& exc)
    {
        // This probably happened because the `which` program was not available on Windows.
        ofLogError("SketchUtils::hasDependency") << exc.displayText();
        return true;
    }
}


} } // namespace of::Sketch
