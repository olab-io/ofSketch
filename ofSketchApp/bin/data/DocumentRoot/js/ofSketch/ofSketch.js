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

var JSONRPCClient; ///< The core JSONRPC WebSocket client.

function onWebSocketOpen(ws) {
    console.log("on open");
    console.log(ws);
}

function onWebSocketMessage(evt) {
    console.log("on message:");
    constle.log(evt.data);
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
}

function loadError(err) {
    console.log("Error loading project");
    console.log(err);
}

function createClassError(err) {
    console.log("Error creating class");
    console.log(err);
}

function saveAlert(message) {
    var m = message || "Project saved."
    console.log(m);
}

$(document).ready( function()
{
    JSONRPCClient = new $.JsonRpcClient({ 
            ajaxUrl: getDefaultPostURL(),
            socketUrl: getDefaultWebSocketURL(), // get a websocket for the localhost
            onmessage: onWebSocketMessage,
            onopen: onWebSocketOpen,
            onclose: onWebSocketClose,
            onerror: onWebSocketError
        });

    var sketchEditor = new SketchEditor(function() {

        $('#toolbar li a, .file-tab a, #new-class a, .action-menu li a').on('click', function(e) {
            e.preventDefault(); 
        });

        $('#toolbar-stop').on('click', function() {

        });

        $('#load-project').on('click', function() {
            // sketchEditor.loadProject();
        });

        $('#toolbar-save').on('click', function() {
            if (sketchEditor.projectLoaded()) {
                sketchEditor.saveProject(function() {
                    saveAlert();
                }, saveError);
            }
        });


        $('.new-class').on('click', function() {
            $('#newClassModal').modal();
        });
        
        $('#create-class').on('click', function() {
            var className = $('#new-class-name').val();
            
            sketchEditor.createClass(className, function() {
                
                sketchEditor.renderTab(className);
                sketchEditor.saveProject(function(){
                    saveAlert();
                }, saveError)}, createClassError);

            $('#new-class-name').val('');
        });

        $('#toolbar-run').on('click', function() {
            if (sketchEditor.projectLoaded()) {
                sketchEditor.saveProject(function() {
                    sketchEditor.run(function() {
                        console.log("Project running!");
                    });
                }, saveError);
            }
        });

        sketchEditor.loadProject("Bounce", function() {
            console.log("Project loaded!");
            }, function(error) {
                console.log("Error:");
                console.log(error);
            });
    });

});
