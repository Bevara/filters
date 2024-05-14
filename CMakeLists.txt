cmake_minimum_required(VERSION 3.7)
project(Filters)

include(filters.cmake)

add_definitions(-fpic)

set(filters
  libpng
  libjpeg
  libjxl
  openjpeg
  qdbmp
  ffmpeg-flac
  ffmpeg-mpeg1
  ffmpeg-hevc
  rfpcm
  liba52
  vorbis
  isobmff
)

foreach(filter ${filters})
    add_subdirectory(${filter})
    file(GLOB WASM_FILTERS
      "${CMAKE_BINARY_DIR}/filters/${filter}/*.wasm"
    )

  file(COPY ${WASM_FILTERS} DESTINATION ${CMAKE_BINARY_DIR})
endforeach()


SET(FF_SRC
        ${GPAC_LOCATION}/src/filters/ff_dec.c
        ${GPAC_LOCATION}/src/filters/ff_enc.c
        ${GPAC_LOCATION}/src/filters/ff_avf.c
        ${GPAC_LOCATION}/src/filters/ff_dmx.c
        ${GPAC_LOCATION}/src/filters/ff_rescale.c
        ${GPAC_LOCATION}/src/filters/ff_common.c
        ${GPAC_LOCATION}/src/filters/ff_mx.c
        ${GPAC_LOCATION}/src/filters/reframe_mpgvid.c
        ${GPAC_LOCATION}/src/filters/reframe_nalu.c
        ${GPAC_LOCATION}/src/filters/reframe_flac.c
        ${GPAC_LOCATION}/src/filters/mux_isom.c
        ${GPAC_LOCATION}/src/filters/mux_ts.c
        ${GPAC_LOCATION}/src/filters/dmx_mpegps.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
        ${GPAC_LOCATION}/src/media_tools/media_import.c
        ${GPAC_LOCATION}/src/media_tools/mpeg2_ps.c
        ${GPAC_LOCATION}/src/isomedia/isom_intern.c
        ${GPAC_LOCATION}/src/isomedia/drm_sample.c
        ${GPAC_LOCATION}/src/isomedia/track.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

SET(FF_INCLUDE
        ${THIRD_PARTIES}/ffmpeg
        ${THIRD_PARTIES_BINARIES}/ffmpeg
        ${THIRD_PARTIES}/x264
        ${THIRD_PARTIES_BINARIES}/x264
        ${GPAC_LOCATION}/src/filters
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}
)

SET(FF_LIB
        ${THIRD_PARTIES_BINARIES}/ffmpeg/libavcodec/libavcodec.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg/libavutil/libavutil.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg/libavformat/libavformat.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg/libavfilter/libavfilter.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg/libswresample/libswresample.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg/libswscale/libswscale.a
        ${THIRD_PARTIES_BINARIES}/x264/libx264.a
)

# add_filter(ffmpeg
#         "${FF_SRC}"
#         "${FF_LIB}"
#         ['_dynCall_ffdec_register','_dynCall_ffenc_register','_dynCall_ffbsf_register','_dynCall_ffavf_register','_dynCall_ffdmx_register','_dynCall_ffsws_register','_dynCall_ffmx_register','_dynCall_mpgviddmx_register','_dynCall_mp4_mux_register','_dynCall_naludmx_register','_dynCall_m2psdmx_register','_dynCall_flac_dmx_register']
#         GPAC_HAS_FFMPEG
#         "${FF_INCLUDE}"
#         ""
#         "1")





# Should always be at the end of this file
set(WASM_FILES ${WASM_FILES} PARENT_SCOPE)

string(JOIN "," JSON_DESC ${FILTERS_JSON_DESC})

file(WRITE ${CMAKE_BINARY_DIR}/filter_list.json "{${JSON_DESC}}")
