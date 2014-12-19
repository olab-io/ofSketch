// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
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
#include "Poco/Path.h"
#include "Poco/PipeStream.h"
#include "Poco/Process.h"
#include "Poco/Task.h"


namespace of {
namespace Sketch {


/// \brief Settings for configuring a ProcessTask.
class ProcessTaskSettings
{
public:
    /// \brief Create default ProcessTaskSettings.
    ProcessTaskSettings();

    /// \brief Create a BaseProcessTaskSettings.
    /// \param taskName The name of the task.
    /// \param command The system command to run.
    /// \param args The command arguments.
    /// \param initialDirectory The initial directory to run the command in.
    /// \param env The environmental variables to be passed with the command.
    /// \param bufferSize The line buffer size for processing the lines.
    ProcessTaskSettings(const std::string& taskName,
                        const std::string& command,
                        const std::vector<std::string>& args = std::vector<std::string>(),
                        const Poco::Path& initialDirectory = "",
                        const Poco::Process::Env& env = Poco::Process::Env(),
                        std::size_t bufferSize = DEFAULT_BUFFER_SIZE);

    /// \brief Destroy the ProcessTaskSettings.
    virtual ~ProcessTaskSettings();

    /// \returns the task name.
    const std::string& getTaskName() const;

    /// \returns the command name.
    const std::string& getCommand() const;

    /// \returns the arguments to pass with the command.
    const std::vector<std::string> getArgs() const;

    /// \returns the initial directory to run the command.
    const Poco::Path& getInitialDirectory() const;

    /// \returns the environmental variables to set for the command.
    const Poco::Process::Env& getEnvironment() const;

    /// \returns the buffer size in bytes for processing the command output.
    std::size_t getBufferSize() const;

    enum
    {
        DEFAULT_BUFFER_SIZE = 8192 ///< The default line buffer size.
    };

protected:
    std::string _taskName; ///< The task name.
    std::string _command;  ///< The command to run.
    std::vector<std::string> _args; ///< The command's argument.
    Poco::Path _initialDirectory; ///< The inital directory for the command.
    Poco::Process::Env _env; ///< The environmental variables for the command.
    std::size_t _bufferSize; ///< The size of the line buffer in bytes.

};


/// \brief A simple class for system call tasks.
class ProcessTask: public Poco::Task
{
public:
    /// \brief Create a ProcessTask.
    ProcessTask(const ProcessTaskSettings& settings);

    /// \brief Destroy the ProcessTask.
    virtual ~ProcessTask();

    /// \brief Run the task.
    virtual void runTask();

    /// \brief Process each line as it is read from std output.
    virtual void processLine(const std::string& line);

protected:
    ProcessTaskSettings _settings; ///< The proces task settings.

};


} } // namespace of::Sketch
