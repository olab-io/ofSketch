// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//               2014 Brannon Dorsey <http://brannondorsey.com>
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
var JSONRPCClient;
var CLIENT_UUID = generateUUID();

$(document).ready( function()
{

    function resizeEditor() {

        // get the new ratio
        var editorRatio = $('#editor-container').height()/aceWrapperHeight;

        aceWrapperHeight = window.innerHeight - $('#wrap').height() - 2;
        $('#ace-wrapper').css({height: aceWrapperHeight});

        $('#editor-container').height(aceWrapperHeight * editorRatio);

        $('#editor-container').resizable({
            handles: 'n, s',   
            resize: resize
        });

        resize();

        function resize() {
            
            $('#console').height($('#ace-wrapper').height() -($('#editor-container').height()));
            sketchEditor.resize();
            consoleEmulator.resize();

        }
    }

    function checkVersion() {
        var releaseURL = "https://api.github.com/repos/olab-io/ofSketch/releases";

        $.getJSON(releaseURL, function(data) {

            if (data.length == 0) {
                console.log("No current releases.");
                return; // no releases
            }

            // Record the latest release.
            var latestRelease = data[0];

            // Get the cleaned semver string.
            var remoteVersion = semver.clean(latestRelease.tag_name);
            var localVersion = semver.clean(systemInfo.version.version);
            
            $('.local-version').text(localVersion);

            // Compare the semver number provided by the server to the remote.
            if (semver.gt(remoteVersion, localVersion))
            {
                var downloadURL = latestRelease.zipball_url;
                var htmlURL = latestRelease.html_url;
                var assets = latestRelease.assets;

                var target = systemInfo.version.target.toLowerCase();

                var binaryURL = "";

                for (var i = 0; i < assets.length; ++i) {
                    var asset = assets[i]

                    if (asset.name.indexOf(target) >= 0) {
                        // Minor hack because direct access requires header use.
                        // https://developer.github.com/v3/repos/releases/
                        binaryURL = (htmlURL + "/" + asset.name).replace("tag", "download");
                        break;
                    }
                }

                // Attempt to feed a binary url directly if one is available.
                if (binaryURL.length > 0)
                {
                    $('#remote-version-download-link').attr('href', binaryURL);
                }
                else
                {
                    $('#remote-version-download-link').attr('href', htmlURL);
                }

                $('.remote-version').text(remoteVersion);

                $('#version-alert').removeClass('hidden');

            }
        
        }).fail(function() {
            console.log("Unable to contact github for a version check.");
        });
    }

    function handleServerEvent(evt) {
        if (evt.method == "version")
        {
            // Add version information.
            systemInfo["version"] = evt.params;

            var versionString = "";
            versionString += systemInfo.version.version;
            versionString += " Target: " + systemInfo.version.target;
            versionString += " User-Agent: " + systemInfo.userAgent;

            $("#version").html(versionString);

            var issuePrefill = '\n\n#### User Info\n\n';
            issuePrefill += '- ofSketch ' + systemInfo.version.version + '\n';
            issuePrefill += '- Target: ' + systemInfo.version.target + '\n';
            issuePrefill += '- User-Agent: ' + systemInfo.userAgent + '\n';
            var link = $('#issue-link').attr('href');
            $('#issue-link').attr('href', link + '?body=' + encodeURIComponent(issuePrefill));

            checkVersion();
        }
        else if (evt.method == "addons")
        {
            console.log(evt.params);
        }
        else if (evt.method == "updateEditorSettings")
        {  
            // check that the update didn't come from this client 
            if (evt.params.clientUUID != CLIENT_UUID) {
                sketchEditor.updateSettings(evt.params.data);
            }
        }
        else if (evt.method == "updateProjectSettings")
        {

        }
    }

        // TODO: move this
    function handleLoggerEvent(evt) {
        // console.log(evt);
        if (evt.method == "message")
        {
            var level = evt.params.level;
            var module = evt.params.module;
            var message = evt.params.message;

            logger.log(level, module, message);
            // consoleEmulator.log(message + '\n');
        }
    }

    function handleTaskQueueEvent(evt) {
        
        // console.log(evt);
        // console.log(evt.params.message);
 
        if (evt.method == "taskList") {
            // TODO: this is a mess.
            // Needs to be filled out by someone with better HTML js skills :)

            // var li = $("<li/>", {
            //     class: 'list-group-item',
            //     id: 'uuidaiusdiudasduidasd'
            // }).appendTo($('#task-queue-list'));

            // var progressBarContainer = $("<div/>", {
            //     class: 'progress progress-striped active'
            // }).appendTo(li);

            // var progressBar = $("<div/>", {
            //     class: 'progress-bar',
            //     role: 'progressbar',
            //     style: 'width: 100%'
            // }).attr('aria-valuenow','100').attr('aria-valuemin', '0').attr('aria-valuemax', '100');

            // progressBar.appendTo(progressBarContainer);

            // var progressInfo = $("<span/>", {
            //     class: 'sr-only',
            //     text: '0% Complete'
            // }).appendTo(progressBarContainer);         

            // var button = $('<button/>', {
            //     type: 'button',
            //     class: 'btn btn-default btn-sm glyphicon glyphicon-remove',
            //     click: function() {
            //         console.log("Task Cancel clicked.");
            //     }
            // }).appendTo(li);

        } else if (evt.method == "taskStarted") {
            // TODO: add a task for the uuid

        } else if (evt.method == "taskCancelled") {
            // TODO: remove the task with the uuid

        } else if (evt.method == "taskFinished") {
            // TODO: remove the task with the uuid
            if (evt.params.uuid == sketchEditor.getCurrentRunTaskId()) {
                
                if (sketchEditor.isCompiling()) {
                 
                    sketchEditor.setCompiling(false);
                    if (compileSuccess) { // this is a terrible way to do this
                        consoleEmulator.clear();
                        sketchEditor.run(function(){}, runError);
                        compileSuccess = false;
                    } else {
                        compileError();
                    }
                }
            }

        } else if (evt.method == "taskFailed") {
            // TODO: remove the task with the uuid

        } else if (evt.method == "taskProgress") {
            // TODO: remove the task with the uuid

        } else if (evt.method == "taskMessage") {
            // TODO: remove the task with the uuid
            // update the message in the li with the message
            if (evt.params.uuid == sketchEditor.getCurrentRunTaskId()) {
                
                // this is a terrible hack
                if (evt.params.message.indexOf("make RunRelease") != -1) {
                    compileSuccess = true;
                }

                consoleEmulator.log(evt.params.message + '\n');
            }
    
        } else {
            console.log("Unknown Task Queue method.");
        }
    }

    function onWebSocketOpen(evt) {
        console.log("on open");
        console.log(evt);
    }

    function onWebSocketMessage(evt) {
        try {
            var json = JSON.parse(evt.data);

            if (json.module == "Logger") {
                handleLoggerEvent(json);
            } else if (json.module == "TaskQueue") {
                handleTaskQueueEvent(json);
            } else if (json.module == "Server") {
                handleServerEvent(json);
            } else {
                console.log("Unknown Module: " + json.module);
                console.log(json);
            }
        } catch (e) {
            console.log("Unknown Websocket Data Type");
            console.log(e);
            console.log(evt.data);
        }
    }

    function onWebSocketClose(evt) {
        // console.log("on close");
        // console.log(evt);
        $('#server-disconnected-modal').modal();
    }

    function onWebSocketError(evt) {
        console.log("on error");
        console.log(evt);
    }

    function saveError(err) {
        console.log("Error saving project");
        console.log(err);
        alertMessage('Save Error!', '', 'alert-danger');
    }

    function loadError(err) {
        console.log("Error loading project");
        console.log(err);
        alertMessage('Load Error!', '', 'alert-danger');
    }

    function createProjectError(err) {
        console.log('Error creating project: ');
        console.log(err);
    }

    function renameProjectError(err) {
        console.log('Error renaming project: ');
        console.log(err);
    }

    function runError(err) {
        console.log('Error running project: ');
        console.log(err);
    }

    function compileError(message, subMessage) {
        var m = message || "Error";
        var s = subMessage || '(see console for more info)';
        alertMessage(m, s, 'alert-danger');
    }

    function createClassError(err) {
        console.log("Error creating class");
        alertMessage('Error Creating Class!', '', 'alert-danger');
    }

    function saveAlert(message, subMessage) {
        // var m = message || "Project saved!";
        // var s = subMessage || '';
        // alertMessage(m, s, "alert-success");
    }

    function runAlert(message, subMessage) {
        var m = message || "Running Project...";
        var s = subMessage || '';
        alertMessage(m, s, "alert-info");
    }

    function alertMessage(message, subMessage, c) {

        clearTimeout(alertTimeout);
        alertBox.removeClass();
        alertBox.addClass('alert ' + c);
        alertBox.html('<strong>' + message + '</strong> ' + subMessage);
        alertBox.css({
            left:$(window).width()/2 - alertBox.width()/2,
            top: $(window).height()/2 - alertBox.height()/2
        });
        
        alertBox.show();
        alertTimeout = setTimeout(function(){
            alertBox.hide();
        }, 2500);
    }

    function parseURLParameters()
    {
        var project = getURLParameter('project');
        if (project) { 
            sketchEditor.loadProject(project, function() {
                $('title').text(project);
                console.log("Project loaded!");
            }, loadError);
        } else {
            sketchEditor.loadTemplateProject(function() {
                console.log("Template project loaded!");
            }, loadError);
        }
    }

    function run()
    {
        if (!sketchEditor.getProject().isTemplate()) {
            sketchEditor.saveProject(function() {
                runAlert();
                consoleEmulator.clear();
                sketchEditor.compile(function(){
                    // this callback is now above
                    // sketchEditor.run(function(){}, runError);
                }, runError);
            }, saveError);
        } else {
            $('#name-project-modal').modal();
        }
    }

    function save()
    {
         if (sketchEditor.projectLoaded()) {
            if (!sketchEditor.getProject().isTemplate()) {
                sketchEditor.saveProject(function() {
                    saveAlert();
                }, saveError);
            } else {
                $('#name-project-modal').modal();
            }   
        }
    }

    function openProject()
    {
        sketchEditor.getProjectList(function(result) {

            var projectList = $('#project-list');
            projectList.empty();
            _.each(result, function(project) {
                var linkElement = $('<a href="' + location.protocol + '//' + location.host + '/?project=' 
                                     + encodeURIComponent(project.projectName)
                                     + '" class="list-group-item" target="_blank">' + project.projectName + '</a>');
                projectList.append(linkElement);
            });

            $('#open-project-modal').modal();

        }, function(err) {
            console.log("Error requesting project list: ");
            console.log(err);
        });
    }

    // localization
    $('[data-localize]').localize('lang', { pathPrefix: "../../localization", skipLanguage: /^en/ });
    
    // prevent defaults
    $('#toolbar li a, #log-levels li a, .file-tab a, #new-class a, .action-menu li a').on('click', function(e) {
        e.preventDefault(); 
    });
    
    // key bindings
    $(document).on("keydown", function(e){

        // cmd-r, ctrl-r
        if ((e.which || e.keyCode) == 116 && (e.metaKey || e.ctrlKey) || 
            (e.which || e.keyCode) == 82 && (e.metaKey || e.ctrlKey)) {

            e.preventDefault(); 
            run();
        } 

        // cmd-s, ctrl-s
        if ((e.which || e.keyCode) == 83 && (e.metaKey || e.ctrlKey)) {
            
            e.preventDefault();
            save();
        }

        // cmd-o, ctrl-o
        if ((e.which || e.keyCode) == 79 && (e.metaKey || e.ctrlKey)) {
            
            e.preventDefault();
            openProject();
        }

    });

    // input validation
    $('.needs-validation').tooltip({
        title: 'A-Z, 0-9, _ and - allowed',
        trigger: 'manual',
        placement: 'top'
    });

    $('.needs-validation').on('mouseout', function(e){
        console.log('fired!')
        $(this).tooltip('hide');
    });
    
    $('.needs-validation').keypress(function(e){
        var code = e.which || e.keyCode;

        // 65 - 90 for A-Z and 97 - 122 for a-z 48 - 57 for 0-9 95 for _ 45 for - 46 for .
        if (!((code >= 65 && code <= 90) ||
              (code >= 97 && code <= 122) || 
              (code >= 48 && code <= 57) || 
               code == 95 || code == 45)) {
            e.preventDefault();
            $(e.currentTarget).tooltip('show');
        } else $(e.currentTarget).tooltip('hide');
    });

    var systemInfo = {
        userAgent:  navigator.userAgent
    }

    var alertTimeout;
    var alertBox;
    var aceWrapperHeight;
    var compileSuccess = false; // this is a terrible global var. Get it out ASAP.
    alertBox = $('#editor-messages');
    alertBox.hide();

    var logger = new Logger();

    JSONRPCClient = new $.JsonRpcClient({ 
            ajaxUrl: getDefaultPostURL(),
            socketUrl: getDefaultWebSocketURL(), // get a websocket for the localhost
            onmessage: onWebSocketMessage,
            onopen: onWebSocketOpen,
            onclose: onWebSocketClose,
            onerror: onWebSocketError
        });

    $('#set-log-level').addClass(logger.getLogLevelLabelClass(logger.getLogLevel()));
    $('#current-log-level').text(logger.getLogLevelName(logger.getLogLevel()));

    $('#log-levels li a').on('click', function() {
        
        var logLevel = parseInt($(this).data('log-level'));
        logger.setLogLevel(logLevel);

        var button = $('#set-log-level');
        button.removeClass();
        button.addClass('btn btn-xs dropdown-toggle ' + logger.getLogLevelLabelClass(logLevel));
        $('#current-log-level').text($(this).text());
    });

    var consoleEmulator = new ConsoleEmulator();

    var sketchEditor = new SketchEditor(function() {
        
        // editor resize stuff
        aceWrapperHeight = window.innerHeight - $('#wrap').height() - 2;
        $('#editor-container').height(aceWrapperHeight * 3/4);
        $('#console').height(aceWrapperHeight * 1/4);

        resizeEditor();

        // events
        $(window).resize(function(){
            sketchEditor.resize();
            resizeEditor();
        });
        
        $('#tab-bar .dropdown').on('click', function(){
            
            var tabName = sketchEditor.getSelectedTabName();
            if (tabName == sketchEditor.getProject().getName()) {
                $('.rename-class, .delete-class').addClass('disabled');
                $('.selected-tab-name').text('');
            } else {
                $('.rename-class, .delete-class').removeClass('disabled');
                $('.selected-tab-name').text(tabName);
            }
        });

        $('#toolbar-run').on('click', function() {
            run();
        });

        $('#toolbar-stop').on('click', function() {
            if (sketchEditor.isRunning() &&
                !$(this).hasClass('disabled')) {
                sketchEditor.stop(function(){
                    // callback
                }, runError);
            }
        });

        $('#toolbar-save').on('click', function() {
           save();
        });

        $('.open-project').on('click', function() {
            openProject();
        });

        $('.new-project').on('click', function() {
            window.open(window.location.protocol + '//' + window.location.host, '_blank');
        });

        $('#editor-settings').on('click', function(e){
            sketchEditor.showSettingsMenu();
        });

        // Modals
        $('.new-class').on('click', function() {
            if (!sketchEditor.getProject().isTemplate()) {
                $('#new-class-modal').modal();               
            } else {
                $('#name-project-modal').modal();
            }
            
        });

        $('.rename-class').on('click', function() {
            if (!$(this).hasClass('disabled')) {
                $('#rename-class-modal').modal();
            }
        });

        $('.delete-class').on('click', function() {
            if (!$(this).hasClass('disabled')) {
                $('#delete-class-modal').modal();
            }
        });

        $('.delete-project').on('click', function() {
            $('#delete-project-modal').modal();
        });

        $('.rename-project').on('click', function() {
            if (!sketchEditor.getProject().isTemplate()) {
                $('#rename-project-modal').modal();  
            } else {
                $('#name-project-modal').modal();
            }
        });

        $('#create-class').on('click', function() {
            
            var className = $('#new-class-name').val();
            if (!sketchEditor.getProject().isClassName(className)) {
                sketchEditor.createClass(className, function() {
                   
                    sketchEditor.selectTab(className);
                    sketchEditor.saveProject(function(){}, saveError)
                }, createClassError);
            } else {
                // class name already exists.
            }

            $('#new-class-name').val('');
        });

        $('#delete-class').on('click', function() {
            
            var projectName = sketchEditor.getProject().getName();
            var tabName = sketchEditor.getSelectedTabName();
            if (tabName != projectName) {
                sketchEditor.deleteClass(tabName, function() {
                    sketchEditor.selectTab(projectName);
                }, function(err) {
                    console.log("Error deleting class: ");
                    console.log(err)
                });
            }
        });

        $('#rename-class').on('click', function() {
            
            var projectName = sketchEditor.getProject().getName();
            var tabName = sketchEditor.getSelectedTabName();
            var newClassName = $('#renamed-class-name').val();
            
            if (tabName != projectName) {
                if (!sketchEditor.getProject().isClassName(newClassName)) {
                    sketchEditor.renameClass(tabName, newClassName, function() {
                       console.log('class renamed!');
                    }, function(err) {
                        console.log('Error renaming class: ');
                        console.log(err);
                    });
                } else {
                    // renamed class is already taken.
                }
            }

            $('#renamed-class-name').val('');
        });

        $('#name-project').on('click', function() {
            
            var projectName = $('#new-project-name').val();
            // TODO: validate name doesn't already exist below
            if (true) {
                sketchEditor.createProject(projectName, function() {
                   sketchEditor.saveProject(function(){
                        window.location.href = window.location.protocol 
                                               + "//" + window.location.host 
                                               + "/?project=" + encodeURIComponent(projectName);
                   }, saveError);
                }, createProjectError);
            } else {
                // Project name is already taken.
            }

            $('#new-project-name').val('');
        });

        $('#delete-project').on('click', function() {
            if (!sketchEditor.getProject().isTemplate()) {
                sketchEditor.deleteProject(function(){
                    navigateToNewProject();
                }, function(){

                });
            } else {
                navigateToNewProject();
            }

            function navigateToNewProject() {
                window.location.href = window.location.protocol 
                                               + "//" + window.location.host;
            }
        });

        $('#rename-project').on('click', function() {

            var newProjectName = $('#renamed-project-name').val();
            if (!sketchEditor.getProject().isTemplate()) {
                
                sketchEditor.renameProject(newProjectName, function(){
                    sketchEditor.saveProject(function(){
                        window.location.href = window.location.protocol 
                                               + "//" + window.location.host 
                                               + "/?project=" + encodeURIComponent(newProjectName);
                   }, saveError);
                }, renameProjectError);

            } else {
                // project is template
            }

            $('#renamed-project-name').val('');
        });

        // Modal Events
        $('#name-project-modal').on('shown.bs.modal', function () {
            $('#new-project-name').focus();
        });

        $('#rename-project-modal').on('shown.bs.modal', function () {
            $('#renamed-project-name').focus();
        });

        $('#new-class-modal').on('shown.bs.modal', function () {
            $('#new-class-name').focus();
        });

        $('#rename-class-modal').on('shown.bs.modal', function () {
            $('#renamed-class-name').focus();
        });

        parseURLParameters();

    });
});
