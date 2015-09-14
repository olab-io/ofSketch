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


#include <string>
#include <unordered_set>
#include "json/json.h"
#include "ofx/IO/FileExtensionFilter.h"
#include "ofTypes.h"
#include "ofFileUtils.h"
#include "FileFilters.h"
#include "SourceFile.h"


namespace of {
namespace Sketch {


/// \brief A Project represents an ofSketch Project.
///
/// ofSketch projects are constructed by examining the user's Projects folder.
///
/// Projects remain "unloaded" until explicitly loaded.
class Project
{
public:
	/// \brief Create a Project using the Project's absolute path.
	Project(const std::string& path);

	/// \brief Destroy the Project.
    ~Project();

    /// \brief Get the absolute path of the Project.
    /// \returns the absolute path of the Project.
	const std::string& path() const;

	std::string sketchPath() const;

	/// \brief Get the name of the Project.
	/// \returns the name of the Project.
	const std::string& name() const;

	/// \brief Determine if the Project is loaded.
	/// \returns true iff the Project is loaded.
	bool isLoaded() const;

	/// \brief Load the Project from the Project path.
	void load();

	/// \brief Determine if any addons are being used.
	/// \returns true if any addons are being used.
	bool hasAddons() const;

	/// \brief Determine if an addon is being used.
	/// \param addon The addon to check.
	bool isUsingAddon(const std::string& addon) const;

	/// \brief Add an addon.
	/// \param addon The addon to add.
    void addAddon(const std::string& addon);

	/// \brief Remove an addon.
	/// \param addon The addon to remove.
	void removeAddon(const std::string& addon);

	/// \brief Get a list of the active addons.
	/// \returns a list of the active addons.
	const std::unordered_set<std::string>& addons() const;

	const std::vector<SourceFile>& sourceFiles() const;



//	void addSource


//    bool hasClasses() const;
//
//    bool create(const std::string& path);
//    bool remove();
//    bool rename(const std::string& newName);
//
//    void save(const Json::Value& data);
//    void load(const std::string& path,
//              const std::string& name);
//
//
//    bool deleteClass(const std::string& className);
//    bool renameClass(const std::string& currentName,
//                     const std::string& newName);
//
//    bool isClassName(const std::string& className) const;
//
//    unsigned int getNumClasses() const;
//
//    Json::Value createClass(const std::string& className);
//
//
//    const Json::Value& getData() const;
//


	/// \brief Parse the addons.make file for a list of the addons.
	/// \param projectPath A path to the project where the addons.make should be found.
	/// \returns a set of addon names / paths strings.
	/// \todo This method and the writing method currently ignore and remove
	/// commented lines, spaces, etc.  Some users may want a "smarter" system.
	static std::unordered_set<std::string> parseAddonsMake(const std::string& projectPath);

private:
	/// \brief The absolute path of the project.
	std::string _path;

	/// \brief The project's name.
    std::string _name;

	/// \brief True if the project is loaded.
	bool _isLoaded;

	/// \brief True if the project is read-only.
	bool _readOnly;

	/// \brief A collection of addon names.
	std::unordered_set<std::string> _addons;

	of::Sketch::ProjectFileFilter _projectFileFilter;

	std::vector<SourceFile> _sourceFiles;


//    Settings& _settings;
//
//
//    std::string _classFileTemplate;
//
//    Json::Value _data;
//

	void _loadAddons();

	void _saveAddons();
    void _saveFile(const Json::Value& fileData);

};


} } // namespace of::Sketch
