# fsc (Filesystem Commands)

fsc is a cross-platform CLI utility for quick and efficient file management, eliminating the need to remember platform-specific commands.

## Building

fsc uses CMake as a build system. Currently the CMakeLists.txt has configurations for Clang and GCC.

### Prerequisites
- CMake >= 4.0.0
- GCC >= 10 or
- Clang >= 12 or
- Any other C++ 20 capable compiler

### steps

```
# Clone the repo
git clone https://christopherstierna/fsc.git

cd fsc
mkdir build
cd build

cmake ..
# or
cmake -B . -S .. -G <GENERATOR OF CHOICE> -D CMAKE_CXX_COMPILER=<COMPILER OF CHOICE> -D CMAKE_BUILD_TYPE=<RELEASE OR DEBUG>

cmake --build .
```

Now add fsc.exe to your system's PATH to use it.

## Usage

Commands are structured as the following:
```
fsc <command> <parameters> <flags>
```
To see a list of all commands use
```
fsc help
```
For help with a specific command use
```
fsc help <command>
```
### examples
```
# creates a folder
fsc create someFolder

# creates a file.
# The -f flag specifies a file,
# since the file created does not have a file extension
fsc create file -f

# creates a .hpp file
fsc create file.hpp

# outputs the file contents to the console
fsc read someText.txt

# lists the current directory
fsc list

# lists dir2 recursively
fsc list dir1/dir2 -r

# renames foo.txt to bar.txt
fsc rename foo.txt bar.txt

# moves secrets.txt into mySecretStuff which is the parent of the current path
fsc move secrets.txt ../mySecretStuff

# clones yippie.txt to dir1
fsc clone yippie.txt dir1
```
These are not all of the commands, to see a full list use "fsc help".

## License

MIT License

Copyright (c) 2025 Christopher Stierna (https://github.com/christopherstierna)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

1. The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

2. This license applies to the entire repository, including all past, present, and
   future commits and versions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.