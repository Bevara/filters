cmake_minimum_required(VERSION 3.7)
project(Filters)

include(filters.cmake)

add_definitions(-fpic)

SET(FF_SRC_HEVC
        ${CMAKE_CURRENT_SOURCE_DIR}/ff_dec-hevc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ff_rescale-hevc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ff_common.c
)

SET(FF_LIB_HEVC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavcodec/libavcodec.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavutil/libavutil.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavformat/libavformat.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libavfilter/libavfilter.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libswscale/libswscale.a
)

SET(FF_INCLUDE_HEVC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ffmpeg
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_filter(ffmpeg-hevc
        "${FF_SRC_HEVC}"
        "${FF_LIB_HEVC}"
        []
        GPAC_HAS_FFMPEG
        "${FF_INCLUDE_HEVC}"
        ""
        "1")
