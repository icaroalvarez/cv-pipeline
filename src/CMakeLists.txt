cmake_minimum_required(VERSION 3.5)

set(source_files
        PipelineController
        ImageProcessor
        Factory
        Observer
        Notifier
        ProcessorConfiguration
        Runnable)

add_library(panoramix
        ${source_files}
        )

target_link_libraries(panoramix PUBLIC
        CONAN_PKG::jsonformoderncpp
        pthread
        opencv_core
        opencv_highgui)
