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


#include "AddonManager.h"


namespace of {
namespace Sketch {


const std::string AddonManager::DEFAULT_ADDON_PATH = "addons/";


AddonManager::AddonManager(const std::string& path):
    _path(path)
{
    Poco::Path fullPath(ofToDataPath(_path, true));

    Poco::File file(fullPath);

    if (!file.exists())
    {
        Poco::FileNotFoundException exc(fullPath.toString());
        throw exc;
    }

    std::vector<Poco::File> files;

    ofx::IO::DirectoryUtils::list(file, files, true, &_directoryFilter);

    std::vector<Poco::File>::iterator iter = files.begin();

    while (iter != files.end())
    {
        std::string addonPath = (*iter).path();
        std::string addonName = Poco::Path(addonPath).getBaseName();
        
        Poco::RegularExpression addonExpression("ofx.+$", Poco::RegularExpression::RE_ANCHORED);
 
        if (addonExpression.match(addonName))
        {
            _addons[addonName] = Addon::SharedPtr(new Addon(addonName, addonPath));
        }

        ++iter;
    }

    _addonWatcher.registerAllEvents(this);
    _addonWatcher.addPath(fullPath.toString(),
                          false,
                          true,
                          &_directoryFilter);
}


AddonManager::~AddonManager()
{
    _addonWatcher.unregisterAllEvents(this);
}


void AddonManager::onDirectoryWatcherItemAdded(const ofx::DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("AddonManager::onDirectoryWatcherItemAdded") << evt.event << " " << evt.item.path();

    std::string path = evt.item.path();
    std::string name = Poco::Path(path).getBaseName();

    _addons[name] = Addon::SharedPtr(new Addon(name, path));

}


void AddonManager::onDirectoryWatcherItemRemoved(const ofx::DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("AddonManager::onDirectoryWatcherItemRemoved") << evt.event << " " << evt.item.path();

    std::string path = evt.item.path();
    std::string name = Poco::Path(path).getBaseName();

    std::map<std::string, Addon::SharedPtr>::iterator iter = _addons.find(name);

    if (iter != _addons.end())
    {
        _addons.erase(iter);
    }
    else
    {
        ofLogError("AddonManager::onDirectoryWatcherItemRemoved") << "Unable to find " << path;
    }
}


void AddonManager::onDirectoryWatcherItemModified(const ofx::DirectoryWatcher::DirectoryEvent& evt)
{
     ofLogNotice("AddonManager::onDirectoryWatcherItemModified") << evt.event << " " << evt.item.path();
}


void AddonManager::onDirectoryWatcherItemMovedFrom(const ofx::DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("AddonManager::onDirectoryWatcherItemMovedFrom") << evt.event << " " << evt.item.path();
}


void AddonManager::onDirectoryWatcherItemMovedTo(const ofx::DirectoryWatcher::DirectoryEvent& evt)
{
     ofLogNotice("AddonManager::onDirectoryWatcherItemMovedTo") << evt.event << " " << evt.item.path();
}


void AddonManager::onDirectoryWatcherError(const Poco::Exception& exc)
{
    ofLogError("AddonManager::onDirectoryWatcherError") << exc.displayText();
}


std::vector<Addon::SharedPtr> AddonManager::getAddons() const
{
    std::vector<Addon::SharedPtr> addons;

    std::map<std::string, Addon::SharedPtr>::const_iterator iter = _addons.begin();

    while (iter != _addons.end())
    {
        addons.push_back(iter->second);
        ++iter;
    }

    return addons;
}


} } // namespace of::Sketch
