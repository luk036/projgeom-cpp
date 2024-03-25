CPMAddPackage(
  NAME fmt
  GIT_TAG 9.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

CPMAddPackage("gh:ericniebler/range-v3#0.12.0")

set(SPECIFIC_LIBS fmt::fmt range-v3)
