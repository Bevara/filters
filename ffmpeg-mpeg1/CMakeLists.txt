cmake_minimum_required(VERSION 3.7)
project(Filters)

include(filters.cmake)

add_definitions(-fpic)

SET(FF_SRC_MPEG1
        ${CMAKE_CURRENT_SOURCE_DIR}/ff_dec-mpeg1.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ff_common.c
        ${CMAKE_CURRENT_SOURCE_DIR}/reframe_mpgvid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dmx_mpegps.c
        ${CMAKE_CURRENT_SOURCE_DIR}/av_parsers.c
        ${CMAKE_CURRENT_SOURCE_DIR}/media_import.c
        ${CMAKE_CURRENT_SOURCE_DIR}/mpeg2_ps.c
)

SET(FF_LIB_MPEG1
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavcodec.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavutil.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavformat.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavfilter.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libswscale.a
)

SET(FF_INCLUDE_MPEG1
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_filter(ffmpeg-mpeg1
        "${FF_SRC_MPEG1}"
        "${FF_LIB_MPEG1}"
        []
        GPAC_HAS_FFMPEG
        "${FF_INCLUDE_MPEG1}"
        ""
        "1")
