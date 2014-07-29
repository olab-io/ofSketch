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
	
	var _settings = new EditorSettings();
	var _tabs = [];
	var _project = undefined;
	var _isRunning = false;
	var _isCompiling = false;
	var _projectFileTemplate;
	var _classTemplate;
	var _autosaveTimeout;

	ace.require("ace/ext/language_tools");

	var _editor = ace.edit('editor');

	var _currentRunTaskId = undefined;

	var _applySettings = function(editorSettings)
	{

		_editor.getSession().setMode(_settings.getData().setMode);
		_editor.setTheme(_settings.getData().setTheme);
		_editor.setBehavioursEnabled(_settings.getData().setBehavioursEnabled);
		_editor.setDisplayIndentGuides(_settings.getData().setDisplayIndentGuides);
		_editor.setDragDelay(_settings.getData().setDragDelay);
		_editor.setFadeFoldWidgets(_settings.getData().setFadeFoldWidgets);
		_editor.setFontSize(_settings.getData().setFontSize);
		_editor.setHighlightActiveLine(_settings.getData().setHighlightActiveLine);
		_editor.setHighlightGutterLine(_settings.getData().setHighlightGutterLine);
		_editor.setHighlightSelectedWord(_settings.getData().setHighlightSelectedWord);
		_editor.getSession().setNewLineMode(_settings.getData().setNewLineMode);
		_editor.setOverwrite(_settings.getData().setOverwrite);
		_editor.setPrintMarginColumn(_settings.getData().setPrintMarginColumn);
		_editor.setReadOnly(_settings.getData().setReadOnly);
		_editor.setScrollSpeed(_settings.getData().setScrollSpeed);
		_editor.setShowFoldWidgets(_settings.getData().setShowFoldWidgets);
		_editor.setShowInvisibles(_settings.getData().setShowInvisibles);
		_editor.setShowPrintMargin(_settings.getData().setShowPrintMargin);
		_editor.getSession().setTabSize(_settings.getData().setTabSize);
		_editor.getSession().setUseSoftTabs(_settings.getData().setUseSoftTabs);
		_editor.setWrapBehavioursEnabled(_settings.getData().setWrapBehavioursEnabled);
		_editor.getSession().setWrapLimit(_settings.getData().setWrapLimit);
		_editor.getSession().setUseWrapMode(_settings.getData().setUseWrapMode);

		_editor.setOptions({
        	enableBasicAutocompletion: true,
        	enableSnippets: true
    	});
	}

	var _registerEvents = function()
	{

		// autocomplete trigger
		_editor.commands.on("afterExec", function(e){
		     if (e.command.name == "insertstring"&&/^[\w.]$/.test(e.args)) {
		         _editor.execCommand("startAutocomplete");
		     }
		});

		//keyboard overrides
		_editor.commands.addCommand({
		    name: 'Ctrl-sOverride',
		    bindKey: {win: 'Ctrl-R'},
		    exec: function(editor) {
		    	run(); //WARNING: this is a global variable
		    }
		});

		_editor.commands.addCommand({
		    name: 'Ctrl-oOverride',
		    bindKey: {mac: 'Ctrl-O'},
		    exec: function(editor) {
		    	openProject(); //WARNING: this is a global variable
		    }
		});

		_editor.commands.addCommand({
		    name: 'Show Editor Settings',
		    bindKey: {mac: 'Command-,', win: 'Ctrl-,'},
		    exec: function(editor) {
		    	_self.showSettingsMenu();
		    }
		});

		// autosave
        _editor.on('change', function(){
        	clearTimeout(_autosaveTimeout);
        	_autosaveTimeout = setTimeout(function(){
	            if (_self.projectLoaded() &&
	            	!_project.isTemplate() &&
	                _project.needsSave()) {
	                _self.saveProject(function(){
	                	console.log('Project Autosaved');
	                }, function(){}); 
	            }
	        }, 1000 * 20); // Autosave every 20 seconds
    	});
		
	}

	var _registerTabEvent = function(tabElement)
	{
		$(tabElement).on('click', function(){
			_self.selectTab($(this).text());
		});
	}

	var _initTabs = function()
	{
		_tabs = [];

		var projectFile = _project.getProjectFile();
		_addTab(getPrettyFileName(projectFile.fileName),
				projectFile.fileName,
				true,
				new ace.createEditSession(_project.getProjectFile().fileContents,
											 _settings.getData().setMode));

		var classes = _project.getClasses();
		_.each(classes, function(c){
			_addTab(getPrettyFileName(c.fileName),
					c.fileName,
					false,
				new ace.createEditSession(c.fileContents, _settings.getData().setMode));
		});

		_resizeTabs(true);
		_self.selectTab(_project.getName());

	}

	var _addTab = function(name, fileName, isProjectFile, editSession)
	{
		var tabElement = $('<li class="file-tab"><a href="#" onclick="return false;">' + name + '</a></li>');
		editSession.on('change', function(){
			tabElement.addClass('unsaved');
			_project.setNeedsSave(true);
		});

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
		$('#tab-bar li:last').prev().prev().after(tabElement);
	}

	var _renderTab = function(name)
	{
		var tab = _.find(_tabs, function(tab){ return tab.name == name; });
		if (tab) {
			_editor.setSession(tab.editSession);
		}
		return tab;
	}

	var _renameTab = function(name, newName)
	{
		// TODO: come back and change this to use _.without
		_.each(_tabs, function(tab, i) {
			if (tab.name == name) {
				_tabs[i].name = newName;
				_tabs[i].fileName = newName + ".sketch";
				_tabs[i].tabElement.find('a').text(newName);
			}
		});
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

	var _resizeTabs = function(noResizeEvent)
	{

		var tab; // the tab to move
		var debounce = 5;
        var containerWidth = $('#tab-bar').width();
        var combinedTabWidth = 0;
        var lastTabWidth = 0;

        $('#tab-bar').children()
                     .each(function(){
                        combinedTabWidth += $(this).width();
                        lastTabWidth = $(this).width();
                     });
        
        if (combinedTabWidth + debounce >= containerWidth) {
        	tab = $('.file-tab:not(#tab-dropdown .file-tab):last');
            tab.prependTo('#tab-dropdown');      
            if (noResizeEvent && combinedTabWidth - lastTabWidth + debounce >= containerWidth) {
            	_resizeTabs(noResizeEvent);
            }
        } else {
            tab = $('#tab-dropdown .file-tab:first');
            $('#tab-bar li:last').prev().prev().after(tab);
            combinedTabWidth += tab.width();
            if (combinedTabWidth >= containerWidth) {
                _resizeTabs();
            }
        }

        if ($('#tab-dropdown').is(':empty')) $('#tab-dropdown-button').hide();
        else $('#tab-dropdown-button').show();

	}

	var _getEditorSettings = function()
	{
		// Omitted as of now:
		// "setVScrollBarAlwaysVisible": "",
		// "setHScrollBarAlwaysVisible": "",
		// "setOptions": "",
		// "setUseWorker": ""
		// "setShowGutter": _editor.getShowGutter(),
		// "setKeyboardHandler": _editor.getKeyboardHandler(),
		
		return {
			setMode: _editor.getSession().getMode().$id,
			setTheme: _editor.getTheme(),
			setBehavioursEnabled: _editor.getBehavioursEnabled(),
			setDisplayIndentGuides: _editor.getDisplayIndentGuides(),
			setDragDelay: _editor.getDragDelay(),
			setFadeFoldWidgets: _editor.getFadeFoldWidgets(),
			setFontSize: _editor.getFontSize(),
			setHighlightActiveLine: _editor.getHighlightActiveLine(),
			setHighlightGutterLine: _editor.getHighlightGutterLine(),
			setHighlightSelectedWord: _editor.getHighlightSelectedWord(),
			setNewLineMode: _editor.getSession().getNewLineMode(),
			setOverwrite: _editor.getOverwrite(),
			setPrintMarginColumn: _editor.getPrintMarginColumn(),
			setReadOnly: _editor.getReadOnly(),
			setScrollSpeed: _editor.getScrollSpeed(),
			setShowFoldWidgets: _editor.getShowFoldWidgets(),
			setShowInvisibles: _editor.getShowInvisibles(),
			setShowPrintMargin: _editor.getShowPrintMargin(),
			setTabSize: _editor.getSession().getTabSize(),
			setUseSoftTabs: _editor.getSession().getUseSoftTabs(),
			setWrapBehavioursEnabled: _editor.getWrapBehavioursEnabled(),
			setWrapLimit: _editor.getSession().getWrapLimit(),
			setUseWrapMode: _editor.getSession().getUseWrapMode()
		};
	}

	this.loadProject = function(projectName, onSuccess, onError)
	{
		_project = new Project(projectName, function(result) {

			_initTabs();
			onSuccess(result);

		}, onError);
	}

	this.loadTemplateProject = function(onSuccess, onError)
	{
		_project = new Project('', function(result) {

			_initTabs();
			onSuccess(result);

		}, onError,
		true); // signifies isTemplate
	}

	this.saveProject = function(onSuccess, onError)
	{
		_updateProject();
		_project.save(function(){
			_.each(_tabs, function(tab){
				tab.tabElement.removeClass('unsaved');
			});
			onSuccess();
		}, onError);
	}

	this.createProject = function(projectName, onSuccess, onError)
	{
		var oldName = _project.getName();
		_project.assignName(projectName);
		_renameTab(oldName, projectName);
		_updateProject();
		_project.create(onSuccess, onError);
	}

	this.deleteProject = function(onSuccess, onError)
	{
		JSONRPCClient.call('delete-project', 
        					{ projectName: _project.getName(),
        					  clientUUID: CLIENT_UUID },
					        onSuccess,
					        onError);
	}

	this.renameProject = function(newProjectName, onSuccess, onError)
	{
		var oldProjectName = _project.getName();
		_project.rename(newProjectName, function(result){
			console.log("renaming tab");
			_renameTab(oldProjectName, newProjectName);
			onSuccess(result);
		
		}, onError);
	}

	this.getProject = function()
	{
		return _project;
	}

	this.projectLoaded = function()
	{
		return !_.isUndefined(_project);
	}

	this.compile = function(onSuccess, onError)
	{
		JSONRPCClient.call('compile-project', 
        					{ projectName: _project.getName() },
					        function(result) {
					        	_currentRunTaskId = result;
					        	_self.setCompiling(true);
					            onSuccess(result);
					        },
					        function(error) {
					            onError(error);
					        });
	}

	this.run = function(onSuccess, onError)
	{
		JSONRPCClient.call('run-project', 
        					{ projectName: _project.getName() },
					        function(result) {
					        	_currentRunTaskId = result;
					        	_self.setRunning(true);
					            onSuccess(result);
					        },
					        function(error) {
					            onError(error);
					        });
	}

	// commands a running project to stop
	this.stop = function(onSuccess, onError)
	{
		if (_currentRunTaskId != undefined)
		{
			JSONRPCClient.call('stop', 
					{ taskId: _currentRunTaskId },
			        function(result) {
			            onSuccess(result);
			            _currentRunTaskId = undefined;
			            _self.setCompiling(false);
			            _self.setRunning(false);
			        },
			        function(error) {
			            onError(error);
			        });
		}
	}

	// notifies that a running project has stopped
	this.notifyProjectStopped = function()
	{
		_currentRunTaskId = undefined;
		_self.setRunning(false)
	}

	this.createClass = function(className, onSuccess, onError)
	{	
		_project.createClass(className, function(classFile) {
			console.log('creating class');
			_addTab(classFile.name, 
					classFile.fileName, 
					false, 
					new ace.createEditSession(classFile.fileContents, 
										_settings.getData().setMode));
			_resizeTabs(true);
			onSuccess(); // should I pass result object?
		}, onError);
	}

	this.deleteClass = function(className, onSuccess, onError)
	{	
		_project.deleteClass(className, function(result){
			var tab = _.findWhere(_tabs, { name: className });
			tab.tabElement.remove();
			_tabs = _.without(_tabs, tab);
			_resizeTabs(true);
			onSuccess(result);
		}, onError);
	}

	this.renameClass = function(className, newClassName, onSuccess, onError)
	{	
		_project.renameClass(className, newClassName, function(result){
			
			_renameTab(className, newClassName);

			onSuccess(result);
		}, onError);
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

	this.getProjectList = function(onSuccess, onError) {
		JSONRPCClient.call('get-project-list', 
        					{},
					        function(result) {
					            onSuccess(result);
					        },
					        function(error) {
					            onError(error);
					        });
	}

	this.isRunning = function()
	{
		return _isRunning;
	}

	this.setRunning = function(bool)
	{
		_isRunning = bool;
	}

	this.isCompiling = function()
	{
		return _isCompiling;
	}

	this.setCompiling = function(bool)
	{
		_isCompiling = bool;
	}

	this.getCurrentRunTaskId = function()
	{
		return _currentRunTaskId;
	}

	this.resize = function()
	{
		_editor.resize();
		_resizeTabs();
	}

	this.showSettingsMenu = function()
	{
		_editor.execCommand('showSettingsMenu');
		
		onSettingsMenuShow(function(){

			// hide select inputs
			$('[contains="setMode"').hide();
			$('[contains="setNewLineMode"').hide();
			$('[contains="setTabSize"]').hide();
			$('[contains="setUseSoftTabs"]').hide();
			$('[contains="setWrapLimit"]').hide();
			$('[contains="setOptions"]').hide();
			$('[contains="setBehavioursEnabled"]').hide();
			$('[contains="setKeyboardHandler"]').hide();
			$('[contains="setVScrollBarAlwaysVisible"]').hide();
			$('[contains="setHScrollBarAlwaysVisible"]').hide();
			$('[contains="setUseWorker"]').hide();
			$('[contains="setWrapBehavioursEnabled"]').hide();
			$('[contains="setUseWrapMode"]').hide();
			$('[contains="setReadOnly"]').hide();

			$('#setFontSize').on('change', function(){
				_editor.setFontSize(parseInt($(this).val()));
			});

			$('#ace_settingsmenu input, #ace_settingsmenu select').on('change', function(){
		
				_settings.update(_getEditorSettings());
				_settings.save(function(){
					console.log('success!');
				}, function(err){
					// console.log(err);
				});
				
			});
		});

		// recursively check if the menu has loaded
		function onSettingsMenuShow(callback) {
			setTimeout(function(){
				if ($('#ace_settingsmenu').size() > 0) callback();
				else onSettingsMenuShow(callback);
			}, 50);
		}
	}

	this.updateSettings = function(data)
	{
		_settings.update(data);
		_applySettings();
	}

	this.notifyProjectClosed = function(onSuccess, onError)
	{
		if (!_project.isTemplate())
		{
			JSONRPCClient.call('notify-project-closed', 
        					{ projectName: _project.getName() },
					        onSuccess,
					        onError);
		}
	}

	this.requestProjectClosed = function(onSuccess, onError)
	{
		JSONRPCClient.call('request-project-closed',
							{
								projectName: _project.getName(),
								clientUUID: CLIENT_UUID
							},
							onSuccess,
							onError);
	}

	_settings.load(function(data){
		
		_applySettings();
		_registerEvents();
		
		callback();

	}, function(){
		console.log('error loading editor settings');
	});
}
