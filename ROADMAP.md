#ofSketch Roadmap

This document gives an overview of some of the planned features and project goals as we approach a v1.0 release. For a more up-to-date look at the progress of these features, and to add to their discussion, check out our issue tracker's [milestone page](https://github.com/olab-io/ofSketch/milestones).

## Windows Support

We hope to have full Windows support by September, 2014. The necessary Windows toolchain to build projects using ofSketch will be distributed with the Windows download. More info on Windows compatibility progress [here](https://github.com/olab-io/ofSketch/labels/windows).

## Project Export

Project export feature that uses a version of the openFrameworks Project Generator in the ofSketch app to export an ofSketch project for the following platforms:

- Makefiles
- Xcode
- Xcode 32-bit
- Visual Studio 32-bit
- Code::Blocks 32-bit Windows
- Code::Blocks 32-bit Linux
- Code::Blocks 64-bit Linux
- Code::Blocks arm6l
- Code::Blocks arm7l

## Custom .h & .cpp Files

To aid in the transition from ofSketch to more advanced IDEs, we intend to give the option to use ofSketch to create and work with regular `.h` and `.cpp` files. Eventually, using this functionality will be as simple as including the appropriate extension when creating the file.

The Processing IDE allows similar functionality when creating and using `.java` files.

## Clang Indexed Autocomplete

We intend to use some of the Clang resources mentioned [here](https://github.com/brannondorsey/ofSketch/wiki/Clang-Resources) to index the openFrameworks Core and use it to better provide autocomplete and syntax highlighting tools in the ACE editor. Ideally, this system would also auto-index user code.