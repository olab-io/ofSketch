// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
//                    2014 Brannon Dorsey <http://brannondorsey.com>
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

$(document).ready( function()
{
        // TODO: move this
    function handleLoggerEvent(evt) {
        console.log(evt);

        if (evt.method == "message")
        {
            // check log level ... ?
        }
    }

    function handleTaskQueueEvent(evt) {
        console.log(evt);
 
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

        } else if (evt.method == "taskFailed") {
            // TODO: remove the task with the uuid

        } else if (evt.method == "taskProgress") {
            // TODO: remove the task with the uuid

        } else if (evt.method == "taskMessage") {
            // TODO: remove the task with the uuid
            // update the message in the li with the message
        } else {
            console.log("Unknown Task Queue method.");
        }
    }

    function onWebSocketOpen(ws) {
        console.log("on open");
        console.log(ws);
    }

    function onWebSocketMessage(evt) {
        try {
            var json = JSON.parse(evt.data);

            if (json.module == "Logger") {
                handleLoggerEvent(json);
            } else if (json.module == "TaskQueue") {
                handleTaskQueueEvent(json);
            } else {
                console.log("Unknown Module: " + json.module);
                console.log(json);
            }
        } catch (e) {
            console.log(e);
            console.log("Unknown Websocket Data Type");
            console.log(evt.data);
        }
    }

    function onWebSocketClose() {
        console.log("on close");
    }

    function onWebSocketError() {
        console.log("on error");
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

    function createClassError(err) {
        console.log("Error creating class");
        alertMessage('Error Creating Class!', '', 'alert-danger');
    }

    function saveAlert(message, subMessage) {
        var m = message || "Project saved!";
        var s = subMessage || '';
        alertMessage(m, s, "alert-success");
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
        alertBox.show();
        alertTimeout = setTimeout(function(){
            alertBox.hide();
        }, 2000);
    }

    function parseURLParameters()
    {
        var project = getURLParameter('project');
        if (project) {
            sketchEditor.loadProject(project, function() {
                console.log("Project loaded!");
            }, loadError);
        } else {
            sketchEditor.loadTemplateProject(function() {
                console.log("Template project loaded!");
            }, loadError);
        }
    }
    
    var alertTimeout;

    JSONRPCClient = new $.JsonRpcClient({ 
            ajaxUrl: getDefaultPostURL(),
            socketUrl: getDefaultWebSocketURL(), // get a websocket for the localhost
            onmessage: onWebSocketMessage,
            onopen: onWebSocketOpen,
            onclose: onWebSocketClose,
            onerror: onWebSocketError
        });

    var alertBox;
    alertBox = $('#editor-messages.alert');
    alertBox.hide();

    var consoleEmulator = new ConsoleEmulator();

    var sketchEditor = new SketchEditor(function() {

        $('#toolbar li a, .file-tab a, #new-class a, .action-menu li a').on('click', function(e) {
            e.preventDefault(); 
        });

        $('.nav-tabs .dropdown').on('click', function(){
            
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
            
            if (!sketchEditor.getProject().isTemplate()) {
                sketchEditor.saveProject(function() {
                    runAlert();
                    sketchEditor.run(function(){}, runError);
                }, saveError);
            } else {
                $('#name-project-modal').modal();
            }
        });

        $('#toolbar-stop').on('click', function() {

        });

        $('#toolbar-save').on('click', function() {
            if (sketchEditor.projectLoaded()) {
                if (!sketchEditor.getProject().isTemplate()) {
                    sketchEditor.saveProject(function() {
                        saveAlert();
                    }, saveError);
                } else {
                    $('#name-project-modal').modal();
                }   
            }
        });

        $('.open-project').on('click', function() {
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
        });

        $('.new-project').on('click', function() {
            window.open(window.location.protocol + '//' + window.location.host, '_blank');
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
                    sketchEditor.saveProject(function(){
                        saveAlert();
                    }, saveError)}, createClassError);
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
                    console.log("RENAME: renamed...");
                    sketchEditor.saveProject(function(){
                        console.log("RENAME: saved...");
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

        parseURLParameters();

    });
});
