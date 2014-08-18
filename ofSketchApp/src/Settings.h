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


#pragma once


#include <string>
#include <json/json.h>
#include "Poco/Environment.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/IPAddress.h"
#include "ofUtils.h"


namespace of {
namespace Sketch {


/// \brief Settings include all settings for ofSketch.
class Settings
{
public:
    /// \brief Create a default settings object.
    Settings();

    bool save() const;
    bool update(const Json::Value& settings);

    Json::Value toJSON() const;

    Poco::Path getDataPath() const;

    Poco::Path addonsPath() const;
    Poco::Path openFrameworksPath() const;
    Poco::Path projectsPath() const;
    Poco::Path compilerTemplatesPath() const;
    Poco::Path editorTemplates() const;
    Poco::Path projectTemplate() const;

    std::size_t webSocketBufferSize() const;
    unsigned short port() const;
    bool useSSL() const;

    unsigned long maxiumFileUploadSize() const;

    bool allowRemoteConnections() const;
    std::vector<Poco::Net::IPAddress> whitelistedIPs() const;

    Poco::Path sslPrivateKeyPath() const;
    Poco::Path sslSelfSignedCertificatePath() const;
    Poco::Path sslCACertPath() const;
    Poco::Net::Context::VerificationMode sslVerificationMode() const;
    int sslVerificationDepth() const;
    bool sslLoadDefaultCAs() const;
    std::string sslCipherList() const;

    Json::Value editorSettings() const;

    /// \brief The path for the default ofSketch settings, relative to bin/data.
    static const Poco::Path DEFAULT_SETTINGS_PATH;

    /// \brief The absolute path for the user-specific ofSketch settings.
    static const Poco::Path USER_SETTINGS_PATH;

private:
    /// \brief An internal copy of the default settings.
    Json::Value _defaultSettings;

    /// \brief A JSON object containing all settings.
    Json::Value _userSettings;

};


} } // namespace of::Sketch
