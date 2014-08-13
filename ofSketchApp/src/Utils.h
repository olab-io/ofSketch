// =============================================================================
//
// Copyright (c) 2013-2014 Christopher Baker <http://christopherbaker.net>
//               2014 Brannon Dorsey <http://brannondorsey.com>
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
#include <json/json.h>
#include "ofConstants.h"
#include "ofUtils.h"


namespace of {
namespace Sketch {


class Utils
{
public:
    // Wraps a json method in the ofSketch protocol headers.
    static Json::Value toJSONMethod(const std::string& module,
                                    const std::string& method,
                                    const Json::Value& params);

    // This is a utility method for quickly converting a json value to a string.
    static std::string toJSONString(const Json::Value& json);

    // TODO: HACK while openFrameworks core is updated.
    // - https://github.com/openframeworks/openFrameworks/issues/2162
    // - https://github.com/openframeworks/openFrameworks/pull/3109
    static ofTargetPlatform getTargetPlatform();
    
    static std::string toString(ofTargetPlatform targetPlatform);

};


} } // namespace of::Sketch
