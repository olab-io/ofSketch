# Contributing to ofSketch

First off, thanks for your interest contributing! ofSketch is still in its early stages of development, so there is plenty to do (and plenty that is undoubtedly broken, doesn't work, etc...). Here is a brief set of guidelines and pointers for contributing to ofSketch. For more information contact [Brannon Dorsey](http://brannondorsey.com) <<brannon@brannondorsey.com>> or [Christopher Baker](http://christopherbaker.net) <<me@christopherbaker.net>>.

If you plan on contributing heavily to ofSketch, check out our [ARCHITECTURE.md](ARCHITECTURE.md) file to learn more about the ofxAddons and Libs that ofSketch uses, as well as an overview of the way that ofSketch works.

Also, check out openFrameworks' [CONTRIBUTING.md](https://github.com/openframeworks/openFrameworks/blob/master/CONTRIBUTING.md) for more info about contributing to ofSketch, openFrameworks, or open source projects in general.

## Report a Bug

- If possible, bug reports should be submitted by clicking the "Report a Bug" button __inside__ ofSketch. This is helpful because info about your user-agent and machine will be automatically included inside of the bug report.
- Before you submit a bug, check the [issues list](https://github.com/olab-io/ofSketch/issues?labels=&milestone=&page=1&state=open) to make sure that someone else hasn't already experienced the same problem. Don't forget to look at [closed issues](https://github.com/olab-io/ofSketch/issues?labels=&milestone=&page=1&state=closed).
- Only submit bug reports on current releases of ofSketch.

## Request a Feature

Feature requests can be proposed in the form of [new issues](https://github.com/olab-io/ofSketch/issues/new) submitted with the "feature request" tag. Before proposing a feature request, please assure that a similar request hasn't already been made. If it is, we would love to hear your voice in the discussion on that similar issue. Please create a feature request __before contributing code or submitting a Pull Request__.

## Contributing Code

We are more likely to accept your code if we feel like it has been discussed already. 
If you are submitting a new feature, it's best if the feature has been discussed beforehand, either as a [feature request](#feature-requests) or on the [forums](http://openframeworks.cc/forums) or the [of-dev mailing list](http://dev.openframeworks.cc/listinfo.cgi/of-dev-openframeworks.cc).

- Submit your Pull Request to the `develop` branch of ofSketch, not the master branch.
- If you are able to do so, test your code on different platforms before submitting it, but at least test it on your platform to make sure it doesn't break anything.
- Please write descriptive commit messages for each of the commits that you make. They don't have to be in-depth, just a brief summary of what the commit contains. A page describing how well-written commit messages look like can be found here.
- ofSketch uses [Poco style guide](http://www.appinf.com/download/CppCodingStyleGuide.pdf) (for the most part). Please try and remain consistent with our code style when writing code for ofSketch.
