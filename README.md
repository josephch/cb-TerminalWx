# cbTerminal plugin for Code::Blocks (Linux)
cbTerminal provides bash shell in the bottom pane of Code::Blocks

## Building the plugin from source
After clone, please fetch submodules
```
git submodule update --init
```
Also codeblocks needs to be installed.
Please run following commands to build with cmake
```
cd cbTerminal
mkdir build
cd build
cmake ../
make
```
Alternatively, you can open the project in codeblocks and build it from there.

## Acknowledgements
Most of the C::B specific is taken from ThreadedSearch plugin and modified. Thank you authors of threaded search plugin.
