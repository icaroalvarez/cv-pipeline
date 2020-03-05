# Panoramix - Test playground for your image processors.

Library tool for testing computer vision image processors. 

## Installing external dependencies

### Dependencies managed by conan
- Catch2
- Trompeloeil
- OpenCV

Run the following script for installing the dependencies:

```bash
sh conan_install.sh
```

## Building the project

### Using CMake

To compile any target in the project, you can directly use CMake. Create a build directory manually and generate the 
Makefiles in it. 

```bash
mkdir build && cd build
cmake ..
make
```

## Technical Features
- Modern C++11/14/17.
- Testing witch Catch2 and mocking with Trompeloeil.
- Testing coverage using lcov. 
- Acceptance Test Driven Development (ATDD) and Behavior-Driven Development (BDD).
- Build system with Modern CMake.
- Conan as dependency manager.
- Design patterns: factory, observer, 
- Multi-threading synchronization with condition variables.
- Template specialization
- std::variant with std::visit
