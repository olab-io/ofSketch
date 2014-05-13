// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
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


function makeCommandDataMessage(command, data) 
{
    txt = typeof txt !== 'undefined' ? data : null;

    return JSON.stringify(
                { 
                    command: command, 
                    data: txt
                }
            );
}

function onOpen(ws) 
{
    ofLogNotice("Connection Opened " + ws);
}

function onMessage(evt) 
{

    // console.log(evt.data);
    // messages are sent sent 
    var message = JSON.parse(evt.data);

    if(message.method == "setEditorSource") {
        editor.setValue(message.data.source);

        // console.log(message.error.code);

    } else {
        console.log("Unknown method: " + message.method);
    }

}

function onClose()
{
  console.log("Connection closed.");
}

function onError()
{
  console.log("Connection Error.");
}


function play()
{
    var message = makeCommandDataMessage("toolbar-play", editor.getValue());
    ws.send(message);
}

function stop()
{
    var message = makeCommandDataMessage("toobar-stop", editor.getValue());
    ws.send(message);
}

function save()
{
    var message = makeCommandDataMessage("toobar-save", editor.getValue());
    ws.send(message);
}

$(document).ready( function()
{

    // // create the new socket
    // var ws = new ofxHTTPBasicWebSocketClient();//['of','off']);

    // // // set callbacks
    // ws.setOnOpen(onOpen);
    // ws.setOnMessage(onMessage);
    // ws.setOnClose(onClose);
    // ws.setOnError(onError);

    // button controls
    $("#toolbar-play").click(function() {
        play();
    });
  
    // button controls
    $("#toobar-stop").click(function() {
        stop();
    });

    var editor = ace.edit("editor");
    editor.setTheme("ace/theme/twilight");
    editor.getSession().setMode("ace/mode/c_cpp");

    editor.commands.addCommand({
        name: 'save',
        bindKey: {win: 'Ctrl-s',  mac: 'Command-s'},
        exec: function(editor) {
            console.log("Just saved!");
        },
        readOnly: true // false if this command should not apply in readOnly mode
    });

    editor.commands.addCommand({
        name: 'run',
        bindKey: {win: 'Ctrl-R',  mac: 'Command-R'},
        exec: function(editor) {
          console.log("Just Ran!");
          },
        readOnly: true // false if this command should not apply in readOnly mode
    });

    editor.commands.addCommand({
        name: 'present',
        bindKey: {win: 'Shift-Ctrl-r',  mac: 'Shift-Command-r'},
        exec: function(editor) {
          console.log("Just presented!");
        },
        readOnly: true // false if this command should not apply in readOnly mode
    });
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
