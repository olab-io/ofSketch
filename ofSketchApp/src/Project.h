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


#pragma once


#include <string>
#include <json/json.h>
#include "Poco/URI.h"
#include "ofTypes.h"
#include "ofFileUtils.h"


namespace of {
namespace Sketch {


class Project
{
public:
    typedef std::shared_ptr<Project> SharedPtr;
    typedef std::weak_ptr<Project> WeakPtr;

    Project(const std::string& path);
    ~Project();

    const std::string& getPath() const;
    std::string getName() const;
    bool isLoaded() const;
    bool hasClasses() const;
    
    bool create(const std::string& path);
    bool remove();
    bool rename(const std::string& newName);
    void save(const Json::Value& data);
    void load(const std::string& path,
              const std::string& name);

    bool deleteClass(const std::string& className);
    bool renameClass(const std::string& currentName,
                     const std::string& newName);

    bool isClassName(const std::string& className) const;
    
    unsigned int getNumClasses() const;

    Json::Value createClass(const std::string& className);
    const Json::Value& getData() const;

    static const std::string SKETCH_FILE_EXTENSION;

private:
    std::string _path;
    std::string _classFileTemplate;
    ofDirectory _sketchDir;

    bool _isLoaded;
    Json::Value _data;

    void _saveFile(const Json::Value& fileData);

};


} } // namespace of::Sketch
