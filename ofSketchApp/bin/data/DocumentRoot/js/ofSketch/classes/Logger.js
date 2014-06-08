// =============================================================================
//
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

/*
Assumes that the following are globally pre-defined:
- jquery.js
- ace.js
- helper.js
- underscore.js
 */

function Logger() 
{

	var OF_LOG_VERBOSE     = 0;
	var OF_LOG_NOTICE      = 1;
	var OF_LOG_WARNING     = 2;
	var OF_LOG_ERROR       = 3;
	var OF_LOG_FATAL_ERROR = 4;
	var OF_LOG_SILENT	   = 5;

	var _self = this;
	var _logLevel = OF_LOG_VERBOSE;

	var _logContainerObj = $('#logger-container');

	var _registerEvents = function()
	{
		
	}

	var _addLogElement = function(logLevel, module, message)
	{
		console.log("logging " + message);
		var logElement = $('<li class="list-group-item"></li>');
		var message = $('<span><strong>' + module + '</strong><br><p style="overflow:auto">'+ message + "</p></span>");
		var label = $('<span class="label" style="float:right">' + _self.getLogLevelName(logLevel) + '</span>');
		
		label.addClass(_self.getLogLevelLabelClass(logLevel));

		logElement.append(label);
		logElement.append(message);
		_logContainerObj.append(logElement);
	}

	this.setLogLevel = function(logLevel)
	{
		console.log("changed log level to " + logLevel);
		_logLevel = logLevel;
	}

	this.getLogLevel = function()
	{
		return _logLevel;
	}

	this.getLogLevelName = function(logLevel) {

		if      (logLevel == OF_LOG_VERBOSE)     return "Verbose";
		else if (logLevel == OF_LOG_NOTICE)      return "Notice";
		else if (logLevel == OF_LOG_WARNING)     return "Warning";
		else if (logLevel == OF_LOG_ERROR)       return "Error";
		else if (logLevel == OF_LOG_FATAL_ERROR) return "Fatal Error";
		else if (logLevel == OF_LOG_SILENT)      return "Silent";
	}

	this.getLogLevelLabelClass = function(logLevel) {

		if      (logLevel == OF_LOG_VERBOSE)     return "btn-info";
		else if (logLevel == OF_LOG_NOTICE)      return "btn-primary";
		else if (logLevel == OF_LOG_WARNING)     return "btn-warning";
		else if (logLevel == OF_LOG_ERROR)       return "btn-danger";
		else if (logLevel == OF_LOG_FATAL_ERROR) return "btn-danger";
		else if (logLevel == OF_LOG_SILENT)      return "btn-default";
	}

	this.log = function(logLevel, module, message)
	{
		if (logLevel >= _self.getLogLevel()) {
			_addLogElement(logLevel, module, message);
		}
	}

	this.clear = function()
	{
		_logContainerObj.empty()
	}

}
