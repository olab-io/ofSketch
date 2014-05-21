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


#include "Project.h"


namespace of {
namespace Sketch {

Project::~Project()
{
}

Project::Project(const std::string& path):
    _path(path),
    _isLoaded(false)
{
    load(_path, getName());
}

bool Project::load(const std::string path, const std::string& name)
{
    ofDirectory sketchDir(ofToDataPath(path + "/sketch"));
    if (sketchDir.exists()) {
        sketchDir.listDir();
        std::vector<ofFile> files = sketchDir.getFiles();
        int classCounter = 0;
        for (int i = 0; i < files.size(); i++) {
            ofFile file = files[i];
            cout<<"File"<<file.getFileName()<<" can be read: "<<file.canRead()<<endl;
            if (file.getBaseName() == name) {
                file.open(file.getAbsolutePath());
                _data["projectFile"]["name"] = file.getBaseName();
                _data["projectFile"]["fileName"] = file.getFileName();
                _data["projectFile"]["fileContents"] = file.readToBuffer().getText();
            } else if (file.getExtension() == "sketch") {
                file.open(file.getAbsolutePath());
                _data["classes"][classCounter]["name"] = file.getBaseName();
                _data["classes"][classCounter]["fileName"] = file.getFileName();
                _data["classes"][classCounter]["fileContents"] = file.readToBuffer().getText();
                classCounter++;
            }
        }
        cout<<_data.getRawString()<<endl;
        _isLoaded = true;
        return true;
    } else return false;
}
    
bool Project::isLoaded()
{
    return _isLoaded;
}

bool Project::save(ofxJSONElement data){
    
}
    
bool Project::create(const std::string& path)
{
    ofDirectory project(ofToDataPath(path));
    if (!project.exists()) {
        ofDirectory temp(ofToDataPath("Resources/Templates/SimpleTemplate"));
        temp.copyTo(ofToDataPath(path));
        return true;
    }
    
    return false;
    
}

bool Project::rename(const std::string& name)
{
    
}
    
bool Project::hasClasses() const
{
    return _data["classes"].size() > 0;
}

const std::string& Project::getPath() const
{
    return _path;
}
    
std::string Project::getName() const
{
    ofFile project(_path);
    return project.getBaseName();
}
    
const ofxJSONElement& Project::getData() const
{
    return _data;
}

} } // namespace of::Sketch
