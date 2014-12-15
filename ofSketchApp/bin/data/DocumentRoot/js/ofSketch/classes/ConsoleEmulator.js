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

function ConsoleEmulator() {
  var _self = this;

  var _editor = ace.edit('console');

  var _applySettings = function() {
    _editor.setTheme('ace/theme/twilight');
    _editor.setHighlightActiveLine(false);
    _editor.setShowPrintMargin(false);
    _editor.setReadOnly(true);
    _editor.getSession().setUseWrapMode(true);
    _editor.renderer.setShowGutter(false);
    _editor.renderer.$cursorLayer.element.style.opacity = 0
  }

  var _registerEvents = function() {
    // disable settings menu
    _editor.commands.addCommand({
      name: 'Show Editor Settings',
      bindKey: {
        mac: 'Command-,',
        win: 'Ctrl-,'
      },
      exec: function(editor) {}
    });
  }

  this.log = function(text) {
    _editor.getSession().insert(_self.getEndPosition(), text);
  }

  this.placeCursorAtEnd = function() {
    _editor.moveCursorToPosition(_self.getEndPosition());
  }

  this.getEndPosition = function() {
    var lastRow = _editor.getSession().getLength();
    var lastColumn = _editor.getSession().getLine(lastRow).length;
    return {
      row: lastRow,
      column: lastColumn
    };
  }

  this.clear = function() {
    _editor.setValue('');
  }

  this.resize = function() {
    _editor.resize();
  }

  _applySettings();
  _registerEvents();

  _editor.on('blur', function() {
    _self.placeCursorAtEnd();
  });
}
