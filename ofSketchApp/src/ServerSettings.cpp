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


#include "ServerSettings.h"
#include "ofSSLManager.h"


namespace of {
namespace Sketch {


const std::string ServerSettings::DEFAULT_WHITELIST = "127.0.0.1/32";
const bool ServerSettings::DEFAULT_USE_SSL = false;


ServerSettings::ServerSettings():
    _port(DEFAULT_PORT),
    _webSocketBufferSize(DEFAULT_WEBSOCKET_BUFFER_SIZE),
    _maximumFileUploadSize(DEFAULT_MAXIMUM_FILE_UPLOAD_SIZE),
    _useSSL(DEFAULT_USE_SSL)
{
    _whitelist.push_back(ofx::Net::IPAddressRange(DEFAULT_WHITELIST));
}


ServerSettings::ServerSettings(uint16_t port,
                               std::size_t webSocketBufferSize,
                               std::size_t maximumFileUploadSize,
                               const ofx::Net::IPAddressRange::List& whitelist,
                               bool useSSL,
                               const SSLSettings& sslSettings):
    _port(port),
    _webSocketBufferSize(webSocketBufferSize),
    _maximumFileUploadSize(maximumFileUploadSize),
    _whitelist(whitelist),
    _useSSL(useSSL),
    _sslSettings(sslSettings)
{
}


std::size_t ServerSettings::getWebSocketBufferSize() const
{
    return _webSocketBufferSize;
}


uint16_t ServerSettings::getPort() const
{
    return _port;
}


bool ServerSettings::useSSL() const
{
    return _useSSL;
}


std::size_t ServerSettings::getMaxiumFileUploadSize() const
{
    return _maximumFileUploadSize;
}


const ofx::Net::IPAddressRange::List& ServerSettings::getWhitelist() const
{
    return _whitelist;
}


const SSLSettings& ServerSettings::getSSLSettings() const
{
    return _sslSettings;
}


} } // namespace of::Sketch
