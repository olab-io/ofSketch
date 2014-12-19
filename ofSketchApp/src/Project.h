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


#pragma once


#include <string>
#include "json/json.h"
#include "Poco/Path.h"
#include "Poco/RegularExpression.h"
#include "Poco/URI.h"
#include "ofTypes.h"
#include "ofFileUtils.h"


namespace of {
namespace Sketch {


class Project
{
public:
    Project(const std::string& path);

    virtual ~Project();

    /// \brief Get the name of the project.
    /// \returns the name of the project.
    const std::string& getName() const;

    /// \brief Get the absolute path of the project.
    /// \returns the absolute path of the project.
    const Poco::Path& getPath() const;


//    bool isLoaded() const;
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
//    static const std::string SKETCH_FILE_EXTENSION;

private:
    std::string _name; ///< The project name.

    Poco::Path _path; ///< The absolute path of the project.


//    Settings& _settings;
//
//    bool _readOnly;
//
//    std::string _path;
//    std::string _classFileTemplate;
//    std::vector<std::string> _addons;
//    ofDirectory _sketchDir;
//
//    bool _isLoaded;
//    Json::Value _data;
//
//    void _saveFile(const Json::Value& fileData);
//    void _loadAddons();
//    void _saveAddons();

};


} } // namespace of::Sketch
