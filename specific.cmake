CPMAddPackage(
  NAME fmt
  GIT_TAG 10.2.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

# Add spdlog for logging functionality
CPMAddPackage(
  NAME spdlog
  GIT_TAG v1.12.0
  GITHUB_REPOSITORY gabime/spdlog
  OPTIONS "SPDLOG_INSTALL YES" # create an installable target
)

# CPMAddPackage("gh:ericniebler/range-v3#0.12.0")

set(SPECIFIC_LIBS fmt::fmt spdlog::spdlog)
