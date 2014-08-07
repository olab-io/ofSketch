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


#include "BaseProcessTask.h"
#include "Poco/Buffer.h"
#include "Poco/Thread.h"
#include <iostream>
#include "ofLog.h"
#include "ofUtils.h"


namespace of {
namespace Sketch {


BaseProcessTask::BaseProcessTask(const std::string& name,
                                 const std::string& command,
                                 const std::vector<std::string>& args,
                                 std::size_t bufferSize):
    Poco::Task(name),
    _command(command),
    _args(args),
    _bufferSize(bufferSize)
{
}


BaseProcessTask::~BaseProcessTask()
{
}


void BaseProcessTask::runTask()
{
    Poco::Pipe _outAndErrPipe;

    Poco::PipeInputStream istr(_outAndErrPipe);

    Poco::ProcessHandle ph = Poco::Process::launch(_command,
                                                   _args,
                                                   0,
                                                   &_outAndErrPipe,
                                                   &_outAndErrPipe);

    ofLogVerbose("BaseProcessTask::runTask") << "Launching Task: " << _command << " Args: " << ofToString(_args) << " PID: " << ph.id();

    Poco::Buffer<char> buffer(_bufferSize);

    while (istr.good() && !istr.fail() && !isCancelled())
    {
        fd_set readset;
        struct timeval tv;

        FD_ZERO(&readset);
        FD_SET(_outAndErrPipe.readHandle(), &readset);

        tv.tv_sec = 0;
        tv.tv_usec = 50 * 1000; // 50 ms.

        int rc = ::select(_outAndErrPipe.readHandle() + 1, &readset, 0, 0, &tv);

        if (rc > 0)
        {
            istr.getline(buffer.begin(), buffer.size());

            if (buffer.begin())
            {
                std::string str(buffer.begin());

                if (!str.empty())
                {
                    // Progress callbacks and custom data events are the
                    // responsibility of the subclass.
                    processLine(str);
                }
            }
        }

    }

    Poco::Process::kill(ph);

    int exitCode = ph.wait();

    ofLogVerbose("BaseProcessTask::runTask") << "Exit PID: " << ph.id() << " with: " << exitCode;
}


} } // namespace of::Sketch
