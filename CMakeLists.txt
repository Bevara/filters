include(filters.cmake)

SET(PNG_INC
        ${THIRD_PARTIES}/libpng-code
        ${THIRD_PARTIES_BINARIES}/libpng
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
        ${CMAKE_SOURCE_DIR}/filters/png/dec_png.c
        ${CMAKE_SOURCE_DIR}/filters/png/reframe_png.c
        ${CMAKE_SOURCE_DIR}/filters/common/convert.c
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
)

SET(JPEG_LIB
        ${THIRD_PARTIES_BINARIES}/libjpeg/.libs/libjpeg.a
)

SET(JPEG_SRC
        ${GPAC_LOCATION}/src/media_tools/img.c
        ${GPAC_LOCATION}/src/filters/enc_jpg.c
        ${CMAKE_SOURCE_DIR}/filters/jpeg/dec_jpeg.c
        ${CMAKE_SOURCE_DIR}/filters/jpeg/reframe_jpeg.c
        ${CMAKE_SOURCE_DIR}/filters/common/convert.c
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
        ${CMAKE_SOURCE_DIR}/filters/jxl/reframe_jxl.c
        ${CMAKE_SOURCE_DIR}/filters/jxl/dec_jxl.c
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
)

SET(OPENJPEG_LIB
        ${THIRD_PARTIES_BINARIES}/openjpeg/bin/libopenjp2.a
)

SET(OPENJPEG_SRC
        ${CMAKE_SOURCE_DIR}/filters/jp2/reframe_jp2.c
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
        ${CMAKE_SOURCE_DIR}/filters/bmp/qdbmp.c
)

SET(QDBMP_INC
        ${THIRD_PARTIES}/qdbmp
)

add_filter(qdbmp
        "${QDBMP_SRC}"
        ""
        ['_dynCall_QDBMP_register']
        ""
        "${QDBMP_INC}"
        ""
        "1")


# SET(FF_SRC
#         ${GPAC_LOCATION}/src/filters/ff_dec.c
#         ${GPAC_LOCATION}/src/filters/ff_enc.c
#         ${GPAC_LOCATION}/src/filters/ff_avf.c
#         ${GPAC_LOCATION}/src/filters/ff_dmx.c
#         ${GPAC_LOCATION}/src/filters/ff_rescale.c
#         ${GPAC_LOCATION}/src/filters/ff_common.c
#         ${GPAC_LOCATION}/src/filters/ff_mx.c
# )

# SET(FF_INCLUDE
#         ${THIRD_PARTIES}/ffmpeg
#         ${THIRD_PARTIES_BINARIES}/ffmpeg
#         ${THIRD_PARTIES}/x264
#         ${THIRD_PARTIES_BINARIES}/x264
# )

# SET(FF_LIB
#         ${THIRD_PARTIES_BINARIES}/ffmpeg/libavcodec/libavcodec.a
#         ${THIRD_PARTIES_BINARIES}/ffmpeg/libavutil/libavutil.a
#         ${THIRD_PARTIES_BINARIES}/ffmpeg/libavformat/libavformat.a
#         ${THIRD_PARTIES_BINARIES}/ffmpeg/libavfilter/libavfilter.a
#         ${THIRD_PARTIES_BINARIES}/ffmpeg/libswresample/libswresample.a
#         ${THIRD_PARTIES_BINARIES}/ffmpeg/libswscale/libswscale.a
#         ${THIRD_PARTIES_BINARIES}/x264/libx264.a
# )

# add_filter(ffmpeg
#         "${FF_SRC}"
#         "${FF_LIB}"
#         ['_dynCall_ffdec_register','_dynCall_ffenc_register','_dynCall_ffbsf_register','_dynCall_ffavf_register','_dynCall_ffdmx_register','_dynCall_ffsws_register','_dynCall_ffmx_register']
#         GPAC_HAS_FFMPEG
#         "${FF_INCLUDE}"
#         ""
#         "1")
        
# Should always be at the end of this file
set(WASM_FILES ${WASM_FILES} PARENT_SCOPE)

string(JOIN "," JSON_DESC ${FILTERS_JSON_DESC})

file(WRITE ${CMAKE_BINARY_DIR}/filter_list.json "{${JSON_DESC}}")