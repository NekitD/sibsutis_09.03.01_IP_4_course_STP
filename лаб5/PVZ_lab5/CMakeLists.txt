cmake_minimum_required(VERSION 3.16)
project(pickup_point CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(FLTK REQUIRED)
find_package(PostgreSQL REQUIRED)

find_path(LIBPQXX_INCLUDE pqxx/pqxx)
find_library(LIBPQXX_LIB NAMES pqxx libpqxx)

add_executable(pickup_point
    src/main.cpp
    src/db.cpp
    src/main_window.cpp
    src/dialogs.cpp
)

target_include_directories(pickup_point PRIVATE src ${LIBPQXX_INCLUDE})
target_link_libraries(pickup_point PRIVATE
    ${FLTK_LIBRARIES}
    ${LIBPQXX_LIB}
    PostgreSQL::PostgreSQL
)