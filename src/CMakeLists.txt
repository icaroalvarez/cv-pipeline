cmake_minimum_required(VERSION 3.5)

set(source_files
        PipelineController
        ImageProcessor
        Factory
        Observer
        Notifier
        ProcessorParameters
        Runnable
        ImageFrameSource)

add_library(panoramix
        ${source_files}
        )

target_link_libraries(panoramix
        PRIVATE pthread
        PUBLIC CONAN_PKG::opencv
        )

target_include_directories(panoramix PUBLIC
        ../src
)
