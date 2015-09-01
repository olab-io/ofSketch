// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ProcessTask.h"
#include "Poco/Buffer.h"
#include "Poco/StreamCopier.h"
#include "Poco/Thread.h"
#include "Poco/TaskNotification.h"
#include <iostream>
#include "ofLog.h"
#include "ofUtils.h"


namespace of {
namespace Sketch {


ProcessTaskSettings::ProcessTaskSettings(const std::string& taskName,
                                         const std::string& command,
                                         const std::vector<std::string>& args,
                                         const Poco::Path& initialDirectory,
                                         const Poco::Process::Env& env,
                                         std::size_t bufferSize):
    _taskName(taskName),
    _command(command),
    _args(args),
    _initialDirectory(initialDirectory),
    _env(env),
    _bufferSize(bufferSize)
{
}


ProcessTaskSettings::~ProcessTaskSettings()
{
}


const std::string& ProcessTaskSettings::getTaskName() const
{
    return _taskName;
}


const std::string& ProcessTaskSettings::getCommand() const
{
    return _command;
}


const std::vector<std::string> ProcessTaskSettings::getArgs() const
{
    return _args;
}


const Poco::Path& ProcessTaskSettings::getInitialDirectory() const
{
    return _initialDirectory;
}


const Poco::Process::Env& ProcessTaskSettings::getEnvironment() const
{
    return _env;
}


std::size_t ProcessTaskSettings::getBufferSize() const
{
    return _bufferSize;
}


ProcessTask::ProcessTask(const ProcessTaskSettings& settings):
    Poco::Task(settings.getTaskName()),
    _settings(settings)
{
}


ProcessTask::~ProcessTask()
{
}


void ProcessTask::runTask()
{
    /// \todo Break apart out and err pipe.
    Poco::Pipe _outAndErrPipe;
    Poco::PipeInputStream istr(_outAndErrPipe);

    try
    {

		Poco::ProcessHandle ph = Poco::Process::launch(_settings.getCommand(),
													   _settings.getArgs(),
													   _settings.getInitialDirectory().toString(),
													   0,
													   &_outAndErrPipe,
													   &_outAndErrPipe,
													   _settings.getEnvironment());

		ofLogVerbose("BaseProcessTask::runTask") << "Launching Task: " << _settings.getCommand() << " Args: " << ofToString(_settings.getArgs()) << " PID: " << ph.id();

		Poco::Buffer<char> buffer(_settings.getBufferSize());

		while (istr.good() && !istr.fail() && !isCancelled())
		{

            /// \todo http://www.cplusplus.com/reference/istream/istream/read/

#if !defined(TARGET_WIN32)
			fd_set readset;
			struct timeval tv;

			FD_ZERO(&readset);
			FD_SET(_outAndErrPipe.readHandle(), &readset);

			tv.tv_sec = 0;
			tv.tv_usec = 50 * 1000; // 50 ms.

			int rc = ::select(_outAndErrPipe.readHandle() + 1,
                              &readset,
                              0,
                              0,
                              &tv);
#else
			// Temporary hack to get around the fact that
			// there is no "select" on Windows platforms.
			int rc = 1;
#endif

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
    catch(Poco::Exception& exc)
    {
        ofLogError("BaseProcessTask::runTask") << exc.displayText();
    }
}


void ProcessTask::processLine(const std::string& line)
{
    postNotification(new Poco::TaskCustomNotification<std::string>(this, line));
}


} } // namespace of::Sketch
