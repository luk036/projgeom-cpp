cmake_minimum_required(VERSION 3.14)

project(ProjGeomInstalledTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies first - ProjGeomConfig.cmake references fmt, spdlog, and Fractions (installed
# alongside the library by `cmake --install`).
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(Fractions REQUIRED)
find_package(ProjGeom REQUIRED)

add_executable(projgeom_installed_test main.cpp)
target_link_libraries(projgeom_installed_test PRIVATE ProjGeom::ProjGeom)

enable_testing()
add_test(NAME projgeom_installed_test COMMAND projgeom_installed_test)
