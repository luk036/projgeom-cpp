# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
# option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# ---- Dependencies ----

CPMAddPackage("gh:doctest/doctest#v2.4.11")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.3")

# if(TEST_INSTALLED_VERSION) find_package(${PROJECT_NAME} REQUIRED) else() CPMAddPackage(NAME
# ${PROJECT_NAME} SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..) endif()

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
add_executable(${PROJECT_NAME}Tests ${sources})
target_link_libraries(
  ${PROJECT_NAME}Tests doctest::doctest ${PROJECT_NAME}::${PROJECT_NAME} ${SPECIFIC_LIBS}
)
set_target_properties(${PROJECT_NAME}Tests PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wpedantic -Wextra -Werror)
elseif(MSVC)
  target_compile_options(${PROJECT_NAME} INTERFACE /W4 /WX /wd4996)
  target_compile_definitions(${PROJECT_NAME}Tests INTERFACE DOCTEST_CONFIG_USE_STD_HEADERS)
else()
  target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wpedantic -Wextra -Werror)
endif()

# ---- Add ${PROJECT_NAME}Tests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME projgeomTests COMMAND
# ${PROJECT_NAME}Tests)

string(TOLOWER ${PROJECT_NAME} PROJECT_FILE_NAME)
add_test(NAME ${PROJECT_FILE_NAME}Tests COMMAND ${PROJECT_NAME}Tests)

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(${PROJECT_NAME} INTERFACE -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(${PROJECT_NAME} INTERFACE -fprofile-arcs -ftest-coverage)
endif()
