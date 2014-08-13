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
            containment: '#ace-wrapper',
            handles: 's',   
            resize: resize
        });

        resize();

        function resize() {
            
            $('#console').height($('#ace-wrapper').height() -($('#editor-container').height()));
            sketchEditor.resize();
            consoleEmulator.resize();

        }
    }

    function onWindowResize() {

        // if ($(window).width() <= 580) {
        //     $('#toolbar button').addClass('btn-sm');
        // } else {
        //     console.log('fired');
        //     $('#toolbar button').removeClass('btn-sm');
        // }
        
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
        else if (evt.method == "requestProjectClosed")
        {
            if (evt.params.clientUUID != CLIENT_UUID &&
                evt.params.projectName == sketchEditor.getProject().getName()) {
                window.close();
            }
        }
        else if (evt.method == "appExit") {
            window.close();
        }

        console.log(evt);
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

        } else if (evt.method == "taskQueued") {
            // TODO: add a task for the uuid

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
                        // compileError();
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

                // if message is compilation error
                if (!_.isUndefined(evt.params.compileError)) {
                    sketchEditor.annotate(evt.params.compileError);
                }

                consoleEmulator.log(evt.params.message + '\n');
            }
    
        } else {

            console.log("Unknown Task Queue method.");
            console.log(evt);
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
        sketchEditor.setDisabled(true);
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

            sketchEditor.getProjectList(function(result){
                var match = _.findWhere(result, { projectName: project });
                if (match) {
                    sketchEditor.loadProject(project, function(result) {

                        $('title').text(project);

                        fileUploader.init(project);

                        if (result.alreadyOpen == true){
                            $('#project-already-open-modal').modal('show');
                            sketchEditor.setDisabled(true);
                        }
                    }, loadError);
                } else {
                    sketchEditor.loadTemplateProject(function(){
                        $('#project-not-found-modal').modal('show');
                    }, loadError);
                }
            }, function(err) {
                console.log("Error requesting project list: ");
                console.log(err);
            });

        } else {

            sketchEditor.loadTemplateProject(function(){}, loadError);
        }
    }

    function run()
    {
        if (!sketchEditor.isDisabled()) {

            if (!sketchEditor.getProject().isTemplate()) {
                sketchEditor.saveProject(function() {
                    runAlert();
                    consoleEmulator.clear();
                    sketchEditor.clearAnnotations();
                    if (sketchEditor.isRunning()) {
                        sketchEditor.stop(function(){
                            sketchEditor.compile(function(){});
                        });
                    } else {
                        sketchEditor.compile(function(){});
                    }
                    
                }, saveError);
            } else {
                $('#name-project-modal').modal();
            }
        }
    }

    function save()
    {
        if (!sketchEditor.isDisabled()) {
            
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
                
                linkElement.on('click', function(){
                    $('#open-project-modal').modal('hide');
                });

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
    $('#toolbar li a, #export-project-modal .list-group a, #log-levels li a, .file-tab a, #new-class a, .action-menu li a').on('click', function(e) {
        e.preventDefault(); 
    });

    $(window).on('beforeunload', function(){
        sketchEditor.notifyProjectClosed();
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

    $('.needs-validation').on('');

    var systemInfo = {
        userAgent:  navigator.userAgent
    }

    var alertTimeout;
    var alertBox;
    var aceWrapperHeight;
    var compileSuccess = false; // this is a terrible global var. Get it out ASAP.
    var inputRegex = /^[A-Za-z0-9_-]+$/;

    JSONRPCClient = new $.JsonRpcClient({ 
            ajaxUrl: getDefaultPostURL(),
            socketUrl: getDefaultWebSocketURL(), // get a websocket for the localhost
            onmessage: onWebSocketMessage,
            onopen: onWebSocketOpen,
            onclose: onWebSocketClose,
            onerror: onWebSocketError
        });

    alertBox = $('#editor-messages');
    alertBox.hide();

    var ofSketchSettings = new OfSketchSettings();
    var consoleEmulator = new ConsoleEmulator();
    var fileUploader = new FileUploader();
    var logger = new Logger();

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
            onWindowResize();
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

        $('.upload-media').on('click', function(){

            if (!sketchEditor.getProject().isTemplate()) {
                $('#upload-media-modal').modal('show');               
            } else {
                $('#name-project-modal').modal('show');
            }
        });

        $('.export-project').on('click', function(){

            if (!sketchEditor.getProject().isTemplate()) {
                $('#export-project-modal').modal('show');               
            } else {
                $('#name-project-modal').modal('show');
            }
        });

        $('.include-addons').on('click', function(){

            if (!sketchEditor.getProject().isTemplate()) {

                sketchEditor.getAddonList(function(globalAddons) {
                    
                    sketchEditor.getProject().getAddonList(function(projectAddons){
                        
                        var coreAddonsList = $('#core-addons-list');
                        var contributedAddonsList = $('#contributed-addons-list');
                        
                        var hasContributedAddons = false;

                        // core addons at openFrameworks master.
                        // Eventually these should be seperated at the backend.
                        var coreAddons = [

                            "ofx3DModelLoader",
                            "ofxAccelerometer",
                            "ofxAndroid",
                            "ofxAssimpModelLoader",
                            "ofxEmscripten",
                            "ofxGui",
                            "ofxKinect",
                            "ofxMultiTouch",
                            "ofxNetwork",
                            "ofxOpenCv",
                            "ofxOsc",
                            "ofxSvg",
                            "ofxSynth",
                            "ofxThreadedImageLoader",
                            "ofxVectorGraphics",
                            "ofxXmlSettings",
                            "ofxiOS"
                        ];

                        coreAddonsList.empty();
                        contributedAddonsList.empty();

                        _.each(globalAddons, function(globalAddon) {

                            // if this addon is already included
                            var alreadyInProject = projectAddons.indexOf(globalAddon.name) != -1;
                            
                            var linkElement = $('<a href="#" data-included=' + alreadyInProject + ' class="list-group-item"><span class="addon-name">'
                                                 + globalAddon.name + '</span>'
                                                 +'<span class="include-marker" style="float: right; display: none" >Included</span></a>');
                            
                            if (alreadyInProject) {
                                linkElement.addClass('list-group-item-info');
                                linkElement.find('.include-marker').show();
                            }

                            linkElement.on('click', function(e){
                                
                                e.preventDefault();

                                var addon = globalAddon.name;
                                var isIncluded = $(this).data('included');

                                if (!isIncluded) {

                                    sketchEditor.getProject().addAddon(addon,
                                                                       function(){

                                                                        isIncluded = !isIncluded;
                                                                        linkElement.addClass('list-group-item-info');
                                                                        linkElement.find('.include-marker').show();
                                                                        linkElement.data('included', isIncluded);

                                                                       }, function(err){
                                                                        console.log(err);
                                                                       });
                                } else {

                                    sketchEditor.getProject().removeAddon(addon,
                                                                       function(){

                                                                        isIncluded = !isIncluded;
                                                                        linkElement.removeClass('list-group-item-info');
                                                                        linkElement.find('.include-marker').hide();
                                                                        linkElement.data('included', isIncluded);

                                                                       }, function(err){
                                                                        console.log(err);
                                                                       });
                                }
                                
                            });
        
                            if (coreAddons.indexOf(globalAddon.name) != -1) coreAddonsList.append(linkElement);
                            else {
                                
                                if (!hasContributedAddons) {
                                    hasContributedAddons = true;
                                    $('#contributed-addons-container').show();
                                }
                                
                                contributedAddonsList.append(linkElement);
                                
                            }
                        });
                    }, console.log);

                    $('#addons-modal').modal();

                }, function(err) {
                    console.log("Error requesting addon list: ");
                    console.log(err);
                });

            } else {
                $('#name-project-modal').modal();
            }
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

        $('#ofsketch-settings').on('click', function(e){
            
            ofSketchSettings.load(function(settings){
                
                $('#ofsketch-settings-port').attr('value', settings.server.port);
                $('#ofsketch-settings-buffer-size').attr('value', settings.server.bufferSize);
                $('#ofsketch-settings-openframeworks-directory').attr('value', settings.openFrameworksDir);
                $('#ofsketch-settings-project-directory').attr('value', settings.projectDir);

                $('#ofsketch-settings-modal').modal('show');

            }, function(){

                console.log('Error loading ofSketch settings');
                console.log(err);
            });
        });

        $('#save-ofsketch-settings').on('click', function(){

            var settings = ofSketchSettings.getData();

            settings.server.port = parseInt($('#ofsketch-settings-port').val());
            settings.server.bufferSize = parseInt($('#ofsketch-settings-buffer-size').val());
            settings.openFrameworksDir = $('#ofsketch-settings-openframeworks-directory').val();
            settings.projectDir = $('#ofsketch-settings-project-directory').val();

            ofSketchSettings.update(settings);
            ofSketchSettings.save(function(){
                $('#ofsketch-settings-modal').modal('hide');
            }, function(err){
                console.log('Error saving ofSketch settings');
                console.log(err);
            });

            console.log(settings.server.port);
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

        $('.close-project').on('click', function(){
            if (!$(this).hasClass('disabled')) {
                window.close();
            }
        });

        $('.quit-app').on('click', function(){
            sketchEditor.requestAppQuit(function(){
                window.close();
            });
        });

        $('#create-class').on('click', function() {

            $('#new-class-modal .alert').hide();
            var className = $('#new-class-name').val();
            if (!inputRegex.test(className)) {
                $('#new-class-modal .validation-error').show();
            } 
            else if (sketchEditor.getProject().isClassName(className)) {
                $('#new-class-modal .name-taken-error').show();
            }
            else {
                sketchEditor.createClass(className, function() {
                    
                    $('#new-class-modal').modal('hide');
                    $('#new-class-name').val('');

                    sketchEditor.selectTab(className);
                    sketchEditor.saveProject(function(){}, saveError)

                }, createClassError);
            }
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
            
            $('#rename-class-modal .alert').hide();

            var projectName = sketchEditor.getProject().getName();
            var tabName = sketchEditor.getSelectedTabName();
            var newClassName = $('#renamed-class-name').val();
            
            if (tabName != projectName) {

                if (!inputRegex.test(newClassName)) {
                    $('#rename-class-modal .validation-error').show();
                }
                else if(sketchEditor.getProject().isClassName(newClassName)) {
                    $('#rename-class-modal .name-taken-error').show();
                }
                else {

                    sketchEditor.renameClass(tabName, newClassName, function() {
                       
                       $('#rename-class-modal').modal('hide');
                       $('#renamed-class-name').val('');

                    }, function(err) {
                        console.log('Error renaming class: ');
                        console.log(err);
                    });
                }
            }
        });

        $('#name-project').on('click', function() {
            
            $('#name-project-modal .alert').hide();
            var projectName = $('#new-project-name').val();
            
            if (!inputRegex.test(projectName)) {
                $('#name-project-modal .validation-error').show();
            }
            else {

                sketchEditor.getProjectList(function(result){
        
                    var match = _.findWhere(result, { projectName: projectName });
                    if (match) {
                        $('#name-project-modal .name-taken-error').show();
                    } else {

                        sketchEditor.createProject(projectName, function() {
                           sketchEditor.saveProject(function(){
                                window.location.href = window.location.protocol 
                                                       + "//" + window.location.host 
                                                       + "/?project=" + encodeURIComponent(projectName);
                           }, saveError);
                        }, createProjectError);
                    }
                
                }, function(err){
                    console.log("Error getting project list:");
                    console.log(err);
                });
            } 
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

            $('#rename-project-modal .alert').hide();
            var newProjectName = $('#renamed-project-name').val();
            if (!sketchEditor.getProject().isTemplate()) {
                
                if (!inputRegex.test(newProjectName)) {
                    $('#rename-project-modal .validation-error').show();
                } else {

                    sketchEditor.getProjectList(function(result){

                        var match = _.findWhere(result, { projectName: newProjectName });
                        if (match) {
                            $('#rename-project-modal .name-taken-error').show();
                        } else {
                            sketchEditor.renameProject(newProjectName, function(){
                                sketchEditor.saveProject(function(){
                                    window.location.href = window.location.protocol 
                                                           + "//" + window.location.host 
                                                           + "/?project=" + encodeURIComponent(newProjectName);
                               }, saveError);
                            }, renameProjectError);
                        }
                    }, function(err){
                        console.log('Error getting project list: ');
                        console.log(err);
                    });
                }

            } else {
                // project is template
            }

        });

        $('#export-project-modal .list-group-item').on('click', function(){
            var platform = $(this).data('export-platform');
            console.log(platform);
            sketchEditor.exportProject(platform, function(data){
                console.log("Exported project.");
            }, function(err){
                console.log("Error exporting project:");
                console.log(err);
            });
        });

        $('#request-project-closed').on('click', function(){
            sketchEditor.requestProjectClosed(function(){
                $('#project-already-open-modal').modal('hide');
                sketchEditor.setDisabled(false);
            });
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

        $('#upload-media-modal').on('hidden.bs.modal', function() {
            fileUploader.reset();
        });


        // Enter keypress events for 1-line input in modals
        $('#new-project-name').on('keypress', function(evt){
            if ((evt.keyCode || evt.which) == 13) $('#name-project').click();
        });

        $('#renamed-project-name').on('keypress', function(evt){
            if ((evt.keyCode || evt.which) == 13) $('#rename-project').click();
        });

        $('#new-class-name').on('keypress', function(evt){
            if ((evt.keyCode || evt.which) == 13) $('#create-class').click();
        });

        $('#renamed-class-name').on('keypress', function(evt){
            if ((evt.keyCode || evt.which) == 13) $('#rename-class').click();
        });


        // logging
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
        
        parseURLParameters();

    });
});
