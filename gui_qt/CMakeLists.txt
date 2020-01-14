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

find_package(Qt5Widgets)

set(source_files
        main.cpp
        mainwindow
        qlabelmat
        qwidgethandler
        ProcessorWidget
        FrameSourceNavigationWidget
        )

set(FORMS
        mainwindow.ui
        )
qt5_wrap_ui(FORM_H ${FORMS})

# Tell CMake to create the helloworld executable
add_executable(gui
        ${source_files}
        ${FORM_H}
        ${LOGGING_SOURCES_FILES})

find_package( OpenCV REQUIRED )

#Link the helloworld executable to the Qt 5 widgets library.
target_link_libraries(gui panoramix image_processors Qt5::Widgets ${OpenCV_LIBS} easylogging)

add_subdirectory(3rd-party)

# Specify include directories or targets to use when compiling a given target
target_include_directories(gui PRIVATE
        ../src
        ../image_processors
        )

#Make a top-level <target> depend on other top-level targets to ensure that they build before <target> does
add_dependencies(gui
        panoramix)