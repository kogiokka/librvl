FetchContent_Declare(
    lz4
    URL "${CMAKE_CURRENT_SOURCE_DIR}/external/lz4-1.9.4.zip"
    URL_HASH MD5=9c6b76f71921dd986468dcde7c095793
)

if(NOT lz4_POPULATED)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
    set(FETCHCONTENT_QUIET FALSE)
    FetchContent_MakeAvailable(lz4)
    add_subdirectory("${lz4_SOURCE_DIR}/build/cmake/" "lz4")
endif()
