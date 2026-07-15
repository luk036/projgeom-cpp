CPMAddPackage(
  NAME fmt
  GIT_TAG 12.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

# Add spdlog for logging functionality
CPMAddPackage(
  NAME spdlog
  GIT_TAG v1.17.0
  GITHUB_REPOSITORY gabime/spdlog
  OPTIONS "SPDLOG_INSTALL YES" "SPDLOG_FMT_EXTERNAL YES" # use external fmt to avoid duplicate
                                                         # symbols
)

CPMAddPackage(
  NAME Fractions
  GIT_TAG v1.1.3
  GITHUB_REPOSITORY luk036/fractions-cpp
  OPTIONS "FMT_INSTALL YES"
)

# CPMAddPackage("gh:ericniebler/range-v3#0.12.0")

set(SPECIFIC_LIBS fmt::fmt spdlog::spdlog Fractions::Fractions)
