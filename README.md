# ofSketch Prereleases

#### The barebones code editor (IDE) for [openFrameworks](http://openframeworks.cc).

[![ScreenShot](http://brannondorsey.com/images/ofsketch_release_video_screenshot.png)](https://vimeo.com/97796242)

ofSketch is being developed by [Brannon Dorsey](http://brannondorsey.com) and [Christopher Baker](http://christopherbaker.net) at the [openLab](http://olab.io) through the [Google Summer of Code](https://developers.google.com/open-source/soc/?csw=1) 2014 program.

**Warning:** ofSketch is currently in development. This is the first pre-release and it is not completely stable.

## Installing Dependencies

### OSX

ofSketch requires Xcode, and the Xcode Command Line Tools to be installed on your computer. If you already have those, then you can skip to the [download section](#download).

1. Download and install Xcode from the App Store.
2. Download the Xcode Command Line Tools. If you are on OSX Mavericks, these should already be installed, and you can update them using Software Update. If you are on OSX 10.8, open Xcode and select Xcode > Preferences > Downloads from the menu bar. From there, install the Xcode Command Line Tools.

### Linux

ofSketch requires the same linux dependencies as a normal installation of openFrameworks.  If you have already followed the Linux installation instructions for a recent version of openFrameworks (since 0.8.0), you can skip to the [download section](#download).

To install the latest version of openFrameworks on Linux, follow the instructions  [here](http://openframeworks.cc/setup/linux-codeblocks/).

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

ofSketch uses its own openFrameworks distribution in the `bin/data/` folder, meaning only core addons work out of the box. To add an addon that ofSketch projects can use, you must place the add-on inside of ofSketch's `openFrameworks/addons` folder.

## Issues

ofSketch prereleases are chock-full of disabled features and undoubtedly some pretty nasty bugs. Sticking to the philosophy "Release early, release often", we have decided to unveil this early prototype to the masses. There are tons of documented (and undocumented) problems. Check out the [issues page](https://github.com/brannondorsey/ofSketch/issues?state=open) for more info.

### Play Nice

Don't edit project files in the `sketch/` or `src/` files of each project. Only modify projects through ofSketch (unless you are adding files to `bin/data/` folder).

### Feature Requests

Check out the [Todo list](https://github.com/brannondorsey/ofSketch/issues/1) to see what's coming. Want to propose something? We've got a [Proposed Features issue](https://github.com/brannondorsey/ofSketch/issues/2) for just that!

### Bug

Is something just downright wrong? Don't hesitate to submit an [issue](https://github.com/brannondorsey/ofSketch/issues?state=open).

## Links

- [openFrameworks documentation](http://openframeworks.cc/documentation/)
- [ofSketch proposal](https://docs.google.com/document/d/1YHX9v3iBmt7Y7WM1_NOZ7zus-woc8OE8lYB3epHrXII/edit?usp=sharing)
- [ofSketch hackpad](https://hackpad.com/collection/G6spIN6QLDT)
- [ofSketch wiki page](https://github.com/brannondorsey/ofSketch/wiki)

