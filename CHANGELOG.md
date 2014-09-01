- 0.3.2 (08-31-2014)
	+ Basic file upload support ([#16](https://github.com/olab-io/ofSketch/issues/16)).
	+ New Class template ([#102](https://github.com/olab-io/ofSketch/issues/102)).
	+ Responsive toolbar fix.
	+ 1-line input modals can now be submitted with the return key.
	
- 0.3.1 (08-17-2014)
	+ Basic dependency toolchain checking that alerts user of install instructions if they are missing dependencies ([#100](https://github.com/olab-io/ofSketch/issues/100])) ([#98](https://github.com/olab-io/ofSketch/issues/98])). 
	+ Added ROADMAP.md document.
	+ Added BUILDING.md document.
	
- 0.3.0 (08-07-2014)
	+ Adapt default ACE Settings Menu to support global ofSketch editor settings ([#42](https://github.com/olab-io/ofSketch/issues/42)).
	+ Added Raspberry Pi support, added workaround for ofSystem issue ([#58](https://github.com/olab-io/ofSketch/issues/58)).
	+ Fix bug that caused Editor to hang on "loading..." when a non-existent project was opened and added "Project Not Found" modal ([#80](https://github.com/olab-io/ofSketch/issues/80)).
	+ Project can now only be editable in one tab at a time ([#74](https://github.com/olab-io/ofSketch/issues/74)).
	+ Fix bug involving renaming and deleting projects that were open in multiple tabs ([#81](https://github.com/olab-io/ofSketch/issues/81)).
	+ Fix bug that allowed ACE Editor to be resized vertically.
	+ Improved front-end validation. Better alerts and checks if Project or Class name already exists ([#11](https://github.com/olab-io/ofSketch/issues/11)).
	+ Error lines now reported correctly ([#77](https://github.com/olab-io/ofSketch/issues/77)).
	+ Stop button now closes running app. ([#45](https://github.com/olab-io/ofSketch/issues/45))
	+ Compilation errors and warnings now appear in-line in the editor ([#21](https://github.com/olab-io/ofSketch/issues/21)).
	+ Fix stop button bug ([#45](https://github.com/olab-io/ofSketch/issues/45)).
	+ Addon support ([#20](https://github.com/olab-io/ofSketch/issues/20))!
	
- 0.2.2 (07-22-14)
	+ Add "About ofSketch" to settings ([#64](https://github.com/olab-io/ofSketch/issues/64)).
	+ Basic localization support for Spanish, German, and Japanese ([#68](https://github.com/olab-io/ofSketch/issues/68)).
	+ Fix bug that prevented new class tabs from displaying when a project was first created ([#76](https://github.com/olab-io/ofSketch/issues/76)).
	+ Updated NewProject template ([#67](https://github.com/olab-io/ofSketch/issues/67)).
	

- 0.2.1 (07-19-14)
	+ Editor fills browser window.
	+ File tabs are responsive.
	+ Title of each webpage is now the name of the open project.

- 0.2.0 (07-15-14)
	+ Now uses openFrameworks 0.8.3.
	+ Fully responsive toolbar.
	+ Client is notified when server connection is lost.
	+ Undo/Redo keyboard functionality.
	+ Front-end input validation.
	+ Cross-platform keyboard shortcuts.
	+ Add Keyboard Shortcuts Table.
	+ Add default ACE Settings Menu.
	+ Autocomplete now triggers when typing.
	+ Basic syntax highlighting for oF global functions.
	+ Unsaved document names appear red.
	+ Fixed ctrl-F, cmd-F (find) bar styling.

- 0.1.6
    + Now uses openFrameworks 0.8.2.
    + Add ARCHITECTURE.md document.
    + Updated README.md.
    + Updates version naming scheme.
    + Update ofSketch OSX App plist.
    + Various bugfixes and source cleanup. 

- 0.1.5 (06-16-2014)
    + Server now sends available addons to client.
    + Updated default number of processors to match target machine cores on a make build.
    + Rewrote build scripts.  Now works on all osx / linux.

- 0.1.4 (06-13-2014)
    + Added linux64 build.
    + Updated build script for linux64.
    + Sketches no longer initialize variables in header.  Init in setup() or ctor.

- 0.1.3 (06-13-2014)
    + Regression requires reengineering of WebSocketLoggerChannel.  Removed for now.
    + Fixed auto release download link.  Now links to binary release if available.
    + Updated build script.

- 0.1.2 (06-13-2014)
    + Fix hang on quit https://github.com/olab-io/ofSketch/issues/52.
    
- 0.1.1 (06-09-2014)
    + Initial release.

- 0.1.0 (06-09-2014)
    + Initial source release.

- 0.0.1 (05-19-2014)
    + Initial proof of concept.
