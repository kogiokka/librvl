FetchContent_Declare(
    liblzma
    URL "${CMAKE_CURRENT_SOURCE_DIR}/external/liblzma-5.2.7.zip"
    URL_HASH MD5=98b6ac8cc863bab1ae104bdfdda736fe
)

if(NOT liblzma_POPULATED)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
    set(FETCHCONTENT_QUIET FALSE)

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build liblzma as a shared library instead of static")
    FetchContent_MakeAvailable(liblzma)
    set_target_properties(liblzma PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
