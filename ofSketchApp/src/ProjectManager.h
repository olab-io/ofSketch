// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#pragma once


#include <vector>
#include <string>
#include "json/json.h"
#include "ofx/IO/DirectoryUtils.h"
#include "ofx/IO/DirectoryFilter.h"
#include "ofx/IO/DirectoryWatcherManager.h"
#include "ofx/IO/PathFilterCollection.h"
#include "ofx/IO/RegexPathFilter.h"
#include "ofx/JSONRPC/MethodArgs.h"
#include "ofx/JSONRPC/JSONRPCUtils.h"
#include "Project.h"
#include "Settings.h"
#include "FileFilters.h"


namespace of {
namespace Sketch {


class ProjectManager
{
public:
    ProjectManager(Settings& settings);

    virtual ~ProjectManager();

    void setup();

	/// \brief Get a pointer to a Project by name.
	///
	/// The ProjectManager retains ownership of the project.
	///
	/// \param name The name of the Project to get.
	/// \returns a pointer to the Project or a nullptr if not found.
	Project* getProject(const std::string& name);

	/// \brief Get a pointer to a Project by name.
	///
	/// The ProjectManager retains ownership of the project.
	///
	/// \param name The name of the Project to get.
	/// \returns a pointer to the Project or a nullptr if not found.
    const Project* getProject(const std::string& projectName) const;

	/// \brief Get a list of the Projects.
	/// \returns a list of the Projects.
	const std::vector<Project>& projects() const;

	const Project& getTemplateProject() const;

//    void loadProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
//    void loadTemplateProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
//    void saveProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
//    void createProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
//    void deleteProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
//    void renameProject(const void* pSender, ofx::JSONRPC::MethodArgs& args);
//
//    void notifyProjectClosed(const std::string& projectName);
//    void reloadProjects();
//    void updateProject(const std::string& projectName);
//

	void onDirectoryWatcherItemAdded(const ofx::DirectoryWatcher::DirectoryEvent& evt);
	void onDirectoryWatcherItemRemoved(const ofx::DirectoryWatcher::DirectoryEvent& evt);
	void onDirectoryWatcherItemModified(const ofx::DirectoryWatcher::DirectoryEvent& evt);
	void onDirectoryWatcherItemMovedFrom(const ofx::DirectoryWatcher::DirectoryEvent& evt);
	void onDirectoryWatcherItemMovedTo(const ofx::DirectoryWatcher::DirectoryEvent& evt);
	void onDirectoryWatcherError(const Poco::Exception& exc);


private:
    Settings& _settings;

	ofx::IO::DirectoryWatcherManager _projectWatcher;

	ProjectFilter _projectFilter;

	std::vector<Project> _projects;

	mutable Project _templateProject;


//    std::vector<Project> _examples;


//    std::vector<std::string> _openProjectNames;
//    std::vector<Project> _projects;
//
//
//    bool _removeFromOpenProjectNames(const std::string& projectName);
};


} } // namespace of::Sketch
