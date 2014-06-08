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


#include "Compiler.h"


namespace of {
namespace Sketch {


Compiler::Compiler(ProcessTaskQueue& taskQueue, const std::string& pathToTemplates):
    _taskQueue(taskQueue),
    _pathToTemplates(pathToTemplates),
    _projectFileTemplate(ofBufferFromFile(ofToDataPath(_pathToTemplates + "/main.txt")).getText()),
    _classTemplate(ofBufferFromFile(ofToDataPath(_pathToTemplates + "/class.txt")).getText())
{
}


Poco::UUID Compiler::compile(const Project& project)
{
    MakeTask::Settings settings;
    return _taskQueue.start(new MakeTask(settings, project, "Release"));
//    return Poco::UUID::null();
}


Poco::UUID Compiler::run(const Project& project)
{
    MakeTask::Settings settings;

//    // We don't return this handle yet, but it is available in the callback.
//    // Compile task.
//    _taskQueue.start(new MakeTask(settings, project, "Release"));

    // Run task.
    return _taskQueue.start(new MakeTask(settings, project, "RunRelease"));

}


void Compiler::generateSourceFiles(const Project& project)
{
    ofDirectory src(project.getPath() + "/src");
    src.remove(true);
    src.create(true);
    
    Json::Value projectData = project.getData();
    std::string projectFile = _projectFileTemplate;
    ofStringReplace(projectFile, "<projectfile>", projectData["projectFile"]["fileContents"].asString());
    _replaceIncludes(projectFile);
    ofBuffer sourceBuffer(projectFile);
    ofBufferToFile(src.getAbsolutePath() + "/main.cpp", sourceBuffer);

    if (project.hasClasses()) {
        
        for (int i = 0; i < projectData["classes"].size(); i++)
        {
            Json::Value c = projectData["classes"][i];
            
            std::string classFile = _classTemplate;
            ofStringReplace(classFile, "<classname>", c["name"].asString());
            ofStringReplace(classFile, "<classfile>", c["fileContents"].asString());
            _replaceIncludes(classFile);
            
            ofBuffer sourceBuffer(classFile);
            ofBufferToFile(src.getAbsolutePath() + "/" + c["name"].asString() + ".h", sourceBuffer);
        }
    }
}


void Compiler::_replaceIncludes(std::string& fileContents) {
    
    Poco::RegularExpression includesExpression("#include .*");
    Poco::RegularExpression::Match match;
    
    std::vector<std::string> includes;
    
    int numMatches = 0;
    std::size_t matchOffset = 0;
    
    while (matchOffset < fileContents.size())
    {
        if (includesExpression.match(fileContents, matchOffset, match) == 0) break;
        std::string include;
        includesExpression.extract(fileContents, match.offset, include);
        includes.push_back(include);
        matchOffset = match.offset + match.length;
        numMatches++;
    }
    
    includesExpression.subst(fileContents, "", Poco::RegularExpression::RE_GLOBAL);
    ofStringReplace(fileContents, "<includes>", ofJoinString(includes, "\n"));

}


void Compiler::_parseAddons()
{
}


void Compiler::_getAddons()
{
}


} } // namespace of::Sketch
