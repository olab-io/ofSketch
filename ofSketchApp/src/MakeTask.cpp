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


#include "MakeTask.h"


namespace of {
namespace Sketch {


MakeTask::Settings::Settings():
    ofRoot(ofToDataPath("openFrameworks")),
    numProcessors(1),
    isSilent(false),
    useDistccServer(false),
    cxx(""),
    cc("")
{
}


MakeTask::MakeTask(const Settings& settings,
                   Project::SharedPtr project,
                   const std::string& target):
    Poco::Task(project->getPath()),
    _settings(settings),
    _project(project),
    _target(target)
{
}


MakeTask::~MakeTask()
{
}


void MakeTask::runTask()
{
    std::string cmd("make");

    std::vector<std::string> args;

    args.push_back("--directory=" + _project->getPath());
    if(_settings.numProcessors > 1)
    {
        args.push_back("-j" + ofToString(_settings.numProcessors));
    }

    if(_settings.isSilent)
    {
        args.push_back("-s");
    }

    args.push_back(_target);

    args.push_back("OF_ROOT=" + _settings.ofRoot);

    Poco::Pipe inPipe; // this needs to be passed in
    Poco::Pipe outAndErrPipe;
//    Poco::Pipe errPipe;

    Poco::ProcessHandle ph = Poco::Process::launch(cmd, args, &inPipe, &outAndErrPipe, &outAndErrPipe);

    Poco::PipeInputStream istr(outAndErrPipe);

    const std::size_t bufferSize = 8192;
    char buffer[bufferSize];

    while(istr.good() && !istr.fail())
    {
        if(isCancelled())
        {
            Poco::Process::kill(ph);
        }

        istr.getline(buffer,bufferSize);
        cout << "LINE>>" << buffer << "<<LINE" << endl;
    }


    int exitCode = ph.wait();

    cout << "exit code: " << exitCode << endl;

}


} } // namespace of::Sketch
