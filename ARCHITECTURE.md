# How does it work?

Here's a quick review of the software architecture (pasted from of-dev mailing list).  Basically it's openFrameworks from top to bottom.  On the back end:

- [ofxHTTP](https://github.com/bakercp/ofxHTTP)

ofxHTTP is an HTTP server / client package built which extends the default behaviors of Poco int a web server routing system with support for websockets, etc.  This is foundational and provides basic file service, etc.

- [ofxIO](https://github.com/bakercp/ofxIO)

ofxIO is a set of utilities for handling file IO, directory watching, etc.  This makes interactions with the disk easier.

- [ofxJSON](https://github.com/bakercp/ofxJSON)

Everything is done by sending JSON back and forth and this jsoncpp wrapper makes that easier.

- [ofxJSONRPC](https://github.com/bakercp/ofxJSONRPC)

This is a JSONRPC 2.0 compliant server built on top of ofxHTTP.  It allows us to interact with the jquery jsonrpc client on the front end.  It can handle round trip JSONRPC requests and notifications over POST requests and WebSockets (we are using the WebSockets exclusively).

- [ofxMediaType](https://github.com/bakercp/ofxMediaType)

This is a set of utilities for determining mime-types and is used by ofxHTTP to determine content-type headers so your browser presents files to you in a smarter way.

- [ofxSSLManager](https://github.com/bakercp/ofxSSLManager)

ofxSSLManager is a set of utilities that simplifies the POCO SSL stack.  A thorough discussion is here:

https://github.com/openframeworks/openFrameworks/pull/2773

This allows us to easily use SSL with the ofxHTTP server.

- [ofxTaskQueue](https://github.com/bakercp/ofxTaskQueue)

This is a multi-threaded task queue that allows us to submit "tasks" (extends Poco::Task) and submit them to a Poco::TaskManager.  The key is that task events are serviced in the main void update() loop which makes tasks that involve OpenGL easier to deal with.  We use this to submit Make tasks and carry out code indexing, etc.  Everything that needs threading goes through the task queue and reports its progress (serialized as JSON) back to the client(s).

On the front end:

We use https://github.com/Textalk/jquery.jsonrpcclient.js to communicate with the ofxJSONRPC server on the back end.  We use Ace (http://ace.c9.io/) for editing and Bootstrap for basic GUI components.  Then there is a lot magical custom javascript to define the client side app and keep the client and server states in sync, handle file post uploads keep task PID handles, etc.

Anyway, there are lots of similar solutions and even GUIS for IDEs (Google Coder, Ace, Arduino even has a new one for the TRE), but all of them require node.js, php, etc.  Originally all of this was built in pure openFrameworks so that it would be easier to teach coding with the Raspberry Pi.  Rather than installing node.js, and a million other packages, we can keep it in a standalone pure C++/openFrameworks/Poco and easily get people up and running with an IDE without an installer, etc.

Of course, there are a lot of other fun things that can be done with the modular backend components, but that will be left to the imagination.

Anyway, I hope that helps shed some light on it ... all of the addons are thoroughly (almost) documented with Doxygen, so feel free to poke around.
