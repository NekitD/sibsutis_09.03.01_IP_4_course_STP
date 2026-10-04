--
Сборка:
mkdir build
cd build
cmake ..
cmake --build . --config Release
--
Запуск (Windows):
cd build\Releases
$env:PICKUP_DB = "host=localhost dbname=pickup user=pickup_user password=pickup_pass"
.\pickup_point.exe
--
Если вы сидите на Windows для сборки придётся помучаться с настройками и возможно даже изменить CMake файл. CMake, который был у меня:

cmake_minimum_required(VERSION 3.16)
project(pickup_point CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "C:/FLTK")

find_package(FLTK CONFIG REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

add_executable(pickup_point
    src/main.cpp
    src/db.cpp
    src/main_window.cpp
    src/dialogs.cpp
)

target_include_directories(pickup_point PRIVATE src)

target_link_libraries(pickup_point PRIVATE
    fltk::fltk
    libpqxx::pqxx
    PostgreSQL::PostgreSQL
)
