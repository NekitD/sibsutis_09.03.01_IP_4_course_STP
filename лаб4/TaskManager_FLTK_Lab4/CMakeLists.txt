cmake_minimum_required(VERSION 3.15)

project(TaskManager_FLTK LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# FLTK is downloaded automatically during the first CMake configure.
# We use the current stable FLTK 1.4.5 release.
FetchContent_Declare(
    fltk
    URL https://github.com/fltk/fltk/releases/download/release-1.4.5/fltk-1.4.5-source.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

set(FLTK_BUILD_TEST OFF CACHE BOOL "" FORCE)
set(FLTK_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FLTK_BUILD_FLUID OFF CACHE BOOL "" FORCE)
set(FLTK_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(fltk)

add_executable(TaskManager_FLTK
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/task.cpp
    src/task.h
    src/project.cpp
    src/project.h
    src/repository.h
    src/taskmanager.cpp
    src/taskmanager.h
)

target_include_directories(TaskManager_FLTK PRIVATE src)
target_link_libraries(TaskManager_FLTK PRIVATE fltk::fltk)

if (WIN32)
    set_target_properties(TaskManager_FLTK PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
