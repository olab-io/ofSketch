ofSketch
========

This is very early work toward a simple sketching environment for github.com/openFrameworks. It requires no dependencies such as node.js / php. It only requires that openFrameworks can compile and run on the machine. Besides allowing quick and easy sketching on the desktop, ofSketch can be launched on remote machines (such as a Raspberry Pi) allowing the user to quickly code on the Raspberry Pi locally without ssh / terminal / smb etc. The main ingredients for ofSketch include:
New openFrameworks Makefile System + http://github.com/bakercp/ofxHTTP (file server / WebSockets) + Ace http://ace.c9.io/ editor.

Video here: https://vimeo.com/75557811

Requires:

https://github.com/bakercpofxHTTP
https://github.com/bakercpofxMediaType
https://github.com/bakercpofxJSON
