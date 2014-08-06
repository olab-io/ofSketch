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
#include "ofUtils.h"


namespace of {
namespace Sketch {


const std::string Project::SKETCH_FILE_EXTENSION = "sketch";


Project::~Project()
{
}


Project::Project(const std::string& path): _path(path), _isLoaded(false)
{
    // this is not efficient at all! I am just keeping these FileTemplate loads in the project
    // constructor because it makes the most sense architecure wise.
    _classFileTemplate = ofBufferFromFile(ofToDataPath("Resources/Templates/SketchTemplates/class.tmpl")).getText();
    load(_path, getName());
}

void Project::load(const std::string& path, const std::string& name)
{
    _sketchDir = ofDirectory(ofToDataPath(path + "/sketch"));
    
    _data.clear();
    
    if (_sketchDir.exists()) 
    {
        _sketchDir.listDir();

        std::vector<ofFile> files = _sketchDir.getFiles();

        int classCounter = 0;

        for (std::size_t i = 0; i < files.size(); ++i) 
        {
            ofFile file = files[i];

            if (file.getBaseName() == name)
            {
                file.open(file.getAbsolutePath());
                _data["projectFile"]["name"] = file.getBaseName();
                _data["projectFile"]["fileName"] = file.getFileName();
                _data["projectFile"]["fileContents"] = file.readToBuffer().getText();
            } 
            else if (file.getExtension() == SKETCH_FILE_EXTENSION)
            {
                file.open(file.getAbsolutePath());
                _data["classes"][classCounter]["name"] = file.getBaseName();
                _data["classes"][classCounter]["fileName"] = file.getFileName();
                _data["classes"][classCounter]["fileContents"] = file.readToBuffer().getText();
                classCounter++;
            }
        }
        
        _loadAddons();
        _isLoaded = true;
    }
}


bool Project::isLoaded() const
{
    return _isLoaded;
}

// saves differences only
void Project::save(const Json::Value& data)
{
    // this is not working for some reason...
    if (_data != data)
    {
        ofLogVerbose("Project::save") << data.toStyledString();
        
        if (_data["projectFile"] != data["projectFile"])
        {
            _data["projectFile"] = data["projectFile"];
            _saveFile(_data["projectFile"]);
        }
        
            
        // uses nested for loop in case classes are not in the same order
        std::vector<Json::Value> newClasses;
        std::vector<Json::Value> deletedClasses;
        

        for (unsigned int i = 0; i < _data["classes"].size(); ++i)
        {
            Json::Value& classFile = _data["classes"][i];

            bool matchFound = false;
            
            for (unsigned int j = 0; j < data.size(); ++j)
            {
                const Json::Value& newClassFile = data["classes"][j];
                
                if (classFile["fileName"] == newClassFile["fileName"]) 
                {
                    if (classFile != newClassFile)
                    {
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
    }
}


bool Project::create(const std::string& path)
{
    ofDirectory project(ofToDataPath(path));

    if (!project.exists()) 
    {
        ofDirectory temp(ofToDataPath("Resources/Templates/SimpleTemplate"));
        temp.copyTo(ofToDataPath(path));
        return true;
    }
    
    return false;
}


bool Project::remove()
{
    ofDirectory projectDir(getPath());
    return projectDir.remove(true);
}


bool Project::rename(const std::string& newName)
{
    if (isLoaded()) 
    {
        ofLogVerbose("Project::rename") << "renaming project \"" << getName() << "\" to \"" << newName + "\"";

        ofFile projectDir(getPath());

        std::string oldProjectName = getName();

        ofLogVerbose("Project::rename") << "project path: " << getPath() << " Renamed to: " << projectDir.getEnclosingDirectory() + newName;

        if (!projectDir.renameTo(projectDir.getEnclosingDirectory() + newName)) return false;

        _path = projectDir.getAbsolutePath();

        ofFile projectFile(projectDir.getAbsolutePath() + "/sketch/" + oldProjectName + "." + SKETCH_FILE_EXTENSION);
        
        
        // remove old executable
        ofDirectory bin;
        bin.listDir(projectDir.path() + "/bin");
        std::vector<ofFile> files = bin.getFiles();
        
        Poco::RegularExpression appExpression( oldProjectName + "(.exe|.app)*$", Poco::RegularExpression::RE_ANCHORED);

        for (int i = 0; i < files.size(); i++)
        {
            std::string baseName = files[i].getBaseName();
            if (appExpression.match(baseName)) {
                files[i].remove(true);
                ofLogVerbose("Project::rename")
                    << "removed " << files[i].getFileName() << " from " << bin.getAbsolutePath() << endl;
            }
        }
        

        ofLogVerbose("Project::rename") << "projectDir path after rename: " << projectDir.getAbsolutePath();
        ofLogVerbose("Project::rename") << "projectFile path: " << projectFile.getAbsolutePath();

        if (!projectFile.renameTo(projectDir.getAbsolutePath() + "/sketch/" + newName + "." + SKETCH_FILE_EXTENSION)) return false;

        ofLogVerbose("Project::rename") << "projectFile path after rename: " << projectFile.getAbsolutePath();

        _data["projectFile"]["name"] = newName;

        _data["projectFile"]["fileName"] = newName + "." + SKETCH_FILE_EXTENSION;

        return true;
    }
    else
    {
        ofLogVerbose("Project::rename") << "Cannot rename project, it is not loaded.";
        return false;
    }
    
}


Json::Value Project::createClass(const std::string& className)
{
    std::string fileContents = _classFileTemplate;
    ofStringReplace(fileContents, "<classname>", className);

    ofLogVerbose("Project::createClass") << "fileContents: "<< fileContents;

    Json::Value classFile;
    // TODO: Load extension from settings
    classFile["fileName"] = className + "." + SKETCH_FILE_EXTENSION;
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
    if (isLoaded())
    {
        ofFile file(_sketchDir.getAbsolutePath() + "/" + className + "." + SKETCH_FILE_EXTENSION);

        if (file.exists())
        {
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
    if (isLoaded())
    {
        ofLogVerbose("Project::renameClass") << "Renaming class...";

        ofFile file(_sketchDir.getAbsolutePath() + "/" + currentName + "." + SKETCH_FILE_EXTENSION);

        if (file.exists() && hasClasses()) 
        {
            unsigned int numClasses = getNumClasses();

            for (unsigned int i = 0; i < numClasses; ++i)
            {
                if (_data["classes"][i]["name"] == currentName)
                {
                    _data["classes"][i]["name"] = newName;
                    _data["classes"][i]["fileName"] = newName + "." + SKETCH_FILE_EXTENSION;

                    file.renameTo(_sketchDir.getAbsolutePath() + "/" + newName + "." + SKETCH_FILE_EXTENSION);
                    return true;
                }
            }
        }
    }

    return false;
}


bool Project::hasClasses() const
{
    return !_data["classes"].empty();
}


bool Project::isClassName(const std::string& className) const
{
    if (hasClasses()) 
    {
        unsigned int numClasses = getNumClasses();

        for (unsigned int i = 0; i < numClasses; ++i)
        {
            if (_data["classes"][i]["name"] == className)
            {
                return true;
            }
        }
    }
    
    return false;
}

    
unsigned int Project::getNumClasses() const
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


const Json::Value& Project::getData() const
{
    return _data;
}
    
void Project::addAddon(std::string& addon)
{
    if (!usingAddon(addon)) {
        _addons.push_back(addon);
        _saveAddons();
    }
}
    
bool Project::removeAddon(std::string& addon)
{
    for (unsigned int i = 0; i < _addons.size(); i++)
    {
        if (addon == _addons[i]) {
            _addons.erase(_addons.begin() + i);
            _saveAddons();
            return true;
        }
    }
    
    return false;
}
  
bool Project::hasAddons() const
{
    return _addons.size() > 0;
}
    
bool Project::usingAddon(std::string& addon) const
{
    for (unsigned int i = 0; i < _addons.size(); i++)
    {
        if (addon == _addons[i]) {
            return true;
        }
    }
    
    return false;
}
    
std::vector<std::string> Project::getAddons() const
{
    return _addons;
}
    
void Project::_loadAddons()
{
    _addons.clear();
    ofFile addonsMakefile(_path + "/addons.make");
    
    if (addonsMakefile.exists()) {
        
        std::vector<std::string> lines = ofSplitString(ofBufferFromFile(addonsMakefile.path()), "\n");
        
        for (unsigned int i = 0; i < lines.size(); i++) {
            
            if (lines[i] != "") {
                
                _addons.push_back(lines[i]);
            }
        }
    }
}
    
void Project::_saveAddons()
{
    ofBuffer buffer(ofJoinString(_addons, "\n"));
    ofBufferToFile(_path + "/addons.make", buffer);
}

void Project::_saveFile(const Json::Value& fileData)
{
    ofBuffer fileBuffer(fileData["fileContents"].asString());
    ofBufferToFile(getPath() + "/sketch/" + fileData["fileName"].asString(), fileBuffer);
}

} } // namespace of::Sketch
