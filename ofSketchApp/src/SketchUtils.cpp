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


#include "SketchUtils.h"
#include "Poco/Process.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include "ofxIO.h"
#include "Constants.h"


namespace of {
namespace Sketch {


bool SketchUtils::JSONfromFile(const std::string& path, Json::Value& value)
{
    try
    {
        ofx::IO::ByteBuffer buffer;
        ofx::IO::ByteBufferUtils::loadFromFile(path, buffer);
        return JSONfromString(buffer.toString(), value);
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Utils::fromFile") << exc.displayText();
        return false;
    }
}


bool SketchUtils::JSONtoFile(const std::string& path, const Json::Value& value)
{
    try
    {
        ofx::IO::ByteBuffer buffer(SketchUtils::toJSONString(value));
        return ofx::IO::ByteBufferUtils::saveToFile(buffer, path);
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Utils::fromFile") << exc.displayText();
        return false;
    }
}


bool SketchUtils::JSONfromString(const std::string& jsonString, Json::Value& value)
{
    Json::Reader reader;

    if (!reader.parse(jsonString, value))
    {
        ofLogError("Utils::fromFile") << "Unable to parse " << jsonString << ": " << reader.getFormattedErrorMessages();
        return false;
    }
    else
    {
        return true;
    }
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


std::string SketchUtils::toJSONString(const Json::Value& json)
{
    Json::FastWriter writer;
    return writer.write(json);
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


std::string SketchUtils::toString(ofTargetPlatform targetPlatform)
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


std::string SketchUtils::getVersion()
{
    std::stringstream ss;

    ss << VERSION_MAJOR << ".";
    ss << VERSION_MINOR << ".";
    ss << VERSION_PATCH;

    if (!getVersionSpecial().empty())
    {
        ss << "-" << getVersionSpecial();
    }

    return ss.str();
}

int SketchUtils::getVersionMajor()
{
    return VERSION_MAJOR;
}


int SketchUtils::getVersionMinor()
{
    return VERSION_MINOR;
}


int SketchUtils::getVersionPatch()
{
    return VERSION_PATCH;
}


std::string SketchUtils::getVersionSpecial()
{
    return VERSION_SPECIAL;
}


} } // namespace of::Sketch
