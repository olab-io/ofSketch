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

function addError(error)
{
    console.log(error);
}

function onWebSocketOpen(ws) 
{
    console.log("on open");
    console.log(ws);
}

function onWebSocketMessage(evt) 
{
    console.log("on message:");
    constle.log(evt.data);
}

function onWebSocketClose()
{
    console.log("on close");
}

function onWebSocketError()
{
    console.log("on error");
}

function play() {
    var $this = $(this);

    // Collect the information to send to the server.
    var params = {
        source: editor.getValue()
    };

    JSONRPCClient.call('play', 
        params,
        function(result) 
        {
            console.log(result);
        },
        function(error) 
        {
            addError(error);
        });
}

function stop() {
    var $this = $(this);
    JSONRPCClient.call('stop', 
        null,
        function(result) 
        {
            console.log(result);
        },
        function(error) 
        {
            addError(error);
        });
}

function load() {
    var $this = $(this);
    JSONRPCClient.call('load', 
        null,
        function(result) 
        {
            editor.setValue(result.source);
        },
        function(error) 
        {
            addError(error);
        });
}

$(document).ready( function()
{
    JSONRPCClient = new $.JsonRpcClient(
        { 
            ajaxUrl: getDefaultPostURL(),
            socketUrl: getDefaultWebSocketURL(), // get a websocket for the localhost
            onmessage: onWebSocketMessage,
            onopen: onWebSocketOpen,
            onclose: onWebSocketClose,
            onerror: onWebSocketError
        }
    );

    var sketchEditor = new SketchEditor(function()
    {


        $('.file-tab a, #new-class a, .action-menu li a').on('click', function(e) {
            e.preventDefault(); 
        });

        // button controls
        $('#toolbar-play').on('click', play);
        $('#toolbar-stop').on('click', stop);
        $('#load-project').on('click', function()
        {
            // sketchEditor.loadProject();
        });

        $('.new-class').on('click', function(){
            $('#newClassModal').modal();
        });
        
        $('#create-class').on('click', function()
        {
            var className = $('#new-class-name').val();
            sketchEditor.createClassFile(className, function(){});
            sketchEditor.renderTab(className);
            $('#new-class-name').val('');
        });

        sketchEditor.loadProject("HelloWorld", function()
        {
            console.log("Project loaded!");
        });

    });

        // editor.commands.addCommand({
        //     name: 'save',
        //     bindKey: {win: settings.keys.save.win,  mac: settings.keys.save.mac},
        //     exec: function(editor) {
                
        //     },
        //     readOnly: true // false if this command should not apply in readOnly mode
        // });

        // editor.commands.addCommand({
        //     name: 'run',
        //     bindKey: {win: settings.keys.run.win,  mac: settings.keys.run.mac},
        //     exec: function(editor) {
        //       //play();
        //     },
        //     readOnly: true // false if this command should not apply in readOnly mode
        // });

        // editor.commands.addCommand({
        //     name: 'present',
        //     bindKey: {win: 'Shift-Ctrl-r',  mac: 'Shift-Command-r'},
        //     exec: function(editor) {
        //       console.log("Just presented!");
        //     },
        //     readOnly: true // false if this command should not apply in readOnly mode
        // });
        // // connect to the websocket
        
        // ws.connect();

        // editor.getSession().on('change', 
        //     function(e) {
        //         console.log("on change: ");
        //         console.log(e);
        //     }
        // );

        // editor.getSession().selection.on('changeSelection', 
        //     function(e) {
        //         console.log("selection change: ");
        //         console.log(e);
        //     }
        // );

        // editor.getSession().selection.on('changeCursor', 
        //     function(e) {
        //         console.log("cursor change: ");
        //         console.log(e);
        //     }
        // );


        // $("#toolbar-play").disable();
        // $("#toolbar-stop").disable();
        // $("#toolbar-upload-media").disable();
        // $("#toolbar-new-project").disable();
        // $("#toolbar-open-project").disable();
        // $("#toolbar-import-project").disable();
        // $("#toolbar-export-project").disable();
        // $("#toolbar-settings").disable();



          // $('#toolbar-play').tooltip()
          // $('#toolbar-stop').tooltip()
          // $('#toolbar-upload-media').tooltip()
          // $('#toolbar-new-project').tooltip()
          // $('#toolbar-open-project').tooltip()
          // $('#toolbar-import-project').tooltip()
          // $('#toolbar-export-project').tooltip()
});
