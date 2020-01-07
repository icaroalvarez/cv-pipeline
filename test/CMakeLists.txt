cmake_minimum_required(VERSION 3.5)

set(test_source_files
        Acceptance.test.cpp
        main.test.cpp)

add_executable(test_panoramix
        ${test_source_files}
        )

target_compile_definitions(test_panoramix
        PRIVATE fixtures_path="${CMAKE_CURRENT_SOURCE_DIR}/fixtures"
        )

set (CMAKE_CXX_FLAGS -DELPP_THREAD_SAFE)