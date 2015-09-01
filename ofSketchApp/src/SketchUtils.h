// =============================================================================
//
// Copyright (c) 2013-2015 Christopher Baker <http://christopherbaker.net>
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


#pragma once


#include <string>
#include "json/json.h"
#include "ofConstants.h"
#include "ofUtils.h"
#include "ofx/HTTP/WebSocketFrame.h"


namespace of {
namespace Sketch {


/// \brief A collection of utilities for ofSketch.
///
/// Generally these do not fit well elsewhere.
class SketchUtils
{
public:
    /// \returns A collection of platform info as json.
    static Json::Value systemInfo();

    /// \brief Create a WebSocketFrame.
    /// \param data The data to send in the websocket frame.
    /// \returns a WebSocketFrame.
    static ofx::HTTP::WebSocketFrame makeFrame(const Json::Value& data);

    // Wraps a json method in the ofSketch protocol headers.
    static Json::Value toJSONMethod(const std::string& module,
                                    const std::string& method,
                                    const Json::Value& params);


    // TODO: HACK while openFrameworks core is updated.
    // - https://github.com/openframeworks/openFrameworks/issues/2162
    // - https://github.com/openframeworks/openFrameworks/pull/3109
    static ofTargetPlatform getTargetPlatform();

    /// \brief Convert an ofTargetPlatform to a std::string.
    /// \param targetPlatform The ofTargetPlatform to convert.
    /// \returns the std::string representation.
    static std::string TargetPlatformToString(ofTargetPlatform targetPlatform);

    /// \brief Convert a std::string to an ofTargetPlatform.
    /// \param targetPlatform The std::string to convert.
    /// \returns the ofTargetPlatform representation.
    static ofTargetPlatform TargetPlatformFromString(const std::string& targetPlatform);

    /// \brief Check to see if the current build system contains a commend.
    /// \param command The command to check for.
    /// \returns true iff the current build system contains the command.
    static bool hasDependency(const std::string& command);

};


} } // namespace of::Sketch
