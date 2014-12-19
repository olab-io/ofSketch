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
_data = {
    "projectFile": {
        "name": "myProject"
        "fileName": "myProject.sketch",
        "fileContents": ""
    },
    "classes": [
        {
            "name": "myProject"
            "fileName": "myClass.sketch",
            "fileContents": ""
        }
    ]
}
 */

function Project(projectName, onSuccess, onError, isTemplate) {
  var _self = this;
  var _needsSave = false;
  var _data;
  var _isTemplate;

  this.load = function(projectName, onSuccess, onError) {
    JSONRPCClient.call('load-project', {
        projectName: projectName
      },
      function(result) {
        _data = result.data;
        onSuccess(result);
      },
      function(error) {
        onError(error);
      });
  }

  this.loadTemplate = function(onSuccess, onError) {
    JSONRPCClient.call('load-template-project', {},
      function(result) {
        _data = result;
        onSuccess(result);
      },
      function(error) {
        onError(error);
      });
  }

  // used only if isAnonymous()
  this.create = function(onSuccess, onError) {
    JSONRPCClient.call('create-project', {
        projectData: _self.getData()
      },
      function(result) {
        _isTemplate = false;
        onSuccess(result);
      },
      function(error) {
        onError(error);
      });
  }

  this.save = function(onSuccess, onError) {
    JSONRPCClient.call('save-project', {
        projectData: _self.getData()
      },
      function(result) {
        _needsSave = false;
        onSuccess(result);
      },
      function(error) {
        onError(error);
      });
  }

  this.rename = function(newProjectName, onSuccess, onError) {
    JSONRPCClient.call('rename-project', {
        projectName: _self.getName(),
        newProjectName: newProjectName,
        clientUUID: CLIENT_UUID
      },
      function(result) {
        _data.projectFile.name = newProjectName;
        _data.projectFile.fileName = newProjectName + ".sketch";
        onSuccess(result);
      },
      function(err) {
        console.log("Rename project error:");
        console.log(err);
        onError(err);
      });
  }

  this.createClass = function(className, onSuccess, onError) {
    JSONRPCClient.call('create-class', {
        projectName: _self.getName(),
        className: className
      },
      function(result) {
        var classFile = result.classFile;
        if (!_self.hasClasses()) _data.classes = [];
        _self.getClasses().push(classFile);
        onSuccess(classFile);
      },
      function(error) {
        onError(error);
      });
  }

  this.deleteClass = function(className, onSuccess, onError) {
    JSONRPCClient.call('delete-class', {
        projectName: _self.getName(),
        className: className
      },
      function(result) {
        _data.classes = _.without(_self.getClasses(), _.findWhere(_self.getClasses(), {
          name: className
        }));
        onSuccess(result);
      },
      function(error) {
        onError(error);
      });
  }

  this.renameClass = function(className, newClassName, onSuccess, onError) {
    JSONRPCClient.call('rename-class', {
        projectName: _self.getName(),
        className: className,
        newClassName: newClassName
      },
      function(result) {

        var c = _.findWhere(_self.getClasses(), {
          name: className
        });
        c.name = newClassName;
        c.fileName = newClassName + '.sketch';
        onSuccess(result);
      },
      function(error) {
        onError(error);
      });
  }

  this.getAddonList = function(onSuccess, onError) {
    JSONRPCClient.call('get-project-addon-list', {
        projectName: _self.getName(),
      },
      function(result) {

        var addons = [];

        if (result.hasAddons) {
          addons = result.addons;
        }

        onSuccess(addons);
      },
      function(error) {
        onError(error);
      });
  }

  this.addAddon = function(addon, onSuccess, onError) {
    JSONRPCClient.call('add-project-addon', {
        projectName: _self.getName(),
        addon: addon
      },
      function(result) {
        onSuccess(result);
      },
      function(error) {
        onError(error);
      });
  }

  this.removeAddon = function(addon, onSuccess, onError) {
    JSONRPCClient.call('remove-project-addon', {
        projectName: _self.getName(),
        addon: addon
      },
      function(result) {
        onSuccess(result);
      },
      function(error) {
        onError(error);
      });
  }

  // used to assign the name of a template project only
  this.assignName = function(name) {
    if (_self.isTemplate()) {
      _data.projectFile.name = name;
      _data.projectFile.fileName = name + ".sketch";
    }
  }

  this.getName = function() {
    return _data.projectFile.name;
  }

  this.isClassName = function(className) {
    return _self.hasClasses() &&
      !_.isUndefined(_.findWhere(_self.getClasses(), {
        name: className
      }));
  }

  this.isTemplate = function() {
    return _isTemplate;
  }

  this.getProjectFile = function() {
    return _data.projectFile;
  }

  this.hasClasses = function() {
    return !_.isUndefined(_data.classes) && !_.isNull(_data.classes);
  }

  this.getClasses = function() {
    return _data.classes;
  }

  this.addClassFile = function(c) {
    _data.classes.push(c);
  }

  this.getData = function() {
    return _data;
  }

  this.needsSave = function() {
    return _needsSave;
  }

  this.setNeedsSave = function(bool) {
    _needsSave = bool
  }

  if (isTemplate) {
    _self.loadTemplate(onSuccess, onError);
    _isTemplate = true;
  } else {
    _self.load(projectName, onSuccess, onError);
    _isTemplate = false;
  }
}
