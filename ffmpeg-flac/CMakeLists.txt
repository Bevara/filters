cmake_minimum_required(VERSION 3.7)
project(Filters)

include(filters.cmake)

add_definitions(-fpic)

SET(FF_SRC_FLAC
        ${CMAKE_CURRENT_SOURCE_DIR}/ff_dec-flac.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ff_common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/reframe_flac.c
        ${CMAKE_CURRENT_SOURCE_DIR}/bitstream.c
)

SET(FF_LIB_FLAC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavcodec.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavutil.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavformat.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavfilter.a
)

SET(FF_INCLUDE_FLAC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_filter(ffmpeg-flac
        "${FF_SRC_FLAC}"
        "${FF_LIB_FLAC}"
        []
        GPAC_HAS_FFMPEG
        "${FF_INCLUDE_FLAC}"
        ""
        "1")
