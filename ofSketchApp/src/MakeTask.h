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


#include <iostream>
#include <set>
#include "Poco/Process.h"
#include "Poco/PipeStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Task.h"
#include "Poco/Net/SocketAddress.h"
#include "ofUtils.h"
#include "Project.h"
#include "BaseProcessTask.h"


namespace of {
namespace Sketch {


class MakeTask: public BaseProcessTask
{
public:
    struct Settings;

    MakeTask(const Settings& settings,
             const Project& project,
             const std::string& target);

    virtual ~MakeTask();

    virtual void processLine(const std::string& line);

    struct Settings
    {
        std::string ofRoot;

        std::size_t numProcessors; // -j
        bool isSilent; // -s

        std::string CXX; // CXX=/usr/lib/distcc/arm-linux-gnueabihf-g++
        std::string CC;  // CC=/usr/lib/distcc/arm-linux-gnueabihf-gcc

        std::string platformVariant; // e.g. PLATFORM_VARIANT=udoo
        bool makefileDebug; // e.g. MAKEFILE_DEBUG=1

        Settings();
    };

private:
    Settings _settings;
    const Project& _project;
    std::string _target;

};


} } // namespace of::Sketch
