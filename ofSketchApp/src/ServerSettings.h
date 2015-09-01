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


#include "ofx/Net/IPAddressRange.h"
#include "SSLSettings.h"


namespace of {
namespace Sketch {


/// \brief A collection server settings.
class ServerSettings
{
public:
    /// \brief Create an instance of default server settings.
    ServerSettings();

    /// \brief Create server settings with parameters.
    /// \param port The server port.
    /// \param webSocketBufferSize The WebSocket buffer size in bytes.
    /// \param maximumFileUploadSize The maximum gile upload size in bytes.
    /// \param whitelist A list of IP addresses that can connect to the server.
    /// \param useSSL True if the server should use SSL encryption.
    /// \param sslSettings The SSL encryption settings.
    ServerSettings(uint16_t port,
                   std::size_t webSocketBufferSize,
                   std::size_t maximumFileUploadSize,
                   const ofx::Net::IPAddressRange::List& whitelist,
                   bool useSSL,
                   const SSLSettings& sslSettings);

    /// \returns The server port.
    uint16_t getPort() const;

    /// \returns The WebSocket buffer size in bytes.
    std::size_t getWebSocketBufferSize() const;

    /// \returns The maximum gile upload size in bytes.
    std::size_t getMaxiumFileUploadSize() const;

    /// \returns A list of IP addresses that can connect to the server.
    const ofx::Net::IPAddressRange::List& getWhitelist() const;

    /// \returns True iff the server should use SSL encryption.
    bool useSSL() const;

    /// \returns The SSL encryption settings.
    const SSLSettings& getSSLSettings() const;

    /// \brief The default whitelist entry.
    static const std::string DEFAULT_WHITELIST;

    /// \brief The default SSL use.
    static const bool DEFAULT_USE_SSL;

    enum
    {
        /// \brief Default port, 0 indicates use any.
        DEFAULT_PORT = 0,
        /// \brief Default websocket buffer size in bytes (40 Megabytes).
        DEFAULT_WEBSOCKET_BUFFER_SIZE = 4194304,
        /// \brief Default maximum file upload size in bytes (Maximum allowed).
        DEFAULT_MAXIMUM_FILE_UPLOAD_SIZE = SIZE_T_MAX
    };

private:
    /// \brief The server port. 0 indicates use any.
    uint16_t _port;

    /// \brief WebSocket buffer size in bytes.
    std::size_t _webSocketBufferSize;

    /// \brief Maximum file upload size in bytes.
    std::size_t _maximumFileUploadSize;

    /// \brief The valid IPs that can connect to this server.
    ofx::Net::IPAddressRange::List _whitelist;

    /// \brief True if SSL server is used.
    bool _useSSL;

    /// \brief The SSL encryption settings.
    SSLSettings _sslSettings;

};


} } // namespace of::Sketch
