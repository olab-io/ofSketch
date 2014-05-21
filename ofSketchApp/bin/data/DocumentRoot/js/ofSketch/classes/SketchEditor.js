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

	var _applySettings = function()
	{
		_editor.setTheme(_settings.editorTheme);
	    _editor.getSession().setMode(_settings.editorMode);
	    _editor.getSession().setTabSize(_settings.tabSize);
	    document.getElementById('editor').style.fontSize = _settings.fontSize + 'px';
	    _editor.setHighlightActiveLine(_settings.editorHighlightActiveLine);
	    _editor.setShowPrintMargin(_settings.editorShowPrintMargin);
	    _editor.setShowInvisibles(_settings.editorShowInvisibles);
	    _editor.setBehavioursEnabled(_settings.editorAutopairCharacters);
	}

	var _registerEvents = function()
	{
		$('.file-tab a, #new-class a').on('click', function(e) {
		    e.preventDefault(); 
		});

		$('#new-class a').on('click', function(){});

		$('#create-class').on('click', function(){
			_self.createClassFile('MyNewClass', function(){});
		});
	}

	var _registerTabEvent = function(tabElement)
	{
		$(tabElement).on('click', function(){
			$(this).parent().find('li').removeClass("active");
			$(this).toggleClass('active');
			_self.renderTab($(this).text());
		});
	}

	var _addTab = function(name, fileName, isProjectFile, editSession)
	{
		var tab = {
			name: name,
			fileName: fileName,
			isProjectFile: isProjectFile,
			editSession: editSession
		}
		_tabs.push(tab);
		var tabElement = $('<li class="file-tab"><a href="#" onclick="return false;">' + tab.name + '</a></li>');
		_registerTabEvent(tabElement);
		if (tab.isProjectFile) tabElement.addClass('active');
		$('ul.nav-tabs li:last').prev().after(tabElement);
	}

	this.loadProject = function(projectName, callback)
	{
		_project = new Project(projectName, function()
		{
		
			_tabs = [];

			var projectFile = _project.getProjectFile();
			_addTab(getPrettyFileName(projectFile.fileName),
					projectFile.fileName,
					true,
					new ace.EditSession(_project.getProjectFile().fileContents,
												 _settings.editorMode));

			var classes = _project.getClasses();
			_.each(classes, function(c){
				_addTab(getPrettyFileName(c.fileName),
						projectFile.fileName,
						false,
					new ace.EditSession(c.fileContents, _settings.editorMode));
			});

			_self.renderTab(projectName);
			callback();
		});
	}

	this.saveProject = function(callback)
	{
		_project.save(callback);
	}

	this.createClassFile = function(className, callback)
	{	
		var classFile = {
			name: className,
			fileName: className + '.' + _settings.classExtension,
			fileContents: _classTemplate.replace("<classname>", className)
		}
		
		_project.addClassFile(classFile);
		_addTab(classFile.name, classFile.fileName, false, new ace.EditSession(classFile.fileContents, 
																			   _settings.editorMode));
		callback();
	}

	this.renderTab = function(name)
	{
		var tab = _.find(_tabs, function(tab){ return tab.name == name; });
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
				_classTemplate = data;
				_applySettings();
				_registerEvents();
				callback();
			});
		});
	});
}
