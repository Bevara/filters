cmake_minimum_required(VERSION 3.7)
project(Filters)

include(filters.cmake)

add_definitions(-fpic)

SET(PNG_INC
        ${THIRD_PARTIES}/libpng-code
        ${THIRD_PARTIES_BINARIES}/libpng
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}        
)

if(DEBUG_MODE)
        SET(PNG_LIB
                ${THIRD_PARTIES_BINARIES}/libpng/libpng16d.a
        )
else()
        SET(PNG_LIB
                ${THIRD_PARTIES_BINARIES}/libpng/libpng16.a
        )
endif()


SET(LIBPNG_SRC
        ${GPAC_LOCATION}/src/media_tools/img.c
        ${GPAC_LOCATION}/src/filters/enc_png.c
        ${CMAKE_SOURCE_DIR}/png/dec_png.c
        ${CMAKE_SOURCE_DIR}/png/reframe_png.c
        ${CMAKE_SOURCE_DIR}/common/convert.c
)

SET(LIBPNG_LIBS
        ${PNG_LIB}
)

add_filter(libpng
        "${LIBPNG_SRC}"
        "${LIBPNG_LIBS}"
        ['_dynCall_pngenc_register','_dynCall_pngdec_register','_dynCall_png_reframe_register']
        GPAC_HAS_PNG
        "${PNG_INC}"
        "-s USE_ZLIB=1"
        "1")

        SET(JPGENC_SRC
        ${GPAC_LOCATION}/src/filters/enc_jpg.c
)

SET(JPEG_INC
        ${THIRD_PARTIES}/jpeg-9e
        ${THIRD_PARTIES_BINARIES}/libjpeg
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}
)

SET(JPEG_LIB
        ${THIRD_PARTIES_BINARIES}/libjpeg/.libs/libjpeg.a
)

SET(JPEG_SRC
        ${GPAC_LOCATION}/src/media_tools/img.c
        ${GPAC_LOCATION}/src/filters/enc_jpg.c
        ${CMAKE_SOURCE_DIR}/jpeg/dec_jpeg.c
        ${CMAKE_SOURCE_DIR}/jpeg/reframe_jpeg.c
        ${CMAKE_SOURCE_DIR}/common/convert.c
)

add_filter(libjpeg
        "${JPEG_SRC}"
        "${JPEG_LIB}"
        ['_dynCall_jpgenc_register','_dynCall_jpegdec_register','_dynCall_jpegdec_reframe_register']
        GPAC_HAS_JPEG
        "${JPEG_INC}"
        ""
        "1")

SET(LIBJXL_SRC
        ${CMAKE_SOURCE_DIR}/jxl/reframe_jxl.c
        ${CMAKE_SOURCE_DIR}/jxl/dec_jxl.c
)

SET(LIBJXL_LIB
        ${THIRD_PARTIES_BINARIES}/libjxl/lib/libjxl.a
        ${THIRD_PARTIES_BINARIES}/libjxl/third_party/highway/libhwy.a
        ${THIRD_PARTIES_BINARIES}/libjxl/third_party/brotli/libbrotlicommon.a
        ${THIRD_PARTIES_BINARIES}/libjxl/third_party/brotli/libbrotlidec.a
        ${THIRD_PARTIES}/emsdk/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten/pic/libc.a
        ${THIRD_PARTIES}/emsdk/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten/pic/libc++.a
        ${THIRD_PARTIES}/emsdk/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten/pic/libc++abi.a
)

SET(LIBJXL_INC
        ${THIRD_PARTIES}/libjxl/lib/include
        ${THIRD_PARTIES_BINARIES}/libjxl/lib/include
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}
)

add_filter(libjxl
        "${LIBJXL_SRC}"
        "${LIBJXL_LIB}"
        ['_dynCall_jxldec_register','_dynCall_jxl_reframe_register']
        ""
        "${LIBJXL_INC}"
        ""
        "1")

SET(OPENJPEG_INC
        ${THIRD_PARTIES}/openjpeg/src/lib/openjp2
        ${THIRD_PARTIES_BINARIES}/openjpeg/src/lib/openjp2
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}
)

SET(OPENJPEG_LIB
        ${THIRD_PARTIES_BINARIES}/openjpeg/bin/libopenjp2.a
)

SET(OPENJPEG_SRC
        ${CMAKE_SOURCE_DIR}/jp2/reframe_jp2.c
        ${GPAC_LOCATION}/src/filters/dec_j2k.c
)

add_filter(openjpeg
        "${OPENJPEG_SRC}"
        "${OPENJPEG_LIB}"
        ['_dynCall_jp2_reframe_register','_dynCall_j2kdec_register']
        GPAC_HAS_JP2
        "${OPENJPEG_INC}"
        ""
        "1")

SET(QDBMP_SRC
        ${CMAKE_SOURCE_DIR}/bmp/qdbmp.c
)

SET(QDBMP_INC
        ${THIRD_PARTIES}/qdbmp
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}
)

add_filter(qdbmp
        "${QDBMP_SRC}"
        ""
        ['_dynCall_QDBMP_register']
        ""
        "${QDBMP_INC}"
        ""
        "1")


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

SET(FF_INCLUDE_FLAC
        ${THIRD_PARTIES}/ffmpeg
        ${THIRD_PARTIES_BINARIES}/ffmpeg-flac
        ${GPAC_LOCATION}/src/filters
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}
)

SET(FF_SRC_FLAC
        ${CMAKE_SOURCE_DIR}/ffmpeg-flac/ff_dec-flac.c
        ${GPAC_LOCATION}/src/filters/ff_common.c
        ${GPAC_LOCATION}/src/filters/reframe_flac.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

SET(FF_LIB_FLAC
        ${THIRD_PARTIES_BINARIES}/ffmpeg-flac/libavcodec/libavcodec.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg-flac/libavutil/libavutil.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg-flac/libavformat/libavformat.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg-flac/libavfilter/libavfilter.a
)

add_filter(ffmpeg-flac
        "${FF_SRC_FLAC}"
        "${FF_LIB_FLAC}"
        ['_dynCall_ffdec_flac_register','_dynCall_flac_dmx_register']
        GPAC_HAS_FFMPEG
        "${FF_INCLUDE_FLAC}"
        ""
        "1")

SET(FF_INCLUDE_MPEG1
        ${THIRD_PARTIES}/ffmpeg
        ${THIRD_PARTIES_BINARIES}/ffmpeg-mpeg1
        ${GPAC_LOCATION}/src/filters
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}
)

SET(FF_SRC_MPEG1
        ${CMAKE_SOURCE_DIR}/ffmpeg-mpeg1/ff_dec-mpeg1.c
        ${GPAC_LOCATION}/src/filters/ff_common.c
        ${GPAC_LOCATION}/src/filters/reframe_mpgvid.c
        ${GPAC_LOCATION}/src/filters/dmx_mpegps.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
        ${GPAC_LOCATION}/src/media_tools/media_import.c
        ${GPAC_LOCATION}/src/media_tools/mpeg2_ps.c
)

SET(FF_LIB_MPEG1
        ${THIRD_PARTIES_BINARIES}/ffmpeg-mpeg1/libavcodec/libavcodec.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg-mpeg1/libavutil/libavutil.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg-mpeg1/libavformat/libavformat.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg-mpeg1/libavfilter/libavfilter.a
        ${THIRD_PARTIES_BINARIES}/ffmpeg-mpeg1/libswscale/libswscale.a
)

add_filter(ffmpeg-mpeg1
        "${FF_SRC_MPEG1}"
        "${FF_LIB_MPEG1}"
        ['_dynCall_ffdec_mpeg1_register','_dynCall_mpgviddmx_register','_dynCall_m2psdmx_register']
        GPAC_HAS_FFMPEG
        "${FF_INCLUDE_MPEG1}"
        ""
        "1")

SET(RFPCM_SRC
        ${GPAC_LOCATION}/src/filters/reframe_rawpcm.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
        ${GPAC_LOCATION}/src/utils/constants.c
)

SET(RFPCM_INC
        ${GPAC_LOCATION}/include
        ${GPAC_BINARIES}
)


add_filter(rfpcm
        "${RFPCM_SRC}"
        ""
        _dynCall_pcmreframe_register
        ""
        "${RFPCM_INC}"
        ""
        "1")

SET(LIBA52_SRC
        ${GPAC_LOCATION}/src/filters/dec_ac52.c
        ${GPAC_LOCATION}/src/filters/reframe_ac3.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
        ${GPAC_LOCATION}/src/odf/descriptors.c
)

SET(LIBA52_INCLUDE
        ${GPAC_BINARIES}
        ${GPAC_LOCATION}/include
        ${THIRD_PARTIES}/a52dec-0.7.4/include
)

SET(LIBA52_LIB
        ${THIRD_PARTIES_BINARIES}/liba52/liba52/.libs/liba52.a
)

add_filter(liba52
        "${LIBA52_SRC}"
        "${LIBA52_LIB}"
        ['_dynCall_rfac3_register','_dynCall_a52dec_register']
        GPAC_HAS_LIBA52
        "${LIBA52_INCLUDE}"
        ""
        "1")


SET(VORBIS_SRC
        ${GPAC_LOCATION}/src/filters/dmx_ogg.c
        ${GPAC_LOCATION}/src/filters/dec_vorbis.c
        ${GPAC_LOCATION}/src/media_tools/gpac_ogg.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
)

SET(VORBIS_INCLUDE
        ${GPAC_BINARIES}
        ${GPAC_LOCATION}/include
        ${THIRD_PARTIES}/ogg/include
        ${THIRD_PARTIES_BINARIES}/ogg/include
        ${THIRD_PARTIES}/vorbis/include
)

SET(VORBIS_LIB
        ${THIRD_PARTIES_BINARIES}/vorbis/lib/libvorbis.a
        ${THIRD_PARTIES_BINARIES}/ogg/libogg.a
)

SET(VORBIS_FLAG
        GPAC_HAS_VORBIS
)

add_filter(vorbis
        "${VORBIS_SRC}"
        "${VORBIS_LIB}"
        ['_dynCall_vorbisdec_register','_dynCall_oggdmx_register']
        "${VORBIS_FLAG}"
        "${VORBIS_INCLUDE}"
        ""
        "1")

# Should always be at the end of this file
set(WASM_FILES ${WASM_FILES} PARENT_SCOPE)

string(JOIN "," JSON_DESC ${FILTERS_JSON_DESC})

file(WRITE ${CMAKE_BINARY_DIR}/filter_list.json "{${JSON_DESC}}")