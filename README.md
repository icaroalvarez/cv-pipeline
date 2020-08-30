# CV-Pipeline - Computer Vision Pipeline.

Process images through a pipeline of image processors. Configure and tune the processors and see the result in real time. 

## Installing external dependencies

### Dependencies managed by conan
- [Catch2](https://github.com/catchorg/Catch2)
- [Trompeloeil](https://github.com/rollbear/trompeloeil)
- [OpenCV](https://github.com/opencv/opencv)
- [JSON for Modern C++](https://github.com/nlohmann/json) 
- [Easylogging++](https://github.com/amrayn/easyloggingpp)

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

## Using image processor pipeline
- [Create your own custom image processors](doc/create_custom_image_processor.md) or use [the ones already created](https://github.com/icaroalvarez/image-processors).
- [Execute pipeline processing](doc/execute_pipeline.md)

## Technical Features
- Modern C++17 (std::variant, std::visit).
- Testing and mocking with Catch2 and Trompeloeil frameworks.
- Testing coverage using lcov. 
- Acceptance Test Driven Development (ATDD) and Behavior-Driven Development (BDD).
- Build system with Modern CMake.
- Conan as dependency manager.
- Multi-threading synchronization with condition variables.
- Template specialization