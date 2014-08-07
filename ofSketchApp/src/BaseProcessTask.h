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
#include <vector>
#include "Poco/PipeStream.h"
#include "Poco/Process.h"
#include "Poco/Task.h"


namespace of {
namespace Sketch {


/// \brief A simple base class for system call tasks.
///
/// Progress callbacks and custom data events are the responsibility of the
/// subclass.
class BaseProcessTask: public Poco::Task
{
public:
    BaseProcessTask(const std::string& name,
                    const std::string& command,
                    const std::vector<std::string>& args = std::vector<std::string>(),
                    std::size_t bufferSize = DEFAULT_BUFFER_SIZE);

    virtual ~BaseProcessTask();

	virtual void cancel();

    virtual void runTask();

    virtual void processLine(const std::string& line) = 0;

    enum
    {
        DEFAULT_BUFFER_SIZE = 8192
    };

protected:
    std::string _command;

    std::vector<std::string> _args;

    std::size_t _bufferSize;

    Poco::Pipe _outAndErrPipe;
};


} } // namespace of::Sketch
