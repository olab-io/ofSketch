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


#include "Paths.h"
#include "ofUtils.h"
#include "ofx/IO/PathUtils.h"


namespace of {
namespace Sketch {


Paths::Paths():
    _projectsPath(ofx::IO::PathUtils::getUserDocumentsPath(),
                  Poco::Path::forDirectory("ofSketch"))
{
}

    
Paths::~Paths()
{
}


Poco::Path Paths::getProjectsPath() const
{
    return _projectsPath;
}


void Paths::setProjectsPath(const Poco::Path& projectsPath)
{
    _projectsPath = projectsPath;
}


Poco::Path Paths::getOpenFrameworksPath() const
{
    return Poco::Path::forDirectory(ofToDataPath("openFrameworks", true));
}


Poco::Path Paths::getCoreAddonsPath() const
{
    return Poco::Path(getOpenFrameworksPath(), Poco::Path::forDirectory("addons"));
}


Poco::Path Paths::getResourcesPath() const
{
    return Poco::Path::forDirectory(ofToDataPath("Resources", true));
}


Poco::Path Paths::getTemplatesPath() const
{
    return Poco::Path::forDirectory(ofToDataPath("Templates", true));
}


Poco::Path Paths::getToolchainsPath() const
{
    return Poco::Path::forDirectory(ofToDataPath("Toolchains", true));
}


Poco::Path Paths::getExamplesPath() const
{
    return Poco::Path::forDirectory(ofToDataPath("Examples", true));
}


Poco::Path Paths::getAddonsPath() const
{
    return Poco::Path(_projectsPath, Poco::Path::forDirectory("addons"));
}


Poco::Path Paths::getSettingsPath()
{
    return Poco::Path(Poco::Path::home(),
                      Poco::Path::forDirectory(".config/ofSketch"));
}


} } // namespace of::Sketch
