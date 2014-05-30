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

	var _settings;
	var _tabs = [];
	var _project = undefined;
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
		
	}

	var _registerTabEvent = function(tabElement)
	{
		$(tabElement).on('click', function(){
			_self.selectTab($(this).text());
		});
	}

	var _addTab = function(name, fileName, isProjectFile, editSession)
	{
		var tabElement = $('<li class="file-tab"><a href="#" onclick="return false;">' + name + '</a></li>');
		var tab = {
			name: name,
			fileName: fileName,
			isProjectFile: isProjectFile,
			isSelected: false,
			editSession: editSession,
			tabElement: tabElement
		}

		_tabs.push(tab);
		_registerTabEvent(tabElement);
		if (tab.isProjectFile) tabElement.addClass('active');
		$('ul.nav-tabs li:last').prev().after(tabElement);
	}

	var _renderTab = function(name)
	{
		var tab = _.find(_tabs, function(tab){ return tab.name == name; });
		if (tab) {
			_editor.setSession(tab.editSession);
		}
		return tab;
	}

	var _updateProject = function()
	{
		var projectData = _project.getData();
		projectTab = _.find(_tabs, function(tab){ return tab.isProjectFile });
		projectData.projectFile.fileName = projectTab.fileName;
		projectData.projectFile.fileContents = projectTab.editSession.getValue();

		var classTabs = _.filter(_tabs, function(tab){ return !tab.isProjectFile });
		var classFiles = projectData.classes;

		_.each(classFiles, function(classFile, i)
		{
			var tab = _.findWhere(classTabs, { fileName: classFile.fileName });
			if (!_.isUndefined(tab)) {
				classFiles[i].fileContents = tab.editSession.getValue();
			}
		});
	}

	this.loadProject = function(projectName, onSuccess, onError)
	{
		_project = new Project(projectName, function(result) {
		
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
						c.fileName,
						false,
					new ace.EditSession(c.fileContents, _settings.editorMode));
			});

			_self.selectTab(projectName);
			onSuccess(result);

		}, onError);
	}

	this.saveProject = function(onSuccess, onError)
	{
		_updateProject();
		_project.save(onSuccess, onError);
	}

	this.getProject = function()
	{
		return _project;
	}

	this.projectLoaded = function()
	{
		return !_.isUndefined(_project);
	}

	this.run = function(onSuccess, onError)
	{
		JSONRPCClient.call('run', 
        					{ projectData: _project.getData() },
					        function(result) {
					            onSuccess(result);
					        },
					        function(error) {
					            onError(error);
					        });
	}

	this.createClass = function(className, onSuccess, onError)
	{	
		_project.createClass(className, function(classFile) {
			
			_addTab(classFile.name, 
					classFile.fileName, 
					false, 
					new ace.EditSession(classFile.fileContents, 
										_settings.editorMode));
			onSuccess(); // should I pass result object?
		}, onError);
	}

	this.deleteClass = function(className, onSuccess, onError)
	{	
		_project.deleteClass(className, function(result){
			var tab = _.findWhere(_tabs, { name: className });
			tab.tabElement.remove();
			_tabs = _.without(_tabs, tab);
			onSuccess(result);
		}, onError);
	}

	this.renameClass = function(className, newClassName, onSuccess, onError)
	{	
		_project.renameClass(className, newClassName, onSuccess, onError);
	}

	this.selectTab = function(name)
	{
		$('.file-tab').each(function(){
			
			$(this).removeClass('active');
			if ($(this).text() == name) {
				$(this).addClass('active');
				_renderTab($(this).text());
			}
		});
	}

	this.getSelectedTabName = function() {
		return $('.file-tab.active a').text();
	}

	$.getJSON(_editorSettingsFile, function(data){
		_settings = data;			
		_applySettings();
		_registerEvents();
		callback();
	});
}
