## Building from source


1. (__NOTE__: If you already have a cloned copy of openFrameworks you may skip this step). Clone openFrameworks:

		git clone https://github.com/openFrameworks/openFrameworks
		git checkout master

2. Clone the ofSketch repo into your `OF_ROOT/apps` folder, making sure that the `ofSketchApp/` folder inside of `ofSketch/` is two directories below your openFrameworks distribution:

		git clone https://github.com/olab-io/ofSketch.git
		cd ofSketch
		
3. Run the init repo script. This will download and install a trimmed down version of the latest oF release in the `bin/data` folder of your ofSketch build:

		./scripts/init_repo.sh
		
4. Clone the ofxAddon dependencies. This script will attempt to clone the required addons and then pull the latest changes. If you already have these addons, make sure that any of your changes are saved:

		./scripts/clone_addons.sh

5. Compile ofSketch:

		cd ofSketchApp
		make && make run


__Note__: OSX users may use the Xcode project instead of building from the command line in step 5.


### Info

Building ofSketch from source requires openFrameworks master branch and the following addons:

- [ofxHTTP](https://github.com/bakercp/ofxHTTP)
- [ofxIO](https://github.com/bakercp/ofxIO)
- [ofxJSON](https://github.com/bakercp/ofxJSON)
- [ofxJSONRPC](https://github.com/bakercp/ofxJSONRPC)
- [ofxMediaType](https://github.com/bakercp/ofxMediaType)
- [ofxSSLManager](https://github.com/bakercp/ofxSSLManager)
- [ofxTaskQueue](https://github.com/bakercp/ofxTaskQueue)
- [ofxProjectGenerator](https://github.com/bakercp/ofxProjectGenerator)
- [ofxPugiXML](https://github.com/bakercp/ofxPugiXML)

For more information, see the [ARCHITECTURE.md](ARCHITECTURE.md) document in this repository.

