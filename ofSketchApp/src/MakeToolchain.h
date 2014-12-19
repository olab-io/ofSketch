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


#include "BaseToolchain.h"
#include "Poco/Path.h"


namespace of {
namespace Sketch {


/// \brief A base class for MakeTool chains.
class MakeToolchain: public BaseToolchain
{
public:
    /// \brief Create a named Make toolchain.
    /// \param name The name of the Toolchain.
    MakeToolchain(const std::string& name);

    /// \brief Destroy the MakeToolchain.
    virtual ~MakeToolchain();

    virtual Poco::Task* newBuildTask(const Project& project,
                                     const std::string& target) const;

    virtual Poco::Task* newRunTask(const Project& project,
                                   const std::string& target) const;

    /// \brief Get the project's exectuable's path given the target.
    /// \param project The project to examine.
    /// \param target The target to look for.
    /// \returns the absolute path to the project's executable's path.
    static Poco::Path getExecutablePath(const Project& project,
                                        const std::string& target);

};


} } // namespace of::Sketch
