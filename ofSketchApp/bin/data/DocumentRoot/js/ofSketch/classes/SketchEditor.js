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

function SketchEditor(callback) 
{
	var _self = this;

	var _editorSettingsFile = "editorsettings.json";
	var _projectFileTemplateFile = "templates/project.txt";
	var _classTemplateFile = "templates/class.txt";

	var _settings;
	var _tabs = [];
	var _project;
	var _projectFileTemplate;
	var _classTemplate;

	var _editor = ace.edit('editor');
    _editor.setTheme(settings.editorTheme);
    _editor.getSession().setMode(settings.editorMode);
    _editor.getSession().setTabSize(settings.tabSize);
    _document.getElementById('editor').style.fontSize = settings.fontSize + 'px';
    _editor.setHighlightActiveLine(settings.editorHighlightActiveLine);
    _editor.setShowPrintMargin(settings.editorShowPrintMargin);
    _editor.setShowInvisibles(settings.editorShowInvisibles);
    _editor.setBehavioursEnabled(settings.editorAutopairCharacters);

	var _registerEvents = function()
	{
		$('#create-class').on('click', _self.createClassFile);
		$('.project-tab').on('click', function(){
			_self.renderTab($(this).text());
		});
	}

	this.load = function(projectName, callback)
	{
		_project = new Project(projectName, callback);
		
		var projectFile = _project.getProjectFile();
		_tabs.push({
			name: getPrettyFileName(projectFile.fileName),
			fileName: projectFile.fileName,
			isProjectFile: true,
			editSession: new ace.EditSession(_project.getProjectFile().fileContents,
											 _settings.editorMode);
		});

		var classes = _project.getClasses();
		_.each(classes, function(c){
			_tabs.push({
				name: getPrettyFileName(c.fileName),
				fileName: projectFile.fileName,
				isProjectFile: false,
				editSession: new ace.EditSession(_c.fileContents, _settings.editorMode);
			});
		});

	}

	this.save = function(callback)
	{
		_project.save(callback);
	}

	this.createClassFile = function(className, callback)
	{
		var c = _classTemplate;
		_classTemplate.replace("<classname>", className);
		_project.addClass(className + '.' + _settings.classExtension, callback);
	}

	this.renderTab = function(name)
	{
		var tab = _.find(_tabs, function(tab){ return tab.name == name; }));
		if (tab) {
			_editor.setSession(tab.editSession);
		}
		return tab;
	}

	$.getJSON(_editorSettingsFile, function(data){
		_settings = data;
		loadFile(_projectFileTemplateFile, function(data){
			_projectFileTemplate = data;
			loadFile(_classTemplateFile, function(data){
				_self._registerEvents();
				callback();
			});
		});
	});
}
