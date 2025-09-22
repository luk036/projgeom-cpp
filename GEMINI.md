# Project Overview

This is a C++ project that provides a library for projective geometry. It is a header-only library that uses modern C++ features like C++20 concepts. The project uses CMake for building and managing dependencies. It also includes a test suite and documentation.

## Building and Running

### CMake

To build and run the project using CMake, you can use the following commands:

```bash
# Build the project
cmake -S . -B build
cmake --build build

# Run the standalone application
./build/standalone/ProjGeom --help

# Run the tests
cd build/test
CTEST_OUTPUT_ON_FAILURE=1 ctest
```

### xmake

To build and run the project using xmake, you can use the following commands:

```bash
# Build the project
xmake

# Run the tests
xmake run test_projgeom
```

## Development Conventions

*   **Coding Style:** The project uses `clang-format` to enforce a consistent coding style. You can use the `format` and `fix-format` targets in the CMake build to check and fix the code style.
*   **Testing:** The project uses `doctest` for testing. The tests are located in the `test` directory.
*   **Dependencies:** The project uses `CPM.cmake` to manage dependencies. The dependencies are defined in the `CMakeLists.txt` and `specific.cmake` files.

## Source Code

The source code is located in the `include/projgeom` directory. The library is header-only, so there are no source files to compile. The `standalone` directory contains a simple command-line application that demonstrates how to use the library. The `test` directory contains the test suite for the library.

The core logic of the library is located in the `include/projgeom/pg_plane.hpp` file. This file defines several functions that implement the axioms and theorems of projective geometry, such as Pappus's theorem and Desargues's theorem. The code is written in a generic way, using templates and C++20 concepts to work with different types of points and lines.

The `include/projgeom/pg_point.hpp` file defines the `pg_point` class, which represents a point in the projective plane. It inherits from the `pg_object` class and provides constructors for creating points from coordinates. It also defines a `join` function for creating a line from two points.

The `include/projgeom/pg_line.hpp` file defines the `pg_line` class, which represents a line in the projective plane. It inherits from the `pg_object` class and provides constructors for creating lines from coordinates. It also defines a `meet` function for creating a point from two lines.
