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


#pragma once


#include <string>
#include "Poco/URI.h"
#include "ProjectFile.h"
#include "ofxJSONElement.h"
#include "ofMain.h"


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
    
    bool create(const std::string& path);
    bool rename(const std::string& name);
    bool save(ofxJSONElement json);
    bool load(const std::string path,
              const std::string& name);
    bool isLoaded();
    
    ofxJSONElement getJson();
    
protected:


private:
    
    std::string _path;
    std::vector<Source> _sources;
    
    bool _isLoaded;
    ofxJSONElement _json;

};


} } // namespace of::Sketch
