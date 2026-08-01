# Try system-installed fmt first (Ubuntu: libfmt-dev, macOS: brew install fmt, Termux: fmt)
find_package(fmt CONFIG QUIET)

if(fmt_FOUND)
  message(STATUS "Found system fmt: ${fmt_DIR}")
  # Tell CPM that fmt is already handled (CPM checks CPM_PACKAGES list). Write the CACHE variable
  # directly: list(APPEND ...) creates a normal-variable shadow that does not propagate into
  # FetchContent subdirectory scopes.
  if(NOT fmt IN_LIST CPM_PACKAGES)
    set(CPM_PACKAGES
        "${CPM_PACKAGES};fmt"
        CACHE INTERNAL "" FORCE
    )
  endif()
else()
  CPMAddPackage(
    NAME fmt
    GIT_TAG 12.1.0
    GITHUB_REPOSITORY fmtlib/fmt
    OPTIONS "FMT_INSTALL YES" # create an installable target
  )
endif()

# Try system-installed spdlog first (Ubuntu: libspdlog-dev, macOS: brew install spdlog, Termux:
# spdlog)
find_package(spdlog CONFIG QUIET)

if(spdlog_FOUND)
  message(STATUS "Found system spdlog: ${spdlog_DIR}")
  # Tell CPM that spdlog is already handled (write CACHE directly, see fmt above)
  if(NOT spdlog IN_LIST CPM_PACKAGES)
    set(CPM_PACKAGES
        "${CPM_PACKAGES};spdlog"
        CACHE INTERNAL "" FORCE
    )
  endif()
else()
  # Add spdlog for logging functionality
  CPMAddPackage(
    NAME spdlog
    GIT_TAG v1.17.0
    GITHUB_REPOSITORY gabime/spdlog
    OPTIONS "SPDLOG_INSTALL YES" "SPDLOG_FMT_EXTERNAL YES" # use external fmt to avoid duplicate
            # symbols
  )
endif()

CPMAddPackage(
  NAME Fractions
  GIT_TAG v1.1.3
  GITHUB_REPOSITORY luk036/fractions-cpp
  OPTIONS "FMT_INSTALL YES"
)

# CPMAddPackage("gh:ericniebler/range-v3#0.12.0")

set(SPECIFIC_LIBS fmt::fmt spdlog::spdlog Fractions::Fractions)
