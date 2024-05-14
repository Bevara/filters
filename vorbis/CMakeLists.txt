cmake_minimum_required(VERSION 3.7)
project(Filters)

include(filters.cmake)

add_definitions(-fpic)


SET(VORBIS_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/dmx_ogg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dec_vorbis.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gpac_ogg.c
        ${CMAKE_CURRENT_SOURCE_DIR}/av_parsers.c
)

SET(VORBIS_INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

SET(VORBIS_LIB
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libvorbis.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libogg.a
)

add_filter(vorbis
        "${VORBIS_SRC}"
        "${VORBIS_LIB}"
        []
        GPAC_HAS_VORBIS
        "${VORBIS_INCLUDE}"
        ""
        "1")
