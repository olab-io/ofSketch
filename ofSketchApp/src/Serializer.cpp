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


#include "Serializer.h"
#include <string>
#include "Poco/Exception.h"
#include "ofLog.h"
#include "ofx/IO/ByteBuffer.h"
//#include "ofx/IO/ByteBufferReader.h"
#include "ofx/IO/ByteBufferUtils.h"
//#include "ofx/IO/ByteBufferWriter.h"
#include "ClientSettings.h"
#include "Paths.h"
#include "Settings.h"
#include "ServerSettings.h"
#include "ofSSLManager.h"
#include "Poco/Net/NetException.h"
#include "Addon.h"


namespace of {
namespace Sketch {


bool Serializer::fromFile(const std::string& path, Json::Value& value)
{
    try
    {
        ofx::IO::ByteBuffer buffer;
        ofx::IO::ByteBufferUtils::loadFromFile(path, buffer);
        return fromString(buffer.toString(), value);
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Serializer::fromFile") << exc.displayText();
        return false;
    }
}


bool Serializer::toFile(const std::string& path, const Json::Value& value)
{
    try
    {
        ofx::IO::ByteBuffer buffer(toString(value));
        return ofx::IO::ByteBufferUtils::saveToFile(buffer, path);
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Serializer::toFile") << exc.displayText();
        return false;
    }
}


bool Serializer::fromString(const std::string& jsonString, Json::Value& value)
{
    Json::Reader reader;

    if (!reader.parse(jsonString, value))
    {
        ofLogError("Serializer::fromString") << "Unable to parse " << jsonString << ": " << reader.getFormattedErrorMessages();
        return false;
    }
    else
    {
        return true;
    }
}


std::string Serializer::toString(const Json::Value& json)
{
    Json::FastWriter writer;
    return writer.write(json);
}


Settings Serializer::loadSettings()
{
    Settings defaults;
    Json::Value settingsJSON;

    if (fromFile(Poco::Path(Paths::settingsPath(), "Settings.json").toString(), settingsJSON))
    {
        fromJSON(settingsJSON, defaults);
    }

    return defaults;
}


bool Serializer::saveSettings(const Settings& settings)
{
    Poco::File settingsFolder(Paths::settingsPath());

    // If the settings folder does not exist, create it.
    settingsFolder.createDirectories();

    return toFile(Poco::Path(Paths::settingsPath(), "Settings.json").toString(), toJSON(settings));
}


bool Serializer::fromJSON(const Json::Value& json, Settings& object)
{
    Paths paths;
    ClientSettings client;
    ServerSettings server;

    if (json.isMember("paths"))
    {
        fromJSON(json["paths"], paths);
    }

    if (json.isMember("client"))
    {
        fromJSON(json["client"], client);
    }

    if (json.isMember("server"))
    {
        fromJSON(json["server"], server);
    }

    object = Settings(paths, client, server);

    return true;
}


Json::Value Serializer::toJSON(const Settings& object)
{
    Json::Value json;
    json["paths"] = toJSON(object.paths());
    json["client"] = toJSON(object.serverSettings());
    json["server"] = toJSON(object.clientSettings());
    return json;
}


bool Serializer::fromJSON(const Json::Value& json, ClientSettings& object)
{
    ClientSettings defaults;

    object.setEditorSettings(json.get("editor", defaults.getEditorSettings()));

    if (json.isMember("autosave"))
    {
        const Json::Value& autosave = json["autosave"];

        object.setAutosaveEnabled(autosave.get("enabled",
                                               defaults.isAutosaveEnabled()).asBool());

        object.setAutosaveFrequency(autosave.get("frequency",
                                                 defaults.getAutosaveFrequency()).asUInt64());

    }
    else
    {
        object.setAutosaveEnabled(defaults.isAutosaveEnabled());
        object.setAutosaveFrequency(defaults.getAutosaveFrequency());
    }

    return true;
}


Json::Value Serializer::toJSON(const ClientSettings& object)
{
    Json::Value json;

    json["autosave"]["enabled"] = object.isAutosaveEnabled();
    json["autosave"]["frequency"] = object.getAutosaveFrequency();
    json["editor"] = object.getEditorSettings();

    return json;
}


bool Serializer::fromJSON(const Json::Value& json, Paths& object)
{
    Paths defaults;

    object.setProjectsPath(json.get("projects",
                                    defaults.getProjectsPath().toString()).asString());

    return true;
}


Json::Value Serializer::toJSON(const Paths& object)
{
    Json::Value json;
    json["projects"] = object.getProjectsPath().toString();
    return json;
}


bool Serializer::fromJSON(const Json::Value& json, ServerSettings& object)
{
    ServerSettings defaults;

    ofx::Net::IPAddressRange::List whitelist;

    if (json.isMember("whitelist"))
    {
        const Json::Value& whitelistJSON = json["whitelist"];

        for (Json::ArrayIndex i = 0; i < whitelistJSON.size(); ++i)
        {
            Json::Value jsonIP = whitelistJSON.get(i, Json::nullValue);

            if (!jsonIP.isNull() && jsonIP.isString())
            {
                try
                {
                    ofx::Net::IPAddressRange ip(jsonIP.asString());
                    whitelist.push_back(ip);
                }
                catch (const Poco::Net::InvalidAddressException& exc)
                {
                    ofLogError("Serializer::fromJSON") << exc.displayText();
                }
            }
            else
            {
                ofLogError("Serializer::fromJSON") << jsonIP.toStyledString();
            }
        }
    }
    else
    {
        cout << ofToString(defaults.getWhitelist()) << endl;


        whitelist = defaults.getWhitelist();
    }

    SSLSettings sslSettings;

    if (json.isMember("sslSettings"))
    {
        fromJSON(json["sslSettings"], sslSettings);
    }

    object = ServerSettings(json.get("port", defaults.getPort()).asUInt(),
                            json.get("webSocketBufferSize", (Json::Value::UInt64)defaults.getWebSocketBufferSize()).asUInt64(),
                            json.get("maximumFileUploadSize", (Json::Value::UInt64)defaults.getMaxiumFileUploadSize()).asUInt64(),
                            whitelist,
                            json.get("useSSL", defaults.useSSL()).asBool(),
                            sslSettings);
}


Json::Value Serializer::toJSON(const ServerSettings& object)
{
    Json::Value json;

    json["port"] = object.getPort();
    json["webSocketBufferSize"] = (Json::Value::UInt64)object.getWebSocketBufferSize();
    json["maximumFileUploadSize"] = (Json::Value::UInt64)object.getMaxiumFileUploadSize();

    Json::Value whitelistJSON;

    const ofx::Net::IPAddressRange::List& whitelist = object.getWhitelist();

    for (std::size_t i = 0; i < whitelist.size(); ++i)
    {
        whitelistJSON.append(whitelist[i].toString());
    }

    json["whitelist"] = whitelistJSON;

    json["useSSL"] = object.useSSL();
    json["sslSettings"] = toJSON(object.getSSLSettings());

    return json;
}


bool Serializer::fromJSON(const Json::Value& json, SSLSettings& object)
{
    SSLSettings defaults;

    object = SSLSettings(json.get("privateKey", defaults.getPrivateKeyPath()).asString(),
                         json.get("certificate", defaults.getCertificatePath()).asString(),
                         json.get("caCert", defaults.getCACertPath()).asString(),
                         ofSSLManager::verificationModeFromString(json.get("verificationMode",
                                                                           defaults.getVerificationMode()).asString()),
                         json.get("verificationDepth", defaults.getVerificationDepth()).asInt(),
                         json.get("loadDefaultCAs", defaults.getLoadDefaultCAs()).asBool(),
                         json.get("cipherList", defaults.getCipherList()).asString());

    return true;
}


Json::Value Serializer::toJSON(const SSLSettings& object)
{
    Json::Value json;

    json["privateKey"] = object.getPrivateKeyPath();
    json["certificate"] = object.getCertificatePath();
    json["caCert"] = object.getCACertPath();
    json["verificationMode"] = ofSSLManager::verificationModeToString(object.getVerificationMode());
    json["verificationDepth"] = object.getVerificationDepth();
    json["loadDefaultCAs"] = object.getLoadDefaultCAs();
    json["cipherList"] = object.getCipherList();

    return json;
}


Json::Value Serializer::toJSON(const Addon& object)
{
    Json::Value json;

    json["path"] = object.getPath().toString();
    json["name"] = object.getName();
    json["author"] = object.getAuthor();

    std::vector<std::string>::const_iterator iter = object.getTags().begin();

    while (iter != object.getTags().end())
    {
        json["tags"].append(*iter);
        ++iter;
    }

    json["url"] = object.getURL();

    return json;
}


//Json::Value ProcessTaskQueue::toJson(const ofx:ProgressMap) const
//{
//    /// TODO: This method may need to be syncrhonized (which means that
//    /// all of our ProcessTaskQueue may need to be synchronized depending on
//    /// how our websocket route is synchronized.
//
//    Json::Value json;
//
//    ProgressMap::const_iterator iter = _IDTaskProgressMap.begin();
//
//    while (iter != _IDTaskProgressMap.end())
//    {
//        const ofx::TaskProgressEventArgs_<Poco::UUID> info = iter->second;
//
//        Json::Value progressJson;
//
//        progressJson["name"] = info.getTaskName();
//        progressJson["uuid"] = info.getTaskId().toString();
//        progressJson["progress"] = info.getProgress();
//        progressJson["message"] = ""; // None for now.
//
//        // Add each of the current tasks to the json array.
//        json.append(progressJson);
//        
//        ++iter;
//    }
//    
//    return json;
//}


} } // namespace of::Sketch
