// =============================================================================
//
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
#include <json/json.h>
#include "Poco/Net/NameValueCollection.h"
#include "ofxJSONElement.h"
#include "ofxHTTP.h"

namespace of {
namespace Sketch {


class UploadRouter
{
public:
    UploadRouter(const std::string& projectDir);
    
    bool onHTTPPostEvent(ofx::HTTP::PostEventArgs& args);
    bool onHTTPFormEvent(ofx::HTTP::PostFormEventArgs& args);
    bool onHTTPUploadEvent(ofx::HTTP::PostUploadEventArgs& args);
    
    struct UploadedFile
    {
        std::string tempFilename;
        std::string filename;
        std::string type;
    };
    
private:
    std::string _path;
    std::map<std::string, UploadRouter::UploadedFile> _uploadedFiles;

};


} } // namespace of::Sketch
