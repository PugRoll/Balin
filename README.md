# Balin
Balin aims to be a simpler build system for smaller C/C++ projects. Similar to CMake it uses a build file specify build tasks.



### Installation
To install Balin clone this repo:
```
git clone https://github.com/PugRoll/balin.git
```
- Have Balin build itself
```
./balin balin.bx
```
- This will install Balin to your PATH variable


### Usage
To use Balin with your C/C++ project create a `.bx` file. This is the file Balin
looks for when building.
```balin
/*myproject.bx*/
executable "a.out"
cppc "g++"
cc "gcc"
version "1.0.0"
include "./include/*"
flags "I o"
```

