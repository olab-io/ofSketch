// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
//               2014 Brannon Dorsey <http://brannondorsey.com>
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
    // this is not efficient at all! I am just keeping these FileTemplate loads in the project
    // constructor because it makes the most sense architecure wise.
    _classFileTemplate = ofBufferFromFile(ofToDataPath("Resources/Templates/SketchTemplates/class.txt")).getText();
    load(_path, getName());
}

void Project::load(const std::string path, const std::string& name)
{
    _sketchDir = ofDirectory(ofToDataPath(path + "/sketch"));
    _data.clear();
    if (_sketchDir.exists()) {
        _sketchDir.listDir();
        std::vector<ofFile> files = _sketchDir.getFiles();
        int classCounter = 0;
        for (int i = 0; i < files.size(); i++) {
            ofFile file = files[i];
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
        _isLoaded = true;
    }
}
    
bool Project::isLoaded() const
{
    return _isLoaded;
}

// saves differences only
void Project::save(ofxJSONElement data){
    
    // this is not working for some reason...
    if (_data != data) {
        
        ofLogVerbose("Project::save") << data.toStyledString();
        
        if (_data["projectFile"] != data["projectFile"]) {
            _data["projectFile"] = data["projectFile"];
            _saveFile(_data["projectFile"]);
        }
        
            
        // uses nested for loop in case classes are not in the same order
        std::vector<Json::Value> newClasses;
        std::vector<Json::Value> deletedClasses;
        

        for (int i = 0; i < _data["classes"].size(); i++) {
            
            Json::Value& classFile = _data["classes"][i];
            bool matchFound = false;
            
            for (int j = 0; j < data.size(); j++) {
                
                Json::Value& newClassFile = data["classes"][j];
                
                if (classFile["fileName"] == newClassFile["fileName"]) {
                    
                    if (classFile != newClassFile) {
                        classFile = newClassFile;
                        _saveFile(classFile);
                    }
                   
                    matchFound = true;
                    break;
                }
            }
            
            if (!matchFound) _saveFile(classFile); // class is new
        }
        
    
    }
    else
    {
        ofLogNotice("Project::save") << "Project data is the same. Not saving project.";
        return true;
    }
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
    
bool Project::remove()
{
    ofDirectory project(_path);
    return project.remove(true);
}

bool Project::rename(const std::string& name)
{
    
}

Json::Value Project::createClass(const std::string& className)
{

    std::string fileContents = _classFileTemplate;
    ofStringReplace(fileContents, "<classname>", className);

    ofLogVerbose("Project::createClass") << "fileContents: "<< fileContents;

    Json::Value classFile;
    // TODO: Load extension from settings
    classFile["fileName"] = className + ".sketch";
    classFile["name"] = className;
    classFile["fileContents"] = fileContents;
    _data["classes"][getNumClasses()] = classFile;
    // TODO: re-loading is a terribly slow way to delete. Come back and optimize.
    // Simply need to remove the Json::Value class in _data["classes"]
    _saveFile(classFile);
    return classFile;
}
    
bool Project::deleteClass(const std::string& className)
{
    if (isLoaded()) {
        ofFile file(_sketchDir.getAbsolutePath() + "/" + className + ".sketch");
        if (file.exists()) {
            file.remove();
            // TODO: re-loading is a terribly slow way to delete. Come back and optimize.
            // Simply need to remove the Json::Value class in _data["classes"]
            load(_path, getName());
            return true;
        }
    }
    
    return false;
}
    
bool Project::renameClass(const std::string& currentName, const std::string& newName)
{
    if (isLoaded()) {
        ofLogVerbose("Project::createClass") << "Renaming class...";

        cout<<"Renaming class..."<<endl;
        ofFile file(_sketchDir.getAbsolutePath() + "/" + currentName + ".sketch");
        if (file.exists() && hasClasses()) {
            for (int i = 0; i < getNumClasses(); i++) {
                if (_data["classes"][i]["name"] == currentName) {
                    _data["classes"][i]["name"] = newName;
                    _data["classes"][i]["fileName"] = newName + ".sketch";

                    ofLogVerbose("Project::createClass") <<"NEW NAME DATA IS "<<newName;

                    file.renameTo(_sketchDir.getAbsolutePath() + "/" + newName + ".sketch");
                    return true;
                }
            }
        }
    }
    
    return false;
}
    
bool Project::hasClasses() const
{
    return _data["classes"].size() > 0;
}
    
bool Project::isClassName(const std::string& className) const
{
    if (hasClasses()) {
        for (int i = 0; i < getNumClasses(); i++) {
            if (_data["classes"][i]["name"] == className) {
                return true;
            }
        }
    }
    
    return false;
}
    
int Project::getNumClasses() const
{
    return _data["classes"].size();
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

void Project::_saveFile(const Json::Value& fileData)
{
    ofBuffer fileBuffer(fileData["fileContents"].asString());
    ofBufferToFile(getPath() + "/sketch/" + fileData["fileName"].asString(), fileBuffer);
}

} } // namespace of::Sketch
