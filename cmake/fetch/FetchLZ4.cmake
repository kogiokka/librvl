FetchContent_Declare(
    lz4
    URL "${CMAKE_CURRENT_SOURCE_DIR}/external/lz4-1.9.4.zip"
    URL_HASH MD5=9c6b76f71921dd986468dcde7c095793
    SOURCE_SUBDIR "build/cmake/" # Look for the CMakeLists.txt of lz4
)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET FALSE)

set(LZ4_BUILD_CLI OFF CACHE BOOL "Build lz4 program")
set(LZ4_BUILD_LEGACY_LZ4C OFF CACHE BOOL "Build lz4c program with legacy argument support")
FetchContent_MakeAvailable(lz4)
