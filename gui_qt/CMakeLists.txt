cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set Policy CMP0071 to new for letting AUTOMOC and AUTOUIC process GENERATE files.
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtWidgets library
set(CMAKE_PREFIX_PATH /Users/icaro/Qt/5.10.0/clang_64/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets)

set(source_files
        main.cpp
        mainwindow
        qlabelmat
        qwidgethandler
        ProcessorWidget
        Utils
        FrameSourceNavigationWidget)

set(FORMS
        mainwindow.ui
        )
qt5_wrap_ui(FORM_H ${FORMS})

# Add the include directories for the Qt 5 Widgets module to
# the compile lines.
#include_directories(${Qt5Widgets_INCLUDE_DIRS})

# Use the compile definitions defined in the Qt 5 Widgets module
#add_definitions(${Qt5Widgets_DEFINITIONS})

# Add compiler flags for building executables (-fPIE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#qt5_generate_moc(main.cpp main.moc)

#add_library(playground_gui ${tst_hdr_moc} ${tst_form_hdr})
#qt5_use_modules(mainwindow Widgets)

# Tell CMake to create the helloworld executable
add_executable(gui
        ${source_files}
        ${FORM_H}
        ${LOGGING_SOURCES_FILES})

#qt5_use_modules(playground_gui Core Gui Widgets)

find_package( OpenCV REQUIRED )

#Link the helloworld executable to the Qt 5 widgets library.
target_link_libraries(gui panoramix image_processors Qt5::Widgets ${OpenCV_LIBS} easylogging)

add_subdirectory(3rd-party)

# Specify include directories or targets to use when compiling a given target
target_include_directories(gui PRIVATE
        ../src
        ../image_processors
        #easylogging
        )

#Make a top-level <target> depend on other top-level targets to ensure that they build before <target> does
add_dependencies(gui
        panoramix)