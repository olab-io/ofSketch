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

void Compiler::make(const Project& project)
{
    
}
    
void Compiler::run(const Project& project)
{
    _generateSource(project);
}

void Compiler::_generateSource(const Project& project)
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
