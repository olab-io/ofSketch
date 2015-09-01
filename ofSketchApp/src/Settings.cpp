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


#include "Settings.h"
#include "json/json.h"
#include "ofUtils.h"
#include "SketchUtils.h"
#include "Poco/FileStream.h"
#include "Poco/Net/NetException.h"


namespace of {
namespace Sketch {


Settings::Settings()
{
}


Settings::Settings(const Paths& paths,
                   const ClientSettings& clientSettings,
                   const ServerSettings& serverSettings):
    _paths(paths),
    _clientSettings(clientSettings),
    _serverSettings(serverSettings)
{
}


//void Settings::save()
//{
////    Poco::Path settingsDirectoryPath(Poco::Path::home(),
////                                     Poco::Path(USER_SETTINGS_PATH,
////                                                SETTINGS_FILENAME));
////    try
////    {
////        Poco::FileOutputStream fos(settingsDirectoryPath.toString());
////        Poco::Util::JSONConfiguration::save(fos);
////        fos.close();
////    }
////    catch (const Poco::Exception& exc)
////    {
////        ofLogError("Settings::save") << exc.displayText();
////    }
//}


//void Settings::setup()
//{
////    Poco::Path settingsDirectoryPath(Poco::Path::home(),
////                                     Poco::Path(USER_SETTINGS_PATH));
////
////    Poco::File settingsDirectory(settingsDirectoryPath);
////
////    try
////    {
////        if (!settingsDirectory.exists())
////        {
////            ofLogVerbose("Settings::setup") << "Settings directory " << settingsDirectory.path() << " does not exist.";
////
////            if (settingsDirectory.createDirectory())
////            {
////                ofLogVerbose("Settings::setup") << "Creating settings directory " << settingsDirectory.path();
////            }
////            else
////            {
////                ofLogError("Settings::setup") << "Unable to create settings directory: " << settingsDirectory.path();
////            }
////        }
////
////        Poco::Path userSettingsPath(settingsDirectoryPath, SETTINGS_FILENAME);
////        Poco::File userSettingsFile(userSettingsPath);
////
////        if (!userSettingsFile.exists())
////        {
////            Poco::Path defaultSettingsPath(ofToDataPath(DEFAULT_SETTINGS_PATH,
////                                                        true),
////                                           SETTINGS_FILENAME);
////
////            Poco::File defaultSettingsFile(defaultSettingsPath);
////
////            ofLogVerbose("Settings::setup") << "Default settings at: " << defaultSettingsFile.path();
////
////            if (defaultSettingsFile.exists())
////            {
////                ofLogVerbose("Settings::setup") << "Default settings at: " << defaultSettingsFile.path() << " are being copied to " << settingsDirectory.path();
////                defaultSettingsFile.copyTo(userSettingsFile.path());
////            }
////            else
////            {
////                ofLogFatalError("Settings::setup") << "Unable to locate default settings: " << defaultSettingsFile.path();
////            }
////        }
////
////        try
////        {
////            Poco::FileInputStream fis(userSettingsFile.path());
////            load(fis);
////            fis.close();
////        }
////        catch (const Poco::Exception& exc)
////        {
////            ofLogError("Settings::setup") << exc.displayText();
////        }
////    }
////    catch (const Poco::Exception& exc)
////    {
////        ofLogError("Settings::setup") << exc.displayText();
////    }
//}


const Paths& Settings::getPaths() const
{
    return _paths;
}


const ClientSettings& Settings::getClientSettings() const
{
    return _clientSettings;
}


const ServerSettings& Settings::getServerSettings() const
{
    return _serverSettings;
}


Paths& Settings::getPathsRef()
{
    return _paths;
}


ClientSettings& Settings::getClientSettingsRef()
{
    return _clientSettings;
}


ServerSettings& Settings::getServerSettingsRef()
{
    return _serverSettings;
}


} } // namespace of::Sketch
