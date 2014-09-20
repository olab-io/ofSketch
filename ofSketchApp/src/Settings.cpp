// =============================================================================
//
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


#include "Settings.h"
#include "ofUtils.h"
#include "Utils.h"
#include "Poco/Net/NetException.h"


namespace of {
namespace Sketch {


const Poco::Path Settings::DEFAULT_SETTINGS_PATH("Resources/Settings/Defaults.json", Poco::Path::PATH_UNIX);
const Poco::Path Settings::USER_SETTINGS_PATH(Poco::Path::home(), ".ofsketch.json");


Settings::Settings()
{
    Poco::File defaultSettingsFile(ofToDataPath(DEFAULT_SETTINGS_PATH.toString(),
                                                true));

    if (!defaultSettingsFile.exists())
    {
        ofLogFatalError("Settings::Settings()") << "Default settings file not found: " << defaultSettingsFile.path();
        return;
    }

    if (Utils::JSONfromFile(defaultSettingsFile.path(), _defaultSettings))
    {
        ofLogFatalError("Settings::Settings()") << "Unable to parse default settings file.";
        return;
    }

    Poco::File userSettingsFile(USER_SETTINGS_PATH);

    if (!userSettingsFile.exists())
    {
        _userSettings = _defaultSettings;
        save();
    }
    else if (!Utils::JSONfromFile(userSettingsFile.path(), _userSettings))
    {
        ofLogFatalError("Settings::Settings()") << "Default settings file not found: " << defaultSettingsFile.path();
        _userSettings = _defaultSettings;
    }
    else
    {
        // It was successful and set already.
    }
}


bool Settings::save() const
{
    return Utils::JSONtoFile(USER_SETTINGS_PATH.toString(), _userSettings);
}


bool Settings::update(const Json::Value& settings)
{
    // TODO:: validate these settings, as they come
    // from the web client and could be manipulated.
    _userSettings = settings;

	return true;
}


Json::Value Settings::toJSON() const
{
    return _userSettings;
}


Poco::Path Settings::getDataPath() const
{
//    ofToDataPath(<#const string &path#>)
	return Poco::Path();
}


Poco::Path Settings::addonsPath() const
{
    return Poco::Path(_userSettings["paths"]["addons"].asString());
}


Poco::Path Settings::openFrameworksPath() const
{
    return Poco::Path(_userSettings["paths"]["openFrameworks"].asString());
}


Poco::Path Settings::projectsPath() const
{
    return Poco::Path(_userSettings["paths"]["projects"].asString());
}


Poco::Path Settings::compilerTemplatesPath() const
{
    return Poco::Path(_userSettings["paths"]["compilerTemplatesPath"].asString());
}


Poco::Path Settings::editorTemplates() const
{
    return Poco::Path(_userSettings["paths"]["editorTemplates"].asString());
}


Poco::Path Settings::projectTemplate() const
{
    return Poco::Path(_userSettings["paths"]["projectTemplate"].asString());
}


std::size_t Settings::webSocketBufferSize() const
{
    return _userSettings["server"]["webSocketBufferSize"].asLargestUInt();
}


unsigned short Settings::port() const
{
    return _userSettings["server"]["port"].asUInt();
}


bool Settings::useSSL() const
{
    return _userSettings["server"]["ssl"].asBool();
}


unsigned long Settings::maxiumFileUploadSize() const
{
    return _userSettings["server"]["maxiumFileUploadSize"].asUInt64();
}


bool Settings::allowRemoteConnections() const
{
    return _userSettings["server"]["security"]["connections"]["allowRemote"].asBool();
}

std::vector<Poco::Net::IPAddress> Settings::whitelistedIPs() const
{
    std::vector<Poco::Net::IPAddress> addresses;

    Json::Value whiteListedIPs = _userSettings["server"]["security"]["connections"]["whitelistedIPs"];

    if (whiteListedIPs.isArray())
    {
        for (Json::ArrayIndex i = 0; i < whiteListedIPs.size(); ++i)
        {
            Json::Value jsonIP = whiteListedIPs.get(i, Json::nullValue);

            if (!jsonIP.isNull() && jsonIP.isString())
            {
                try
                {
                    Poco::Net::IPAddress ip(jsonIP.asString());
                    addresses.push_back(ip);
                }
                catch (const Poco::Net::InvalidAddressException& exc)
                {
                    ofLogError("Settings::whitelistedIPs") << exc.displayText();
                }
            }
            else
            {
                // nothing
            }
        }

    }
    else
    {
        return addresses;
    }
}

Poco::Path Settings::sslPrivateKeyPath() const
{
    return Poco::Path(_userSettings["server"]["security"]["ssl"]["privateKey"].asString());
}

Poco::Path Settings::sslSelfSignedCertificatePath() const
{
    return Poco::Path(_userSettings["server"]["security"]["ssl"]["selfSignedCertificate"].asString());
}

Poco::Path Settings::sslCACertPath() const
{
    return Poco::Path(_userSettings["server"]["security"]["ssl"]["cacert"].asString());
}

Poco::Net::Context::VerificationMode Settings::sslVerificationMode() const
{
    std::string mode = _userSettings["server"]["security"]["ssl"]["cacert"].asString();

    if (mode == "VERIFY_NONE")
    {
        return Poco::Net::Context::VERIFY_NONE;
    }
    else if (mode == "VERIFY_RELAXED")
    {
        return Poco::Net::Context::VERIFY_RELAXED;
    }
    else if (mode == "VERIFY_STRICT")
    {
        return Poco::Net::Context::VERIFY_STRICT;
    }
    else if (mode == "VERIFY_ONCE")
    {
        return Poco::Net::Context::VERIFY_ONCE;
    }
    else
    {
        ofLogWarning("Settings::sslVerificationMode") << "Unrecognized verification mode: " << mode;
        return Poco::Net::Context::VERIFY_RELAXED;
    }
}


int Settings::sslVerificationDepth() const
{
    return _userSettings["server"]["security"]["ssl"]["verificationDepth"].asInt();
}


bool Settings::sslLoadDefaultCAs() const
{
    return _userSettings["server"]["security"]["ssl"]["loadDefaultCAs"].asBool();
}


std::string Settings::sslCipherList() const
{
    return _userSettings["server"]["security"]["ssl"]["cipherList"].asString();
}

Json::Value Settings::editorSettings() const
{
    return _userSettings["editor"];
}


} } // namespace of::Sketch
