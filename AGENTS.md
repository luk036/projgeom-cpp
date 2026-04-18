# AGENTS.md - projgeom-cpp

## Project Overview

**projgeom-cpp** is a C++20 header-only library for projective geometry with constexpr support, branchless computation, and C++20 concepts. It includes a static library component for logger implementation.

---

## Build Commands

### Build the project
```bash
cmake -S. -B build
cmake --build build
```

### Run all tests
```bash
cd build/test
CTEST_OUTPUT_ON_FAILURE=1 ctest

# or run the executable directly:
./build/test/ProjGeomTests
```

### Run a single test
The test executable uses doctest. Use `--test-case="TestName"` or `-tc="TestName"` to run a specific test:
```bash
./build/test/ProjGeomTests --test-case="Perspective"
./build/test/ProjGeomTests -tc="Perspective"
```

### Run tests with regex filter
```bash
./build/test/ProjGeomTests -ts="Persp"   # Run test cases matching "Persp"
```

### Code coverage
```bash
cmake -S. -B build -DENABLE_TEST_COVERAGE=1
cmake --build build
cd build/test && ctest
```

### Format check/fix
```bash
cmake -S . -B build/test

# View formatting changes
cmake --build build --target format

# Apply formatting fixes
cmake --build build --target fix-format
```

Requires: `clang-format`, `cmake-format`, `pyyaml`

### Build documentation
```bash
cmake -S . -B build
cmake --build build --target GenerateDocs
# View: open build/documentation/doxygen/html/index.html
```

Requires: `doxygen`, `jinja2`, `pygments`

### Additional CMake options
- Sanitizers: `-DUSE_SANITIZER=<Address|Memory|Undefined|Thread|Leak>'`
- Static analyzers: `-DUSE_STATIC_ANALYZER=<clang-tidy|iwyu|cppcheck>`
- Ccache: `-DUSE_CCACHE=ON`

---

## Code Style Guidelines

### Formatting (`.clang-format`)
- **Style**: Google-based
- **Column limit**: 100
- **Indent width**: 4
- **Brace style**: Attach
- **Namespace indentation**: All
- **Include grouping**: Regroup (separates system/standard headers from project headers)

### Naming Conventions
| Element | Convention | Example |
|---------|------------|---------|
| Types (struct, class) | PascalCase | `pg_point`, `pg_line` |
| Functions | snake_case | `join()`, `check_axiom()` |
| Member variables | `_underscore` prefix | `_Base`, `_K` |
| Template parameters | `_underscore` prefix | `_K`, `_Value` |
| Constants | PascalCase | `L_INF` |
| Namespaces | lowercase | `fun` |

### Code Organization

**Headers** (`include/projgeom/`):
- Use `#pragma once`
- Include order (regrouped by clang-format):
  1. Associated header
  2. Project headers
  3. Standard library
  4. External libraries
- Example:
  ```cpp
  #include "pg_common.hpp"
  #include "pg_object.hpp"
  
  #include <array>
  #include <cassert>
  
  #include "common_concepts.h"  // external
  ```

**Namespace**: All library code lives in `fun` namespace.

### C++ Standards & Features
- **C++ standard**: C++20
- **Concepts**: Use `std::equality_comparable`, `std::convertible_to` from `<concepts>`
- **Constexpr**: Prefer `constexpr` for all computable functions
- **No raw loops**: Use algorithms or range-based for-loops where appropriate

### Error Handling
- Use `<cassert>` for invariants and precondition checks
- Concepts used for compile-time validation
- No exceptions (project uses error codes or assertions)

### Testing Patterns
- **Framework**: doctest
- **Property-based**: RapidCheck (define `RAPIDCHECK_H` to enable)
- Test file naming: `test_*.cpp`
- Test cases use `TEST_CASE("Name")` and assertions like `CHECK()`, `REQUIRE()`

```cpp
#include <doctest/doctest.h>
#include <projgeom/pg_plane.hpp>

TEST_CASE("Perspective") {
    auto pt_p = PerspPoint({3, 4, 5});
    CHECK(fun::check_axiom(pt_p, pt_q, ln_m));
}
```

### Dependencies (via CPM.cmake)
- **fmt**: Formatting library
- **doctest**: Testing framework
- **rapidcheck**: Property-based testing
- **PackageProject.cmake**: Installation

---

## Key Files

| Path | Description |
|------|-------------|
| `CMakeLists.txt` | Root build configuration |
| `test/CMakeLists.txt` | Test configuration |
| `include/projgeom/` | Header-only library |
| `source/logger.cpp` | Logger implementation |
| `.clang-format` | Code formatting rules |
| `cmake/tools.cmake` | Build tools configuration |

---

## Common Development Workflow

1. Make changes to headers in `include/projgeom/`
2. Run format: `cmake --build build --target fix-format`
3. Build: `cmake --build build`
4. Test: `./build/test/ProjGeomTests`
5. Run specific test case during development