FetchContent_Declare(
    logc
    URL "${CMAKE_CURRENT_SOURCE_DIR}/external/log.c-f9ea34994bd58ed342d2245cd4110bb5c6790153.zip"
    URL_HASH MD5=a2a0187ff04c5aa6085a1909be34f51a
)

if(NOT logc_POPULATED)
    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
    set(FETCHCONTENT_QUIET FALSE)
    FetchContent_MakeAvailable(logc)

    add_library(logc STATIC)
    target_sources(logc PRIVATE "${logc_SOURCE_DIR}/src/log.c")
    target_include_directories(logc PUBLIC "${logc_SOURCE_DIR}/src/")
    target_compile_definitions(logc PRIVATE LOG_USE_COLOR)
    set_target_properties(logc PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

