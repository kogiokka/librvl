include(FetchContent)

set(lz4_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/lz4")

FetchContent_Declare(
    lz4
    URL "https://github.com/lz4/lz4/archive/refs/tags/v1.9.4.zip"
    URL_HASH MD5=9c6b76f71921dd986468dcde7c095793
    SOURCE_DIR "${lz4_SOURCE_DIR}"
    TLS_VERIFY TRUE
)

set(FETCHCONTENT_QUIET FALSE)
FetchContent_MakeAvailable(lz4)

add_subdirectory("${lz4_SOURCE_DIR}/build/cmake/")
