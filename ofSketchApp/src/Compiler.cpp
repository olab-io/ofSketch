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
    
    Compiler::Compiler(std::string pathToTemplates):
_pathToTemplates(pathToTemplates)
{
    _projectFileTemplate = ofBufferFromFile(ofToDataPath(_pathToTemplates + "/main.txt")).getText();
    _classTemplate = ofBufferFromFile(ofToDataPath(_pathToTemplates + "/class.txt")).getText();
}

void Compiler::run(const Project& project)
{
    
    std::string cmd("make");
    std::vector<std::string> args;
    
    args.push_back("--directory=" + ofToDataPath("Projects/HelloWorld/", true));
    //        if(_settings.numProcessors > 1)
    //        {
    //            args.push_back("-j" + ofToString(_settings.numProcessors));
    //        }
    
    args.push_back("-j" + ofToString(8));
    
    //        if(_settings.isSilent)
    //        {
    args.push_back("-s");
    //        }
    
    //        args.push_back(_target);
    
    args.push_back("OF_ROOT=" + ofToDataPath("openFrameworks/", true));
    
    Poco::Pipe inPipe; // this needs to be passed in
    Poco::Pipe outAndErrPipe;
    
    Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, &inPipe, &outAndErrPipe, &outAndErrPipe);
    Poco::PipeInputStream istr(outAndErrPipe);
    
    //        const std::size_t bufferSize = 8192;
    //        char buffer[bufferSize];
    //
    //        while(istr.good() && !istr.fail())
    //        {
    //            if(isCancelled())
    //            {
    //                Poco::Process::kill(ph);
    //            }
    //
    //            istr.getline(buffer,bufferSize);
    //            cout << "LINE>>" << buffer << "<<LINE" << endl;
    //        }
    
    Poco::StreamCopier::copyStream(istr, std::cout);
    
    int exitCode = ph.wait();
    
    cout << "exit code: " << exitCode << endl;
    
    args.push_back("run");
    
    ph = Poco::Process::launch(cmd, args, &inPipe, &outAndErrPipe, &outAndErrPipe);
    
    Poco::PipeInputStream istr2(outAndErrPipe);
    Poco::StreamCopier::copyStream(istr2, std::cout);
    
    exitCode = ph.wait();
    
    cout << "exit code: " << exitCode << endl;
}

void Compiler::generateSourceFiles(const Project& project)
{
    ofxJSONElement projectData = project.getData();
    std::string projectFile = _projectFileTemplate;
    ofStringReplace(projectFile, "<projectfile>", projectData["projectFile"]["fileContents"].asString());
    
    if (project.hasClasses()) {
        
        std::string includes;
        std::vector<std::string> classFiles;
        
        for (int i = 0; i < projectData["classes"].size(); i++) {
            
            ofxJSONElement c = projectData["classes"][i];
            includes += "#include \"" + c["name"].asString() + ".h\"\n";
            
            std::string classFile = _classTemplate;
            ofStringReplace(classFile, "<classname>", c["name"].asString());
            ofStringReplace(classFile, "<classfile>", c["fileContents"].asString());
            ofStringReplace(classFile, "<includes>", ""); // temporary
            
            ofBuffer sourceBuffer(classFile);
            cout<<"Class name: "<<c["name"].asString()<<endl;
            ofBufferToFile(project.getPath() + "/src/" + c["name"].asString() + ".h", sourceBuffer);
        }
        
        ofStringReplace(projectFile, "<includes>", includes);
        
        ofBuffer sourceBuffer(projectFile);
        ofBufferToFile(project.getPath() + "/src/main.cpp", sourceBuffer);
    
    } else ofStringReplace(projectFile, "<includes>", "");
}
    
void Compiler::_parseAddons()
{
    
}
    
void Compiler::_getAddons()
{
    
}

    
} } // namespace of::Sketch
