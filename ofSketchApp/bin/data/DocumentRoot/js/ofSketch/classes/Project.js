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
_data = {
	"projectFile": {
		"name": "myProject" 
		"fileName": "myProject.sketch",
		"fileContents": "" 
	},
	"classes": [
		{
			"name": "myProject"
			"fileName": "myClass.sketch",
			"fileContents": "" 
		}
	]
}
 */

function Project(projectName, callback) 
{
	var _settings;
	var _data;
	var _file = {
		name: "",
		fileName: "",
		fileContents: "",
	}

	this.load = function(projectName, callback)
	{
		JSONRPCClient.call('load-project', 
        					{ projectName: projectName },
					        function(result) {
					            _data = result;
					            callback();
					        },
					        function(error) {
					            addError(error);
					        });
	}

	this.save = function(callback)
	{
		JSONRPCClient.call('save-project', 
        					{ projectName: projectName },
					        function(result) {
					            console.log(result);
					        },
					        function(error) {
					            addError(error);
					        });
	}

	this.rename = function(callback)
	{
		JSONRPCClient.call('rename-project', 
        					projectName,
					        function(result) {
					            console.log(result);
					        },
					        function(error) {
					            addError(error);
					        });
	}

	this.getProjectFile = function()
	{
		return _data.projectFile;
	}

	this.getClasses = function()
	{
		return _data.classes;
	}

	this.addClassFile = function(c)
	{
		_data.classes.push(c);
	}

	this.getData = function()
	{
		return _data;
	}

	this.load(projectName, callback);
}
