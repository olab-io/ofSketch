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


#include "UploadRouter.h"

namespace of {
namespace Sketch {
    
    
UploadRouter::UploadRouter(const std::string& projectDir):
    _path(projectDir)
{
    
}
    
bool UploadRouter::onHTTPPostEvent(ofx::HTTP::PostEventArgs& args)
{
    ofLogNotice("UploadManager::onHTTPPostEvent") << "Data: " << args.getBuffer().getText();
    return false;
}


bool UploadRouter::onHTTPFormEvent(ofx::HTTP::PostFormEventArgs& args)
{
    
    const Poco::Net::NameValueCollection& form = args.getForm();
    std::string projectName = form["projectName"];
    
    UploadRouter::UploadedFile uploadedFile = _uploadedFiles[args.getPostId().toString()];
    ofFile tempFile(uploadedFile.tempFilename);
    
    ofLogNotice("UploadRouter::onHTTPFormEvent") << "Project path" << _path + "/" + projectName << endl;
    ofDirectory project(_path + "/" + projectName);
    
    if (project.exists()) {
        
        tempFile.renameTo(project.getAbsolutePath() + "/bin/data/" + uploadedFile.filename);
        ofx::HTTP::Utils::dumpNameValueCollection(args.getForm(), ofGetLogLevel());
    }
    
//    Json::Value json;
//    
//    json["files"][0]["name"] = "picture1.jpg";
//    json["files"][0]["size"] = 902604;
//    json["files"][0]["url"] = "http://example.org/files/picture1.jpg";
//    json["files"][0]["thumbnailUrl"] = "http://example.org/files/thumbnail/picture1.jpg";
//    json["files"][0]["deleteUrl"] = "http://example.org/files/picture1.jpg";
//    json["files"][0]["deleteType"] = "DELETE";
//    
//    ofBuffer buffer(json.asString());
//    std::string string(json.asString());
//    args.response.sendBuffer(&string, string.size());
    
    return true;
}


bool UploadRouter::onHTTPUploadEvent(ofx::HTTP::PostUploadEventArgs& args)
{
    std::string stateString = "";
    
    switch (args.getState())
    {
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_STARTING:
            stateString = "STARTING";
            break;
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_PROGRESS:
            stateString = "PROGRESS";
            break;
        case ofx::HTTP::PostUploadEventArgs::UPLOAD_FINISHED:
            stateString = "FINISHED";
            break;
    }
    
    if (stateString == "FINISHED") {

        UploadRouter::UploadedFile file;
        
        file.tempFilename = args.getFilename();
        file.filename = args.getOriginalFilename();
        file.type = args.getFileType().toString();
        
        _uploadedFiles[args.getPostId().toString()] = file;

        ofLogNotice("UploadManager::onHTTPUploadEvent") << "Post ID: " << args.getPostId().toString() << endl;
        ofLogNotice("UploadManager::onHTTPUploadEvent") << "FINISHED";
        ofLogNotice("UploadManager::onHTTPUploadEvent") << "";
        ofLogNotice("UploadManager::onHTTPUploadEvent") << "         state: " << stateString;
        ofLogNotice("UploadManager::onHTTPUploadEvent") << " formFieldName: " << args.getFormFieldName();
        ofLogNotice("UploadManager::onHTTPUploadEvent") << "orig. filename: " << args.getOriginalFilename();
        ofLogNotice("UploadManager::onHTTPUploadEvent") <<  "      filename: " << args.getFilename();
        ofLogNotice("UploadManager::onHTTPUploadEvent") <<  "      fileType: " << args.getFileType().toString();
        ofLogNotice("UploadManager::onHTTPUploadEvent") << "# bytes xfer'd: " << args.getNumBytesTransferred();
    }
    
    return true;
}

    
} } // namespace of::Sketch
