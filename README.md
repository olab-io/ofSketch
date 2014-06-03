# ofSketch v0.0.2

#### The barebones code editor (IDE) for [openFrameworks](http://openframeworks.cc).


![ofSketch Screenshot](http://brannondorsey.com/images/ofsketch.png)

ofSketch is being developed by [Brannon Dorsey](http://brannondorsey.com) and [Chris Baker](http://christopherbaker.net) at [openLab](http://olab.io) through the [Google Summer of Code](https://developers.google.com/open-source/soc/?csw=1) 2014 program.

**Warning:** ofSketch is currently in development. This is the first pre-release and it __IS NOT__ stable. In fact, lots of things don't work.

**Note:** It is likely that ofSketch development will somewhat halt during the month of June 2014. After that, we will be in high-gear to release a stable and feature packed version in August of 2014.

## Getting Started

### Download

[Download](http://brannondorsey.com/hidden/ofsketch_v0.0.2_osx_release.zip) the ofSketch app for OSX. All platforms will be supported soon.

ofSketch comes packaged with the following contents:

- ofSketch.app
- Projects/

For ofSketch to work properly, the "Projects" folder __must__ be located in the same folder as the ofSketch app.


### Running Examples

Double-click ofSketch to open the editor. From there you can create, load, save, and run ofSketch projects. The projects folder comes with a few example projects to get a feel for the editor. Click the open folder icon to select a project. Press the play button to run a project.

### Coding

As you may have noticed, ofSketch projects look a bit different than the openFrameworks/C++ code that you might be used to writing in Xcode or Code::Blocks. That is for a reason! Writing ofSketch code is akin to writing header-style C++ (don't worry if that doesn't make since), and should look similar to [Processing](http://processing.org/).

### Classes

Writing you're own classes is easy with ofSketch! Simply add a new tab by clicking the "+" icon. Anywhere else in your code that you wan't to use a custom class, you must include it at the top of that file like so:

```cpp
#include "MyClass.h"
```

### Using Addons

Currently, there is no elegant way to use ofxAddons with ofSketch. This feature is coming, but in the meantime all addons must be specified in the `addons.make` files. 

## Issues

ofSketch v0.0.2 is chock-full of disabled features and undoubtedly some pretty nasty bugs. Sticking to the philosophy "Release early, release often", we have decided to unveil this early prototype to the masses. There are tons of documented (and undocumented) problems. Check out the [issues page](https://github.com/brannondorsey/ofSketch/issues?state=open) for more info.

### Play Nice

- Currently, there is absolutely __NO__ input validation on __ANYTHING__. Be careful when naming projects, classes, etc... that you don't include illegal characters (like spaces) or clash names.
- Don't play with project files. Only modify projects through ofSketch, not your file browser (unless you are adding files to the `data/` folder).

### Feature Requests

Check out the [Todo list](https://github.com/brannondorsey/ofSketch/issues/1) to see what's coming. Want to propose something? We've got a [Proposed Features issue](https://github.com/brannondorsey/ofSketch/issues/2) for just that!

### Bug

Is something just downright wrong? Don't hesitate to submit an [issue](https://github.com/brannondorsey/ofSketch/issues?state=open).

## Developers

Building ofSketch from source requires the following add ons:

- [ofxHTTP](https://github.com/bakercp/ofxHTTP)
- [ofxIO](https://github.com/bakercp/ofxIO)
- [ofxJSON](https://github.com/bakercp/ofxJSON)
- [ofxJSONRPC](https://github.com/bakercp/ofxJSONRPC)
- [ofxMediaType](https://github.com/bakercp/ofxMediaType)
- [ofxSSLManager](https://github.com/bakercp/ofxSSLManager)
- [ofxTaskQueue](https://github.com/bakercp/ofxTaskQueue)

Hypothetically, ofSketch should work on all platforms when compiled from source, however this is untested.

## Links

- [openFrameworks documentation](http://openframeworks.cc/documentation/)
- [ofSketch proposal](https://docs.google.com/document/d/1YHX9v3iBmt7Y7WM1_NOZ7zus-woc8OE8lYB3epHrXII/edit?usp=sharing)
- [ofSketch hackpad](https://hackpad.com/collection/G6spIN6QLDT)
- [ofSketch wiki page](https://github.com/brannondorsey/ofSketch/wiki)

