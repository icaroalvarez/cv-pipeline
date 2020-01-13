cmake_minimum_required(VERSION 3.5)

set(source_files
        PipelineController
        ImageProcessor
        Factory
        Observer
        Notifier
        ProcessorConfiguration
        )

add_library(panoramix
        ${source_files}
        )

target_link_libraries(panoramix PRIVATE
        pthread
        opencv_core
        opencv_highgui)
