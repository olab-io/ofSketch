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
#include "json/json.h"
#include "ofLog.h"
#include "ofx/HTTP/WebSocketRoute.h"
#include "ofx/HTTP/WebSocketFrame.h"


namespace of {
namespace Sketch {


class WebSocketLoggerChannel: public ofConsoleLoggerChannel
{
public:
    typedef std::shared_ptr<WebSocketLoggerChannel> SharedPtr;

    WebSocketLoggerChannel();

    virtual ~WebSocketLoggerChannel();

    void log(ofLogLevel level,
             const std::string& module,
             const std::string& message);

    void log(ofLogLevel level,
             const std::string& module,
             const char* format, ...);

    void log(ofLogLevel level,
             const std::string& module,
             const char* format,
             va_list args);

    void setWebSocketRoute(ofx::HTTP::WebSocketRoute::SharedPtr webSocketRoute);

    static SharedPtr makeShared()
    {
        return SharedPtr(new WebSocketLoggerChannel());
    }

protected:
    ofx::HTTP::WebSocketRoute::WeakPtr _webSocketRoute;

};


} } // namespace of::Sketch
