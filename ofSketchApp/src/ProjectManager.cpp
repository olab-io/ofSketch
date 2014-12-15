// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
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


#include "ProjectManager.h"


namespace of {
namespace Sketch {


ProjectManager::ProjectManager(Settings& settings):
    _settings(settings),
    _templateProject(Poco::Path(_settings.getPathsRef().getTemplatesPath(), Poco::Path::forDirectory("SimpleProject")).toString())
{
}


ProjectManager::~ProjectManager()
{
}

    
void ProjectManager::setup()
{
    std::vector<std::string> files;

    ofx::IO::DirectoryFilter filter;

    ofx::IO::DirectoryUtils::list(_settings.getPaths().getProjectsPath().toString(),
                                  files,
                                  true,
                                  &filter);

    std::vector<std::string>::iterator iter = files.begin();

    while (iter != files.end())
    {
        ofLogVerbose("ProjectManager::ProjectManager") << *iter;
        _projects.push_back(Project(*iter));
        ++iter;
    }
}


// can this function be: return getProjectRef(projectName); only?
const Project& ProjectManager::getProject(const std::string& projectName) const
{
    for (std::size_t i = 0; i < _projects.size(); ++i) 
    {
        if (_projects[i].getName() == projectName) 
        {
            return _projects[i];
        }
    }

    return _projects[0]; //fallback
}


Project& ProjectManager::getProjectRef(const std::string& projectName)
{
    for (std::size_t i = 0; i < _projects.size(); ++i) 
    {
        if (_projects[i].getName() == projectName) 
        {
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


void ProjectManager::getProjectList(const void* pSender,
                                    ofx::JSONRPC::MethodArgs& args)
{
    Json::Value projectList;

    for (unsigned int i = 0; i < _projects.size(); ++i) {
        projectList[i]["projectName"] = _projects[i].getName();
    }

    args.result = projectList;
    ofLogNotice("Project::getProjectList") << "Project list requested";
}


void ProjectManager::loadProject(const void* pSender,
                                 ofx::JSONRPC::MethodArgs& args)
{
    if (args.params.isMember("projectName"))
    {
        std::string projectName = args.params["projectName"].asString();

        if (projectExists(projectName)) 
        {
            for (std::size_t i = 0; i < _projects.size(); ++i) 
            {
                if (_projects[i].getName() == projectName) 
                {
                    Project& project = _projects[i];

                    if (!project.isLoaded()) 
                    {
                        project.load(project.getPath(), projectName);
                    }

                    args.result["data"] = project.getData();
                    args.result["alreadyOpen"] = ofContains(_openProjectNames, projectName);
                    _openProjectNames.push_back(projectName);
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


void ProjectManager::loadTemplateProject(const void *pSender,
                                         ofx::JSONRPC::MethodArgs &args)
{
    if (!_templateProject.isLoaded()) 
    {
        _templateProject.load(_templateProject.getPath(), _templateProject.getName());
    }

    args.result = _templateProject.getData();

    ofLogNotice("Project::loadTemplateProject") << "Loaded a template project.";
}


void ProjectManager::saveProject(const void* pSender,
                                 ofx::JSONRPC::MethodArgs& args)
{
    if (args.params.isMember("projectData")) 
    {
        Json::Value projectData = args.params["projectData"];

        std::string projectName = projectData["projectFile"]["name"].asString();

        if (projectExists(projectName)) 
        {
            Project& project = getProjectRef(projectName);
            project.save(projectData);
        }

        ofLogNotice("ProjectManager::saveProject") << "Saved " << projectName << " project";

    }
    else args.error = "A projectData object was not sent";
}

void ProjectManager::createProject(const void* pSender,
                                   ofx::JSONRPC::MethodArgs& args)
{
    Json::Value projectData = args.params["projectData"];

    std::string projectName = args.params["projectData"]["projectFile"]["name"].asString();

    ofDirectory projectDir(_templateProject.getPath());

    projectDir.copyTo(_settings.getPaths().getProjectsPath().toString() + "/" + projectName);

    ofFile templateProjectFile(_settings.getPaths().getProjectsPath().toString() + "/" + projectName + "/sketch/NewProject." + Project::SKETCH_FILE_EXTENSION);

    templateProjectFile.remove();

    Project project(_settings.getPaths().getProjectsPath().toString() + "/" + projectName);
    project.save(projectData);
    _projects.push_back(project);
    args.result = project.getData();
    ofLogNotice("Project::createProject") << "Created " << projectName << " project";
}

void ProjectManager::deleteProject(const void *pSender,
                                   ofx::JSONRPC::MethodArgs &args)
{
    std::string projectName = args.params["projectName"].asString();
    Project& project = getProjectRef(projectName);
    project.remove();

    for (std::size_t i = 0; i < _projects.size(); ++i)
    {
        if (_projects[i].getName() == projectName) 
        {
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

    if (project.rename(newProjectName)) 
    {
        _removeFromOpenProjectNames(projectName);
        _openProjectNames.push_back(newProjectName);

        args.result["message"] = "Renamed " + projectName + " project to " + newProjectName + ".";
        ofLogNotice("Project::renameProject") << "Renamed " << projectName << " project to " << newProjectName;
    } 
    else 
    {
        args.error["message"] = "Error renaming " + projectName + " project.";
        ofLogError("Project::renameProject") << "Error renaming " << projectName << " project to " << newProjectName;
    }
}


bool ProjectManager::projectExists(const std::string& projectName) const
{
    for (std::size_t i = 0; i < _projects.size(); ++i) 
    {
        if (projectName == _projects[i].getName())
        {
            return true;
        }
    }

    return false;
}


void ProjectManager::reloadProjects()
{
    // TODO:
}


void ProjectManager::updateProject(const std::string& projectName)
{
    // TODO:
}

void ProjectManager::notifyProjectClosed(const std::string& projectName)
{
    _removeFromOpenProjectNames(projectName);
}


bool ProjectManager::_removeFromOpenProjectNames(const std::string& projectName)
{
    for (std::size_t i = 0; i < _openProjectNames.size(); i++)
    {
        if (_openProjectNames[i] == projectName)
        {
            _openProjectNames.erase(_openProjectNames.begin() + i);
            return true;
        }
    }

    return false;
}


} } // namespace of::Sketch
