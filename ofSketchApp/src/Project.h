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
#include "json/json.h"
#include "Poco/Path.h"
#include "Poco/RegularExpression.h"
#include "Poco/URI.h"
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
	struct File;

	/// \brief Create a Project using the Project's absolute path.
	Project(const std::string& path);

	/// \brief Destroy the Project.
    ~Project();

    /// \brief Get the absolute path of the Project.
    /// \returns the absolute path of the Project.
    const Poco::Path& path() const;

	Poco::Path sketchPath() const;

	/// \brief Get the name of the Project.
	/// \returns the name of the Project.
	const std::string& name() const;

	/// \brief Determine if the Project is loaded.
	/// \returns true iff the Project is loaded.
	bool isLoaded() const;

	/// \brief Load the Project from the Project path.
	void load();

//    bool hasClasses() const;
//    bool hasAddons() const;
//    bool usingAddon(std::string& addon) const;
//
//    bool create(const std::string& path);
//    bool remove();
//    bool rename(const std::string& newName);
//
//    void save(const Json::Value& data);
//    void load(const std::string& path,
//              const std::string& name);
//
//    void addAddon(std::string& addon);
//    bool removeAddon(std::string& addon);
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
//    std::vector<std::string> getAddons() const;
//
//    const Json::Value& getData() const;
//



	static std::vector<std::string> parseAddonsMake(const std::string& projectPath);

private:
	/// \brief The absolute path of the project.
	Poco::Path _path;

	/// \brief The project's name.
    std::string _name;

	/// \brief True if the project is loaded.
	bool _isLoaded;

	/// \brief True if the project is read-only.
	bool _readOnly;

	/// \brief A collection of addon names.
	std::vector<std::string> _addons;

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
