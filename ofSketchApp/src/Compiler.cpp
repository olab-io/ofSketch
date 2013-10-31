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


#include "Compiler.h"


namespace of {
namespace Sketch {


Compiler::Compiler()
{
}

Compiler::~Compiler()
{
}

void Compiler::make(const Sketch& sketch, const std::string& target)
{
    std::string cmd("make");
    std::vector<std::string> args;
    args.push_back("--directory=" + sketch.getPath());
    if(_numProcessors > 1) args.push_back("-j" + ofToString(_numProcessors));
    if(_isSilent) args.push_back("-s");

    args.push_back(target);

    Poco::Pipe outPipe;
    Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, 0, &outPipe, 0);

    Poco::PipeInputStream istr(outPipe);
    // could also save to text and return to web.
    // std::ofstream ostr("processes.txt");
    Poco::StreamCopier::copyStream(istr,std::cout);

}


} } // namespace of::Sketch
