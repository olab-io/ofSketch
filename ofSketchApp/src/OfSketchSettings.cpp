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


#include "OfSketchSettings.h"

namespace of {
namespace Sketch {


OfSketchSettings::OfSketchSettings(const std::string& path):
    _path(path)
{
    load();
    ofLogVerbose("EditorSettings::EditorSettings") << "Project Directory: " << getProjectDir();
}

bool OfSketchSettings::load()
{
    return _data.open(_path);
}

bool OfSketchSettings::save()
{
    return _data.save(_path);
}

const Json::Value& OfSketchSettings::getData() const
{
    return _data;
}

int OfSketchSettings::getPort() const
{
    return _data["port"].asInt();
}
    
std::string OfSketchSettings::getProjectDir() const
{
    return _data["projectDir"].asString();
}

std::string OfSketchSettings::getSketchDir() const
{
    return _data["sketchDir"].asString();
}

std::string OfSketchSettings::getAddonsDir() const
{
    return _data["addonsDir"].asString();
}

std::string OfSketchSettings::getOpenFrameworksDir() const
{
    return _data["openFrameworksDir"].asString();
}

std::string OfSketchSettings::getOpenFrameworksVersion() const
{
    return _data["openFrameworksVersion"].asString();
}
   
std::string OfSketchSettings::getProjectSettingsFilename() const
{
    return _data["projectSettingsFilename"].asString();
}
    
std::string OfSketchSettings::getProjectExtension() const
{
    return _data["projectExtension"].asString();
}

std::string OfSketchSettings::getClassExtension() const
{
    return _data["classExtension"].asString();
}

} } // namespace of::Sketch
