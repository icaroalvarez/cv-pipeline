cmake_minimum_required(VERSION 3.5)

set(test_source_files
        main.test.cpp
        Acceptance.test.cpp
        Factory.test.cpp
        ProcessorParameters.Test.cpp
        Runnable.Test.cpp
        ImageFrameSource.Test.cpp
        VideoFrameSource.Test.cpp
        FrameSourceFactory.Test.cpp
        JsonStorage.Test.cpp
        PipelineController.Test.cpp
        ImageProcessors/Mosaic.Test.cpp
        )

add_executable(test-cv-pipeline
        ${test_source_files}
        )

target_link_libraries(test-cv-pipeline PRIVATE
        cv-pipeline
        CONAN_PKG::trompeloeil
        CONAN_PKG::Catch2
        )

target_include_directories(test-cv-pipeline PRIVATE
        ../src)

target_compile_definitions(test-cv-pipeline
        PRIVATE fixtures_path="${CMAKE_CURRENT_SOURCE_DIR}/fixtures/"
        )

option(COVERAGE_ENABLED "Calculate test coverage" OFF)

if(COVERAGE_ENABLED MATCHES ON)
    message("Testing coverage enabled")
    include(CMakeModules/CodeCoverage.cmake)
    append_coverage_compiler_flags()
    target_compile_options(cv-pipeline PRIVATE -g -fprofile-arcs -ftest-coverage)
    setup_target_for_coverage_lcov(
            NAME test-cv-pipeline-coverage
            EXECUTABLE test-cv-pipeline
            DEPENDENCIES cv-pipeline
            BASE_DIRECTORY "${PROJECT_SOURCE_DIR}/src"
            EXCLUDE "${PROJECT_SOURCE_DIR}/test/*" "/usr/*" "$ENV{HOME}/.conan/*")
endif()