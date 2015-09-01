// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include <map>
#include "Poco/Task.h"
#include "ofTypes.h"
#include "ofx/IO/DirectoryUtils.h"
#include "ofx/IO/DirectoryFilter.h"
#include "ofx/IO/DirectoryWatcherManager.h"
#include "Settings.h"


namespace of {
namespace Sketch {


class BaseToolchain;
class Project;


using ofx::DirectoryWatcher;
using ofx::IO::DirectoryFilter;
using ofx::IO::DirectoryUtils;
using ofx::IO::DirectoryWatcherManager;


/// \brief The ToolchainManager.
class ToolchainManager
{
public:
    /// \brief Create a default ToolchainManager instance.
    ToolchainManager(Settings& settings);

    /// \brief Destroy the ToolchainManager instance.
    virtual ~ToolchainManager();

    void setup();

    Poco::Task* newBuildTask(const Project& project,
                             const std::string& target,
                             const std::string& toolchian);

    Poco::Task* newRunTask(const Project& project,
                           const std::string& target,
                           const std::string& toolchian);

private:
    typedef std::shared_ptr<BaseToolchain> Toolchain;
    typedef std::map<std::string, Toolchain> ToolchainMap;

    /// \brief Settings.
    Settings& _settings;

    ToolchainMap _toolchains;

    DirectoryFilter _directoryFilter;

};


} } // namespace of::Sketch
