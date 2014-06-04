// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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


#include "ProjectManager.h"

using namespace ofx;

namespace of {
namespace Sketch {


ProjectManager::ProjectManager(const std::string& path):
    _path(path),
    _templateProject(ofToDataPath("Resources/Templates/NewProject", true))
{
    ofLogNotice("ProjectManager::ProjectManager") << "_path: " <<_path;
    _projectWatcher.addPath(_path);

    std::vector<std::string> files;

    ofx::IO::DirectoryFilter filter;

    ofx::IO::DirectoryUtils::list(ofToDataPath(_path, true),
                                  files,
                                  true,
                                  &filter);

    std::vector<std::string>::iterator iter = files.begin();

    while(iter != files.end())
    {
        ofLogVerbose("ProjectManager::ProjectManager") << *iter;
        _projects.push_back(Project(*iter));
        ++iter;
    }
    
}
    
ProjectManager::~ProjectManager()
{
}

// can this function be: return getProjectRef(projectName); only?
const Project& ProjectManager::getProject(const std::string& projectName) const
{
    for (int i = 0; i < _projects.size(); i++) {
        if (_projects[i].getName() == projectName) {
            return _projects[i];
        }
    }
    
    return _projects[0]; //fallback
}
    
Project& ProjectManager::getProjectRef(const std::string& projectName)
{
    for (int i = 0; i < _projects.size(); i++) {
        if (_projects[i].getName() == projectName) {
            return _projects[i];
        }
    }
    
    return _projects[0]; //fallback
}
    
const std::vector<Project>& ProjectManager::getProjects() const
{
    return _projects;
}

//    std::string sketchPath = ofToDataPath("HelloWorldSketch/src/main.cpp",true);
//
//    ofBuffer buffer = ofBufferFromFile(sketchPath);
//
//    Json::Value setEditorSource;
//    setEditorSource["method"] = "setEditorSource";
//    Json::Value editorSource;
//    editorSource["source"] = buffer.getText();
//    setEditorSource["data"] = editorSource;
//
//    cout << setEditorSource.toStyledString() << endl;
//
//    evt.getConnectionRef().sendFrame(WebSocketFrame(setEditorSource.toStyledString()));
//
//    cout << "Connection opened from: " << evt.getConnectionRef().getClientAddress().toString() << endl;

void ProjectManager::getProjectList(const void* pSender, JSONRPC::MethodArgs& args)
{
    Json::Value projectList;
    for (int i = 0; i < _projects.size(); i++) {
        projectList[i]["projectName"] = _projects[i].getName();
    }
    args.result = projectList;
    ofLogNotice("Project::getProjectList") << "Project list requested";
}
    
void ProjectManager::loadProject(const void* pSender, JSONRPC::MethodArgs& args)
{
    if (args.params.isMember("projectName")){
        std::string projectName = args.params["projectName"].asString();
        if (projectExists(projectName)) {
            for (int i = 0; i < _projects.size(); i++) {
                if (_projects[i].getName() == projectName) {
                    Project& project = _projects[i];
                    if (!project.isLoaded()) {
                        project.load(project.getPath(), projectName);
                    }
                    args.result = project.getData();
                    ofLogNotice("Project::loadProject") << "Loaded " << projectName << " project";
                    return;
                }
            }
            ofLogError("Project::loadProject") << "Project: "<< projectName << " was not found";
        }
        else
        {
            ofLogError("Project::loadProject") << "Error loading project";
        }
    }
    else
    {
        ofLogError("Project::loadProject") << "ProjectName is not a member.";
    }
}
    
void ProjectManager::loadTemplateProject(const void *pSender, JSONRPC::MethodArgs &args)
{
    
    if (!_templateProject.isLoaded()) {
        _templateProject.load(_templateProject.getPath(), _templateProject.getName());
    }
    args.result = _templateProject.getData();
    ofLogNotice("Project::loadTemplateProject") << "Loaded a template project";
}
    
void ProjectManager::saveProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    if (args.params.isMember("projectData")) {
        Json::Value projectData = args.params["projectData"];
        std::string projectName = projectData["projectFile"]["name"].asString();
        if (projectExists(projectName)) {
            Project& project = getProjectRef(projectName);
            project.save(projectData);
        }
        ofLogNotice("ProjectManager::saveProject") << "Saved " << projectName << " project";
        
    } else args.error = "A projectData object was not sent";
}

void ProjectManager::createProject(const void* pSender, ofx::JSONRPC::MethodArgs& args)
{
    Json::Value projectData = args.params["projectData"];
    std::string projectName = args.params["projectData"]["projectFile"]["name"].asString();
    ofDirectory projectDir(_templateProject.getPath());
    projectDir.copyTo(_path + "/" + projectName);
    ofFile templateProjectFile(_path + "/" + projectName + "/sketch/NewProject.sketch");
    templateProjectFile.remove();
    
    Project project(_path + "/" + projectName);
    project.save(projectData);
    _projects.push_back(project);
    args.result = project.getData();
    ofLogNotice("Project::createProject") << "Created " << projectName << " project";
}
    
void ProjectManager::deleteProject(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    std::string projectName = args.params["projectName"].asString();
    Project& project = getProjectRef(projectName);
    project.remove();

    for (int i = 0; i < _projects.size(); i++) {
        if (_projects[i].getName() == projectName) {
            _projects.erase(_projects.begin() + i);
            args.result["message"] = "Deleted " + projectName + " project.";
            ofLogNotice("Project::deleteProject") << "Deleted " << projectName << " project";
            return;
        }
    }
    args.error["message"] = "Error deleting " + projectName + " project.";
    ofLogError("Project::deleteProject") << "Error deleting " << projectName << " project";
}
    
void ProjectManager::renameProject(const void *pSender, ofx::JSONRPC::MethodArgs &args)
{
    std::string projectName = args.params["projectName"].asString();
    std::string newProjectName = args.params["newProjectName"].asString();
    Project& project = getProjectRef(projectName);
    if (project.rename(newProjectName)) {
        args.result["message"] = "Renamed " + projectName + " project to " + newProjectName + ".";
        ofLogNotice("Project::renameProject") << "Renamed " << projectName << " project to " << newProjectName;
    } else {
        args.error["message"] = "Error renaming " + projectName + " project.";
        ofLogError("Project::renameProject") << "Error renaming " << projectName << " project to " << newProjectName;
    }
}

bool ProjectManager::projectExists(const std::string& projectName) const
{
    for (int i = 0; i < _projects.size(); i++) {
        if (projectName == _projects[i].getName())
            return true;
    }
    
    return false;
}
    
void ProjectManager::reloadProjects()
{
    
}
    
void ProjectManager::updateProject(const std::string& projectName)
{
    
}

void ProjectManager::onDirectoryWatcherItemAdded(const Poco::DirectoryWatcher::DirectoryEvent& evt)
{
    ofSendMessage("Added:    " + evt.item.path());
}


void ProjectManager::onDirectoryWatcherItemRemoved(const Poco::DirectoryWatcher::DirectoryEvent& evt)
{
    ofSendMessage("Removed:  " + evt.item.path());
}


void ProjectManager::onDirectoryWatcherItemModified(const Poco::DirectoryWatcher::DirectoryEvent& evt)
{
    ofSendMessage("Modified: " + evt.item.path());
}


void ProjectManager::onDirectoryWatcherItemMovedFrom(const Poco::DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("ofApp::onDirectoryWatcherItemMovedFrom") << "Moved From: " << evt.item.path();
}


void ProjectManager::onDirectoryWatcherItemMovedTo(const Poco::DirectoryWatcher::DirectoryEvent& evt)
{
    ofLogNotice("ofApp::onDirectoryWatcherItemMovedTo") << "Moved To: " << evt.item.path();
}


void ProjectManager::onDirectoryWatcherError(const Poco::Exception& exc)
{
    ofLogError("ofApp::onDirectoryWatcherError") << "Error: " << exc.displayText();
}


} } // namespace of::Sketch
