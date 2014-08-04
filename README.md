# ofSketch Prereleases

#### The barebones code editor (IDE) for [openFrameworks](http://openframeworks.cc).

[![ScreenShot](http://brannondorsey.com/images/ofsketch_release_video_screenshot.png)](https://vimeo.com/97796242)

ofSketch is being developed by [Brannon Dorsey](http://brannondorsey.com) and [Christopher Baker](http://christopherbaker.net) at the [openLab](http://olab.io) through the [Google Summer of Code](https://developers.google.com/open-source/soc/?csw=1) 2014 program.

**Warning:** ofSketch is currently in development. This is the first pre-release and it is not completely stable.

## Getting Started

### Download

[Download](https://github.com/olab-io/ofSketch/releases/) the ofSketch app for your platform (all oF-compatible platforms will eventually be supported).

ofSketch comes packaged with the following contents:

- CHANGELOG.md
- CONTRIBUTING.md
- data
- LICENSE.md
- ofSketch.app
- README.md

**Note**: ofSketch prereleases use an openFrameworks v0.8.3 distribution bundled inside of the App's data folder itself. All code written in ofSketch must be oF v0.8.3 compatible. 

### Running Examples

Double-click ofSketch to open the editor. 

_Note: If OSX does not allow you to open the program due to security restrictions, right click on the ofSketch.app bundle and click "open".  See [this link](http://support.apple.com/kb/ht5290) for more information about Apple's security system._

From there you can create, load, save, and run ofSketch projects. The projects folder comes with a few example projects to get a feel for the editor. Click the open folder icon to select a project. Press the play button to run a project.

### Coding

As you may have noticed, ofSketch projects look a bit different than the openFrameworks/C++ code that you might be used to writing in Xcode or Code::Blocks. That is for a reason! Writing ofSketch code is akin to writing header-style C++ (don't worry if that doesn't make since), and should look similar to [Processing](http://processing.org/) code.

### Classes

Writing you're own classes is easy with ofSketch! Simply add a new tab by clicking the "+" icon. Anywhere else in your code that you wan't to use a custom class, you must include it at the top of that file like so:

```cpp
#include "MyClass.h"
```

### Using Addons

Currently, there is no elegant way to use ofxAddons with ofSketch. This feature is coming, but in the meantime all addons must be specified in the `addons.make` files of each project you mean to use them in. ofSketch uses its own openFrameworks distribution in the `bin/data/` folder, meaning only core addons work out of the box. To add an addon that ofSketch projects can use, you must place the add-on inside of ofSketch's `openFrameworks/addons` folder.

This is terribly annoying and we can assure you that there will be a better system for managing addons soon.

## Issues

ofSketch prereleases are chock-full of disabled features and undoubtedly some pretty nasty bugs. Sticking to the philosophy "Release early, release often", we have decided to unveil this early prototype to the masses. There are tons of documented (and undocumented) problems. Check out the [issues page](https://github.com/brannondorsey/ofSketch/issues?state=open) for more info.

### Play Nice

Don't edit project files in the `sketch/` or `src/` files of each project. Only modify projects through ofSketch (unless you are adding files to `bin/data/` folder).

### Feature Requests

Check out the [Todo list](https://github.com/brannondorsey/ofSketch/issues/1) to see what's coming. Want to propose something? We've got a [Proposed Features issue](https://github.com/brannondorsey/ofSketch/issues/2) for just that!

### Bug

Is something just downright wrong? Don't hesitate to submit an [issue](https://github.com/brannondorsey/ofSketch/issues?state=open).

## Developers

### Building from source

To build using openFrameworks v0.8.3 on OSX and Linux follow the steps below.

1. Clone the ofSketch repo into your `OF_ROOT/apps` folder, making sure that the `ofSketchApp/` folder inside of `ofSketch/` is two directories below your openFrameworks distribution:

		git clone https://github.com/olab-io/ofSketch.git
		cd ofSketch
		
2. Run the init repo script. This will download and install a trimmed down version of the latest oF release in the `bin/data` folder of your ofSketch build:

		./scripts/init_repo.sh
		
3. Clone the ofxAddon dependencies. This script will attempt to clone the required addons and then pull the latest changes. If you already have these addons, make sure that any of your changes are saved:

		./scripts/clone_addons.sh

4. Compile ofSketch:

		cd ofSketchApp
		make && make run


__Note__: OSX users may use the Xcode project instead of building from the command line in step 4.


### Info

Building ofSketch from source requires openFrameworks v0.8.3 and the following addons:

- [ofxHTTP](https://github.com/bakercp/ofxHTTP)
- [ofxIO](https://github.com/bakercp/ofxIO)
- [ofxJSON](https://github.com/bakercp/ofxJSON)
- [ofxJSONRPC](https://github.com/bakercp/ofxJSONRPC)
- [ofxMediaType](https://github.com/bakercp/ofxMediaType)
- [ofxSSLManager](https://github.com/bakercp/ofxSSLManager)
- [ofxTaskQueue](https://github.com/bakercp/ofxTaskQueue)

For more information, see the [ARCHITECTURE.md](ARCHITECTURE.md) document in this repository.


## Links

- [openFrameworks documentation](http://openframeworks.cc/documentation/)
- [ofSketch proposal](https://docs.google.com/document/d/1YHX9v3iBmt7Y7WM1_NOZ7zus-woc8OE8lYB3epHrXII/edit?usp=sharing)
- [ofSketch hackpad](https://hackpad.com/collection/G6spIN6QLDT)
- [ofSketch wiki page](https://github.com/brannondorsey/ofSketch/wiki)

