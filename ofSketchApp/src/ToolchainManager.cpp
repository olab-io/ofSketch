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


#include "ToolchainManager.h"
#include "MakeToolchain.h"
#include "ofLog.h"


namespace of {
namespace Sketch {


ToolchainManager::ToolchainManager(Settings& settings): _settings(settings)
{
}

    
ToolchainManager::~ToolchainManager()
{
}


void ToolchainManager::setup()
{
	// Currently this method simply lists the contents of the Toolchain
	// directory and does little more.  Future versions may allow the
	// possibility of choosing from various toolchains using the
	// ToolChainManager.
	Poco::Path toolchainPath = _settings.paths().toolchainsPath();

    try
    {
		std::vector<Poco::File> files;

        DirectoryUtils::list(toolchainPath, files, true, &_directoryFilter);

		for (const auto& toolchain: files)
		{
			std::string toolchainName = Poco::Path(toolchain.path()).getBaseName();
			ofLogVerbose("ToolchainManager::setup") << "Loading Toolchain: " << toolchainName;
		}
    }
    catch (const Poco::Exception& exc)
    {
        ofLogFatalError("ToolchainManager::setup") << exc.displayText();
    }

    /// \todo More sophisticated toolchain selection from the filesystem.
	/// for now, all building is done with the default system makefile.
    _toolchains["default"] = std::make_shared<MakeToolchain>("default");

}


Poco::Task* ToolchainManager::newBuildTask(const Project& project,
                                           const std::string& target,
                                           const std::string& toolchain)
{
    ToolchainMap::iterator iter = _toolchains.find(toolchain);

    if (iter != _toolchains.end())
    {
        return iter->second->newBuildTask(project, target);
    }
    else
    {
        ofLogError("ToolchainManager::build") << "No toolchain called " << toolchain;
        return 0;
    }
}


Poco::Task* ToolchainManager::newRunTask(const Project& project,
                                         const std::string& target,
                                         const std::string& toolchain)
{
    ToolchainMap::iterator iter = _toolchains.find(toolchain);

    if (iter != _toolchains.end())
    {
        return iter->second->newRunTask(project, target);
    }
    else
    {
        ofLogError("ToolchainManager::run") << "No toolchain called " << toolchain;
        return 0;
    }
}



} } // namespace of::Sketch
