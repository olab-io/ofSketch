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


#pragma once


#include <string>
#include <set>
#include "Poco/URI.h"
#include "Poco/RegularExpression.h"
#include "ofx/IO/DirectoryUtils.h"
#include "ofx/IO/DirectoryFilter.h"
#include "ofx/IO/DirectoryWatcherManager.h"
#include "ofEvents.h"
#include "ofLog.h"
#include "Addon.h"


namespace of {
namespace Sketch {


class AddonManager
{
public:
    AddonManager(const std::string& addonsPath);
    virtual ~AddonManager();

    void onDirectoryWatcherItemAdded(const ofx::DirectoryWatcher::DirectoryEvent& evt);
    void onDirectoryWatcherItemRemoved(const ofx::DirectoryWatcher::DirectoryEvent& evt);
    void onDirectoryWatcherItemModified(const ofx::DirectoryWatcher::DirectoryEvent& evt);
    void onDirectoryWatcherItemMovedFrom(const ofx::DirectoryWatcher::DirectoryEvent& evt);
    void onDirectoryWatcherItemMovedTo(const ofx::DirectoryWatcher::DirectoryEvent& evt);
    void onDirectoryWatcherError(const Poco::Exception& exc);

    std::vector<Addon::SharedPtr> getAddons() const;

    static const std::string DEFAULT_ADDON_PATH;

private:
    std::string _path;
    std::map<std::string, Addon::SharedPtr> _addons;
    ofx::IO::DirectoryWatcherManager _addonWatcher;

    ofx::IO::DirectoryFilter _directoryFilter;

};


} } // namespace of::Sketch
