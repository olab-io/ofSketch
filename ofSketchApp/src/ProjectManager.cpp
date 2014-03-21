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


#include "ProjectManager.h"


namespace of {
namespace Sketch {


ProjectManager::ProjectManager(const std::string& path):
    _path(path)
{
    _projectWatcher.addPath(_path);

    std::vector<std::string> files;

    ofx::IO::DirectoryFilter filter;

    ofx::IO::DirectoryUtils::list(ofToDataPath(_path),
                                  files,
                                  true,
                                  &filter);

    std::vector<std::string>::iterator iter = files.begin();

    while(iter != files.end())
    {
        cout << *iter << endl;
        ++iter;
    }

}

ProjectManager::~ProjectManager()
{
}

//Project ProjectManager::newProject(std::string& name)
//{
//
//    return Project("");
//}


std::vector<Project> ProjectManager::getProjects() const
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
