// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
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
#include "SketchUtils.h"


namespace of {
namespace Sketch {


AddonManager::AddonManager(Settings& settings): _settings(settings)
{
    _addonWatcher.registerAllEvents(this);
}


AddonManager::~AddonManager()
{
    _addonWatcher.unregisterAllEvents(this);
}


void AddonManager::setup()
{
    Poco::File addonsDirectory(_settings.getPaths().getAddonsPath());
    Poco::File coreAddonsDirectory(_settings.getPaths().getCoreAddonsPath());

    try
    {
        addonsDirectory.createDirectories();

        std::vector<Poco::File> files;

        DirectoryUtils::list(coreAddonsDirectory, files, true, &_directoryFilter);

        std::vector<Poco::File>::iterator iter = files.begin();

        while (iter != files.end())
        {
            const Poco::File& addon = *iter;

            std::string addonName = Poco::Path(addon.path()).getBaseName();

            Poco::Path targetAddonPath(_settings.getPaths().getAddonsPath(),
                                       Poco::Path::forDirectory(addonName));

            Poco::File targetAddon(targetAddonPath);

            if (!targetAddon.exists())
            {
                ofLogNotice("AddonManager::setup") << "Copying core addon: " << addonName;
                addon.copyTo(addonsDirectory.path());
                targetAddon.setReadOnly();
            }

            ++iter;
        }

    }
    catch (const Poco::Exception& exc)
    {
        ofLogFatalError("AddonManager::setup") << "Unable to create projects directory: " << exc.displayText();
    }

    _addonWatcher.addPath(_settings.getPaths().getAddonsPath(),
                          true,
                          true,
                          &_directoryFilter);

}


const std::vector<Addon>& AddonManager::getAddons() const
{
    return _addons;
}


void AddonManager::onDirectoryWatcherItemAdded(const DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("AddonManager::onDirectoryWatcherItemAdded") << evt.event << " " << evt.item.path();

    Poco::Path path(evt.item.path());

    std::string name = path.getBaseName();

    // \todo Parse from addon_config.mk.
    Addon addon(path,
                name,
                "",
                "",
                std::vector<std::string>(),
                "");

    _addons.push_back(addon);
}


void AddonManager::onDirectoryWatcherItemRemoved(const DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("AddonManager::onDirectoryWatcherItemRemoved") << evt.event << " " << evt.item.path();

    Poco::Path path(evt.item.path());

    std::string name = path.getBaseName();

    // \todo Parse from addon_config.mk.
    Addon addon(path,
                name,
                "",
                "",
                std::vector<std::string>(),
                "");

    std::vector<Addon>::iterator iter = _addons.begin();

    while (iter != _addons.end())
    {
        const Addon& _addon = *iter;

        if (_addon.getName() == addon.getName())
        {
            _addons.erase(iter);
            return;
        }

        ++iter;
    }

    ofLogError("AddonManager::onDirectoryWatcherItemRemoved") << "Unable to remove " << path.toString();
}


void AddonManager::onDirectoryWatcherItemModified(const DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("AddonManager::onDirectoryWatcherItemModified") << evt.event << " " << evt.item.path();
}


void AddonManager::onDirectoryWatcherItemMovedFrom(const DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("AddonManager::onDirectoryWatcherItemMovedFrom") << evt.event << " " << evt.item.path();
}


void AddonManager::onDirectoryWatcherItemMovedTo(const DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("AddonManager::onDirectoryWatcherItemMovedTo") << evt.event << " " << evt.item.path();
}


void AddonManager::onDirectoryWatcherError(const Poco::Exception& exc)
{
    ofLogError("AddonManager::onDirectoryWatcherError") << exc.displayText();
}


} } // namespace of::Sketch
