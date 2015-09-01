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


#include "WebSocketLoggerChannel.h"
#include "Poco/Buffer.h"
#include "ofApp.h"
#include "Serializer.h"


namespace of {
namespace Sketch {


WebSocketLoggerChannel::WebSocketLoggerChannel()
{
}


WebSocketLoggerChannel::~WebSocketLoggerChannel()
{
}


void WebSocketLoggerChannel::log(ofLogLevel level,
                                 const std::string& module,
                                 const std::string& message)
{
    ofConsoleLoggerChannel::log(level, module, message);

    ofx::HTTP::WebSocketRoute::SharedPtr route = _webSocketRoute.lock();

    if (route)
    {
        Json::Value params;
        params["level"] = level;
        params["module"] = module;
        params["message"] = message;

        Json::Value json = SketchUtils::toJSONMethod("Logger",
                                                     "message",
                                                     params);

        ofx::HTTP::WebSocketFrame frame(Serializer::toString(json));

        route->broadcast(frame);
    }
}

void WebSocketLoggerChannel::log(ofLogLevel level,
                                 const std::string& module,
                                 const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log(level, module, format, args);
    va_end(args);
}

void WebSocketLoggerChannel::log(ofLogLevel level,
                                 const std::string& module,
                                 const char* format,
                                 va_list args)
{
    ofConsoleLoggerChannel::log(level, module, format, args);
    std::cerr << "WebSocketLoggerChannel::log Message was not passed to WebSocket." << std::endl;
}


void WebSocketLoggerChannel::setWebSocketRoute(ofx::HTTP::WebSocketRoute::SharedPtr webSocketRoute)
{
    _webSocketRoute = webSocketRoute;
}


} } // namespace of::Sketch
