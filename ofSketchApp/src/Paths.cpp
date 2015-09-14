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


#include "Paths.h"
#include "ofUtils.h"
#include "Poco/Path.h"


namespace of {
namespace Sketch {


Paths::Paths():
	_projectsPath(Poco::Path::forDirectory(Poco::Path::home() + "/Documents/ofSketch").toString())
{
}


Paths::Paths(const std::string& projectsPath):
    _projectsPath(projectsPath)
{
}


Paths::~Paths()
{
}


const std::string& Paths::getProjectsPath() const
{
    return _projectsPath;
}


void Paths::setProjectsPath(const std::string& projectsPath)
{
    _projectsPath = projectsPath;
}


std::string Paths::addonsPath() const
{
    return Poco::Path(_projectsPath, Poco::Path::forDirectory("addons")).toString();
}


std::string Paths::openFrameworksPath()
{
    return Poco::Path::forDirectory(ofToDataPath("openFrameworks", true)).toString();
}


std::string Paths::coreAddonsPath()
{
    return Poco::Path(openFrameworksPath(), Poco::Path::forDirectory("addons")).toString();
}


std::string Paths::resourcesPath()
{
    return Poco::Path::forDirectory(ofToDataPath("Resources", true)).toString();
}


std::string Paths::templatesPath()
{
    return Poco::Path::forDirectory(ofToDataPath("Templates", true)).toString();
}


std::string Paths::toolchainsPath()
{
    return Poco::Path::forDirectory(ofToDataPath("Toolchains", true)).toString();
}


std::string Paths::examplesPath()
{
    return Poco::Path::forDirectory(ofToDataPath("Examples", true)).toString();
}


std::string Paths::settingsPath()
{
    return Poco::Path(Poco::Path::home(),
                      Poco::Path::forDirectory(".config/ofSketch")).toString();
}


} } // namespace of::Sketch
