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


#pragma once


#include <string>
#include <vector>
#include "Poco/Task.h"


namespace of {
namespace Sketch {


class Project;


/// \brief A base class for Toolchains.
///
/// A toolchain is a factory for providing Tasks that relate to various portions
/// of the build process.  Task pointers created by the toolchain must be
/// deallocated by the caller.
class BaseToolchain
{
public:
    /// \brief Create a BaseToolchain with parameters.
    /// \param name The name of the BaseToolchain.
    BaseToolchain(const std::string& name);

    /// \brief Destroy this BaseToolchain.
    virtual ~BaseToolchain();

    /// \brief Get the name of the Toolchain.
    /// \returns the name of the Toolchain.
    const std::string& getName() const;

    /// \brief Get a list of the valid build / run targets.
    /// \returns a list of the valid build / run targets.
    const std::vector<std::string>& getTargets() const;

    /// \brief Create a task that builds an openFrameworks project.
    /// \param project The project to build.
    /// \param target The build target.
    /// \returns a Task contianing the build task or 0 on failure.
    /// \warning The Task pointer must be freed by the caller.
    virtual Poco::Task* newBuildTask(const Project& project,
                                     const std::string& target) const;

    /// \brief Create a task that runs an openFrameworks project.
    /// \param project The project to run.
    /// \param target The run target.
    /// \returns a Task contianing the run task or 0 on failure.
    /// \warning The Task pointer must be freed by the caller.
    virtual Poco::Task* newRunTask(const Project& project,
                                   const std::string& target) const;

protected:
    /// \brief The toolchain name.
    std::string _name;

    /// \brief A list of valid build / run targets for this toolchain.
    std::vector<std::string> _targets;

};


} } // namespace of::Sketch
