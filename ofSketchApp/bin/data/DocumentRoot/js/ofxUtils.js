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

// HELPER //////////////////////////////////////////////////////////////////////
Array.prototype.hasObject = (
    !Array.indexOf ? function (o) {
        var l = this.length + 1;
        while (l -= 1) {
            if (this[l - 1] === o) {
                return true;
            }
        }
        return false;
    } : function (o) {
        return (this.indexOf(o) !== -1);
    }
);

// LOGGING /////////////////////////////////////////////////////////////////////
var OF_LOG_VERBOSE     = 0;
var OF_LOG_NOTICE      = 1;
var OF_LOG_WARNING     = 2;
var OF_LOG_ERROR       = 3;
var OF_LOG_FATAL_ERROR = 4;
var OF_LOG_SILENT      = 5;

var logLevel = OF_LOG_VERBOSE;

function ofGetLogLevelName(_logLevel)
{
    switch(_logLevel)
    {
        case OF_LOG_VERBOSE:
            return "OF_LOG_VERBOSE";
        case OF_LOG_NOTICE:
            return "OF_LOG_NOTICE";
        case OF_LOG_WARNING:
            return "OF_LOG_WARNING";
        case OF_LOG_ERROR:
            return "OF_LOG_ERROR";
        case OF_LOG_FATAL_ERROR:
            return "OF_LOG_FATAL_ERROR";
        case OF_LOG_SILENT:
            return "OF_LOG_SILENT";
        default:
            return "UNKNOWN_LOG_LEVEL";
    }
}

function ofGetLogLevel()
{
    return logLevel;
}

function ofSetLogLevel(_logLevel)
{
    logLevel = _logLevel; 
}

function ofLog(logLevel,val)
{
    if(logLevel >= ofGetLogLevel()) 
    {
        console.log(val);
    }
}

function ofLogVerbose(_message) 
{
    ofLog(self.OF_LOG_VERBOSE, _message);
}

function ofLogNotice(_message)
{
    ofLog(self.OF_LOG_NOTICE, _message);
}

function ofLogWarning(_message)
{
    ofLog(self.OF_LOG_WARNING, _message);
}

function ofLogError(_message)
{
    ofLog(self.OF_LOG_ERROR, _message);
}

function ofLogFatalError(_message)
{ 
    ofLog(self.OF_LOG_FATAL_ERROR, _message);
}

function ofLogSilent(_message)
{
    ofLog(self.OF_LOG_SILENT, _message);
}

// UTILITIES ///////////////////////////////////////////////////////////////////

function ofClamp(val,max,min) 
{
    return (val < min ? max : (val > min ? min : val));
}


//--------------------------------------------------------------------------
// make sure that the appropriate websocket scheme is used
// i.e. ws:// or wss:// for secure connections
function getDefaultWebSocketURL() 
{
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
function getDefaultPostURL() 
{
    return document.URL + "post";
}
