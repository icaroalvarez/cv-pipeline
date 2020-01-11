cmake_minimum_required(VERSION 3.5)

set(source_files
        PipelineController
        ImageProcessor
        Factory)

add_library(panoramix
        ${source_files}
        )