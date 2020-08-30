cmake_minimum_required(VERSION 3.5)

set(source_files
        PipelineController
        ImageProcessor
        Factory
        Observer
        Notifier
        ProcessorParameters
        Runnable
        ImageFrameSource
        FrameSource
        VideoFrameSource
        FrameSourceFactory
        PipelineConfiguration
        JsonStorage)

add_library(cv-pipeline
        ${source_files}
        )

target_link_libraries(cv-pipeline
        PRIVATE
            pthread
        PUBLIC
            CONAN_PKG::opencv
            CONAN_PKG::jsonformoderncpp
            CONAN_PKG::easyloggingpp
            stdc++fs
        )

target_include_directories(cv-pipeline PUBLIC
        ../src
)
