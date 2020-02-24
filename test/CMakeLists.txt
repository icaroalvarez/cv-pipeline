cmake_minimum_required(VERSION 3.5)

set(test_source_files
        main.test.cpp
        Acceptance.test.cpp
        Factory.test.cpp
        ProcessorConfiguration.Test.cpp
        Runnable.Test.cpp
        ImageFrameSource.Test.cpp)

add_executable(test_panoramix
        ${test_source_files}
        )

target_link_libraries(test_panoramix PRIVATE
        panoramix
        CONAN_PKG::trompeloeil
        CONAN_PKG::Catch2
        )

target_include_directories(test_panoramix PRIVATE
        ../src)

target_compile_definitions(test_panoramix
        PRIVATE fixtures_path="${CMAKE_CURRENT_SOURCE_DIR}/fixtures/"
        )