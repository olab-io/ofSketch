// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
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

// ofxHTTP /////////////////////////////////////////////////////////////////////

// function ofPixelHandler()
// {
//     var CMD_PIXELS = "SET_PIXELS";

//     var OF_PIXELS_MONO   = 0;
//     var OF_PIXELS_RGB    = 1;
//     var OF_PIXELS_RGBA   = 2;
//     var OF_PIXELS_BGRA   = 3;
//     var OF_PIXELS_RGB565 = 4;

//     var PIXELS_DATA_TYPE_ARRAY_BUFFER = 0;
//     var PIXELS_DATA_TYPE_BLOB         = 1;

//     var _bIsWaitingForPixels = false;
//     var _dataType      = PIXELS_DATA_TYPE_ARRAY_BUFFER;
//     var _type          = OF_PIXELS_RGBA;
//     var _width         = 0;
//     var _height        = 0;
//     var _destination   = "";

// }

// ofPixelHandler.prototype = function() 
// {
//     isWaitingForPixels: function() 
//     {
//         return this._bIsWaitingForPixels;
//     },
//     getPixelsDataType: function()
//     {
//         return this._pixelsDataType;
//     },
//     getPixelsType: function()
//     {
//         return this._pixelsType;
//     },
//     getWidth: function()
//     {
//         return this._width;
//     },
//     getHeight: function()
//     {
//         return this._height;
//     }
//     getDestination: function()
//     {
//         return this._destination;
//     }
//     handleMessage: function(message)
//     {


//         return false;

//         return true;
//     }

// }

//------------------------------------------------------------------------------
function ofxHTTPBasicWebSocketClient(supportedProtocols)
{
    var _self = this;

    var _ws  = null; // WebSocket
    var _wsDataType = "binary";
    var _customPath = "";
    var _suppotedSubProtocols   = supportedProtocols || [];
    var _keepAliveInterval      = 15000; // milliseconds
    var _keepAliveTimeout       = null;  // set with setTimeout()
    var _retryConnectTimer      = null;  // set with setInterval()
    var _retryConnectInterval   = 5000;  // milliseconds
    var _bIsRetryingConnect     = false; // are we in the process of retrying
    var _maxConnectRetries      = 10;    // we will allow this many retries 
    var _numConnectRetries      = 0;     // how many times have we retried to connect 

    var _commandCallbacks = new Object();

    var _sequenceReceived       = 0;
    var _sequenceTransmited     = 0;

    var _JSONRPClient = new $.JsonRpcClient({ 
        getSocket: function() { 
            return _ws; 
        },
        onmessage: function(evt) { 
            _onMessage(evt); 
        }
    });

    //--------------------------------------------------------------------------
    

    //--------------------------------------------------------------------------
    var _onUpdateMessage = function(message)
    {
        ofLogNotice("onMessageUpdate");
    }

    //--------------------------------------------------------------------------
    var _onUpdateStatus = function(status)
    { 
        ofLogNotice("onStatusUpdate");
    }

    //--------------------------------------------------------------------------
    var _sendKeepAlive = function()
    { 
        ofLogNotice("sendKeepAlive");
    }

    //--------------------------------------------------------------------------
    var _onOpen = function(evt)
    {
        ofLogNotice("onOpen");
    } 
    
    //--------------------------------------------------------------------------
    var _onMessage = function(evt)
    {
        ofLogNotice("onMessage " + evt.data);
    }

    //--------------------------------------------------------------------------
    var _onClose = function(evt)
    {
        ofLogNotice("onClose");
    }
    
    //--------------------------------------------------------------------------
    var _onError = function(evt)
    {
        ofLogNotice("onError");
    }

    //--------------------------------------------------------------------------
    // make sure that the appropriate websocket scheme is used
    // i.e. ws:// or wss:// for secure connections
    this.getWebSocketURL = function() 
    {
// TODO :       ws_host = window.location.href.replace(/(http|https)(:\/\/.*?)\//, 'ws$2');
        var scheme;
        var url = document.URL;
        if(url.substring(0, 5) == "https")
        {
            scheme = "wss://";
            url = url.substr(8);
        }
        else
        {
            scheme = "ws://";
            
            if (url.substring(0, 4) == "http")
            {
                url = url.substr(7);
            }
        }

        url = url.split('/');

        return scheme + url[0];
    }

    //--------------------------------------------------------------------------
    this.getWebSocketDataType = function()
    {
        return _wsDataType;
    }

    //--------------------------------------------------------------------------
    this.getWebSocket = function()
    {
        return _ws;
    }

    //--------------------------------------------------------------------------
    this.isConnected = function()
    {
        return _ws != null;
    }

    //--------------------------------------------------------------------------
    this.getCustomPath = function()
    {
        return _customPath;
    }

    //--------------------------------------------------------------------------
    this.setCustomPath = function(customPath)
    {
        _customPath = customPath;
    }

    //--------------------------------------------------------------------------
    this.getKeepAliveInterval = function() 
    {
        return _keepAliveInterval;
    }

    //--------------------------------------------------------------------------
    this.setKeepAliveInterval = function(keepAliveInterval)
    {
        _keepAliveInterval = keepAliveInterval;
    }

    //--------------------------------------------------------------------------
    this.getSupportedSubProtocols = function()
    {
        return _suppotedSubProtocols;
    }

    //--------------------------------------------------------------------------
    this.getRetryConnectInterval = function()
    {
        return _retryConnectInterval;
    }

    //--------------------------------------------------------------------------
    this.setRetryConnectInterval = function(retryConnectInterval)
    {
        _retryConnectInterval = retryConnectInterval;
    }

    //--------------------------------------------------------------------------
    this.isRetryingConnect = function()
    {
        return _isRetryingConnect;
    }

    //--------------------------------------------------------------------------
    this.getMaxConnectRetries = function()
    {
        return _maxConnectRetries;
    }

    //--------------------------------------------------------------------------
    this.setMaxConnectRetries = function(maxConnectRetries)
    {
        _maxConnectRetries = maxConnectRetries;
    }

    //--------------------------------------------------------------------------
    this.setBinaryType = function(binaryType) 
    {
        if(_self.isConnected()) 
        {
            if(binaryType == "blob" || 
               binaryType == "arraybuffer")
            {
                _ws.binaryType = binaryType;
            } 
            else 
            {
                ofLogError("Unknown binaryType: " + binaryType);
            }
        } 
        else
        {
          ofLogError("WebSocket has not been initialized.  Please call connect() first.");
        }
    }

    //--------------------------------------------------------------------------
    this.setOnOpen = function(fn)
    {
        _onOpen = fn;
    }

    //--------------------------------------------------------------------------
    this.setOnMessage = function(fn)
    {
        _onMessage = fn;
    }

    //--------------------------------------------------------------------------
    this.setOnClose = function(fn)
    {
        _onClose = fn;
    }

    //--------------------------------------------------------------------------
    this.setOnError = function(fn)
    {
        _onError = fn;
    }

    //--------------------------------------------------------------------------
    this.getBufferedAmount = function()
    {
        if(_self.isConnected())
        {
            return _ws.bufferedAmount;
        } 
        else 
        {
            ofLogError("WebSocket has not been initialized.  Please call connect() first.");
            return -1;
        }
    }

    //--------------------------------------------------------------------------
    this.connect = function()
    {
        if("WebSocket" in window) 
        {
            try 
            {
                _ws = new WebSocket(_self.getWebSocketURL(),
                                    _self.getSupportedSubProtocols());

                _ws.onopen = function(evt)
                {
                    _onOpen(evt); // our event

                    _retryConnectTimer = setInterval(
                        function()
                        { 
                            _sendKeepAlive();
                        }, 
                            _self.getKeepAliveInterval()
                        );
                } 
            
                _ws.onmessage = function(evt)
                { 
                    _onMessage(evt);
                }

                _ws.onclose = function(evt)
                {
                    clearInterval(_retryConnectTimer);
                    
                    if(!evt.wasClean || evt.code == 1006 /* 1006 == abnormal closure */)
                    {
                        if(_self.getSupportedSubProtocols().length > 0 &&
                           _self.getSupportedSubProtocols().hasObject(evt.target.protocol)) 
                        {
                            ofLogError("None of the proposed subprotocols [" + _self.getSupportedSubProtocols() + "] were supported by the server.");
                        }
                        else
                        {
                            ofLogError("Closed for an unknown reason, code: " + evt.code);
                        }
                    }
                
                    _onClose(evt); // our event
                }
                
                _ws.onerror = function(evt)
                { 
                    _onError(evt);
                }

            } catch(exception) {
                ofLogError(exception);  
            } 
        } 
        else 
        {
            alert("This browser does not support WebSockets.  Please upgrade your browser.");
        }
    }

    //--------------------------------------------------------------------------
    this.disconnect = function()
    {
        if(_self.isConnected()) 
        {
            window.clearInterval(_keepAliveTimeout);
            _ws.close();
            _ws = null;
        } 
        else
        {
          ofLogError("WebSocket has not been initialized.  Please call connect first.");
        }
    }

    //--------------------------------------------------------------------------
    this.send = function(val)
    {
        // this will be reset each time
        window.clearInterval(_keepAliveTimeout);
        _keepAliveTimeout = window.setInterval(_sendKeepAlive,
                                               _keepAliveInterval);

        // lastSendTime = now
        _ws.send(val);

        ofLogVerbose("Sent " + val);
    }
}
