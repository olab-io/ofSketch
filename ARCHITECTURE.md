![ofSketch Architecture](http://brannondorsey.com/images/ofsketch_architecture.png)

# How does ofSketch work?

This document serves as a quick overview of the ofSketch software architecture. The ofSketch app is coded completely with openFrameworks and its core libs (Poco, etc). Using ofxHTTP, the app acts as a file server to deliver and communicate with the ofSketch browser GUI using [JSONRPC](http://www.jsonrpc.org/specification) via HTTP WebSockets.

## Client/Server Relationship

The ofSketch browser client acts as a controller, making commands and requests for the app to perform tasks that involve things like disk I/O, system calls (`make` tasks, etc), and other activities that the client doesn't have permission to execute on its own. We use two protocols to facilitate this communication, each with its own role.

### JSONRPC over WebSockets

Whenever the client requests the server to complete a task, it is done via the JSONRPC "remote procedure call" protocol. with ofSketch, a JSONRPC message is sent to the server for it to complete actions like:

- Saving, loading, creating, and renaming projects and class files
- Running and stopping projects
- Requesting lists of available projects and ofxAddons
- Saving and loading settings

JSONRPC methods are asynchronous and are guaranteed to return an error or success object to the client, which then handles displaying results to the user.

### Custom Protocol over WebSockets

Regular WebSocket connections are used whenever data is streamed from the server to the client without being explicitly requested by the client. There are a set of messages that the client is constantly listening for, and it acts accordingly whenever one of these messages arrive over the WebSocket. Some of the data sent via the Custom Protocol include:

- Version info
- Compilation Feedback
- Error Messages
- Console logging from projects that are running


## "Sketch" Format

Each ofSketch project folder has a `sketch/` directory where all source files created with ofSketch are saved. The files in this folder represent the exact files that were edited in the ofSketch app at the last state they were saved. These files have a `.sketch` extension, and have not undergone any file concatenation or string replacement to be turned into valid `.h` files for compilation. They are the raw files that are loaded to and saved from the ofSketch IDE.

ofSketch abstracts the reality that code is being written in header-style C++, where code implementation is written in the header (`.h`) file itself, and no implementation (`.cpp`) files are used.

## Compilation

When the play button is pressed in the ofSketch IDE, all project files are first saved in the `sketch/` directory. From there, they are transformed into `.h` files by the ofSketch app using templates found in `data/Resources/Templates/CompilerTemplates/`. ofSketch then runs a `make` like system call declaring the `OF_ROOT` directory, and other make flags, using the ofSketch Settings.  If compiler errors are present, those errors are parsed and sent to the IDE.  If compilation is successful, then the application binary is executed and the resulting process handle is passed to the IDE so that the application can be stopped using the stop button.  

## Addons

#### [ofxHTTP](https://github.com/bakercp/ofxHTTP)

ofxHTTP is an HTTP server / client package built which extends the default behaviors of Poco into a web server routing system with support for websockets, etc.  This is foundational and provides basic file service, etc.

#### [ofxIO](https://github.com/bakercp/ofxIO)

ofxIO is a set of utilities for handling file IO, directory watching, etc.  This makes interactions with the disk easier.

#### [ofxJSON](https://github.com/bakercp/ofxJSON)

Everything is done by sending JSON back and forth and this jsoncpp wrapper makes that easier.

#### [ofxJSONRPC](https://github.com/bakercp/ofxJSONRPC)

This is a JSONRPC 2.0 compliant server built on top of ofxHTTP.  It allows us to interact with the jquery jsonrpc client on the front end.  It can handle round trip JSONRPC requests and notifications over POST requests and WebSockets (we are using the WebSockets exclusively).

#### [ofxMediaType](https://github.com/bakercp/ofxMediaType)

This is a set of utilities for determining mime-types and is used by ofxHTTP to determine content-type headers so your browser presents files to you in a smarter way.

#### [ofxSSLManager](https://github.com/bakercp/ofxSSLManager)

ofxSSLManager is a set of utilities that simplifies the POCO SSL stack.  A thorough discussion is here:

<https://github.com/openframeworks/openFrameworks/pull/2773>

This allows us to easily use SSL with the ofxHTTP server.

#### [ofxTaskQueue](https://github.com/bakercp/ofxTaskQueue)

This is a multi-threaded task queue that allows us to submit "tasks" (extends Poco::Task) and submit them to a Poco::TaskManager.  The key is that task events are serviced in the main void update() loop which makes tasks that involve OpenGL easier to deal with.  We use this to submit Make tasks and carry out code indexing, etc.  Everything that needs threading goes through the task queue and reports its progress (serialized as JSON) back to the client(s).

#### [ofxPG](https://github.com/bakercp/ofxPG)

ofxPG is represents the core functionality of the openFrameworks Project Generator.  This addon is used to produce project files for more advanced IDEs such as Xcode, Visual Studio, etc.

#### [ofxPugiXML](https://github.com/bakercp/ofxPugiXML)

PugiXML is required by ofxPG and used because of its more advanced XPath-based XML search capabilities. 

## Front-end Libraries

#### [ACE Editor](http://ace.c9.io/#nav=about)

This is the embedded code editor that we use. It manages all code editing capabilities within ofSketch. We use a slightly customized ACE Editor for our syntax highlighting and auto complete. More info about that here:

<https://github.com/olab-io/ofSketch/issues/63>

#### [Bootstrap 3](http://getbootstrap.com/)

We use Bootstrap as our basic HTML, CSS, and JS framework.

#### [jQuery 1.11](http://jquery.com/)

For DOM manipulation.

#### [jQuery JSONRPCclient](https://github.com/Textalk/jquery.jsonrpcclient.js)

For JSONRPC communication to the server.

#### [jQuery-File-Upload](https://github.com/blueimp/jQuery-File-Upload)

jQuery plugin that we use for uploading files to each project's `data/` folder.

#### [Custom jQuery UI](http://jqueryui.com/)

For the draggable console.

#### [Underscore.js](http://underscorejs.org/)

For extended JavaScript functionality.

#### [Semver.js](https://github.com/npm/node-semver)

Used for parsing version info.
