# Panoramix - Visual playground for your image processors.

Tool for graphically testing computer vision image processors.

## Installing external dependencies

### Dependencies managed by conan

```bash
mkdir build && cd build
conan install ..
```

### Other dependencies
- OpenCV 3 and Qt5 must been manually installed in the system

## Building the project

### Using CMake

To compile any target in the project, you can directly use CMake. Create a build directory manually and generate the 
Makefiles in it. 

```bash
mkdir build && cd build
cmake ..
make
```