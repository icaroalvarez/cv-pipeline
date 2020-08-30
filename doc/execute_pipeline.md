# Execute pipeline processing
See the complete executable example in test/Acceptance.test.cpp.

In the following, the main steps are explained.

## 1) Create a pipeline and register the image processors.
Processor must be registered in the pipeline so they can be loaded.
```cpp
PipelineController controller;
controller.registerImageProcessor<Processor1>(processor1Name);
controller.registerImageProcessor<Processor2>(processor2Name);
```

## 2) Load pipeline configuration
Frame source path and processors to be loaded are defined in the configuration.
```cpp
controller.loadPipeline(pipelineConfiguration);
```

## 3) Register an observer into the pipeline
An observer is a class that inherits from Observer, so a update method must be implemented which will be called when the pipeline finish processing.

```cpp
controller.registerObserver(observer);
```

## 4) Fire pipeline processing
Will call observer when finishes
```cpp
controller.firePipelineProcessing();
```

## 5) Check result images
After the pipeline finishes, each image processor debug image can be retrieved.
```cpp
auto debugImageProcessor1{controller.getDebugImageFrom(processor1Index)};
auto debugImageProcessor2{controller.getDebugImageFrom(processor2Index)};
```

## 6) Configure processors
Processor parameters can be configured anytime.
```cpp
configureProcessor(processorIndex, configuration);
```