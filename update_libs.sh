#!/bin/bash

echo "Setting environnement"

find source path
source_path="`echo $0 | sed -e 's#/update_libs.sh##'`"
source_path_used="yes"
if test -z "$source_path" -o "$source_path" = "." ; then
    source_path="`pwd`"
    source_path_used="no"
    build_path=$source_path
else
    source_path="`cd \"$source_path\"; pwd`"
    build_path="`pwd`"
fi

echo "Updating libs for filter ffmpeg-flac"
cp $build_path/third_parties/ffmpeg-flac/libavcodec/libavcodec.a $source_path/ffmpeg-flac/lib/
cp $build_path/third_parties/ffmpeg-flac/libavfilter/libavfilter.a $source_path/ffmpeg-flac/lib/
cp $build_path/third_parties/ffmpeg-flac/libavformat/libavformat.a $source_path/ffmpeg-flac/lib/
cp $build_path/third_parties/ffmpeg-flac/libavutil/libavutil.a $source_path/ffmpeg-flac/lib/


echo "Updating libs for filter ffmpeg-hevc"
cp $build_path/third_parties/ffmpeg-hevc/libavcodec/libavcodec.a $source_path/ffmpeg-hevc/lib/
cp $build_path/third_parties/ffmpeg-hevc/libavfilter/libavfilter.a $source_path/ffmpeg-hevc/lib/
cp $build_path/third_parties/ffmpeg-hevc/libavformat/libavformat.a $source_path/ffmpeg-hevc/lib/
cp $build_path/third_parties/ffmpeg-hevc/libavutil/libavutil.a $source_path/ffmpeg-hevc/lib/

echo "Updating libs for filter ffmpeg-mpeg1"
cp $build_path/third_parties/ffmpeg-mpeg1/libavcodec/libavcodec.a $source_path/ffmpeg-mpeg1/lib/
cp $build_path/third_parties/ffmpeg-mpeg1/libavfilter/libavfilter.a $source_path/ffmpeg-mpeg1/lib/
cp $build_path/third_parties/ffmpeg-mpeg1/libavformat/libavformat.a $source_path/ffmpeg-mpeg1/lib/
cp $build_path/third_parties/ffmpeg-mpeg1/libavutil/libavutil.a $source_path/ffmpeg-mpeg1/lib/

echo "Updating libs for filter ffmpeg-x264"
cp $build_path/third_parties/ffmpeg-x264/libavcodec/libavcodec.a $source_path/ffmpeg-x264/lib/
cp $build_path/third_parties/ffmpeg-x264/libavformat/libavformat.a $source_path/ffmpeg-x264/lib/
cp $build_path/third_parties/ffmpeg-x264/libavutil/libavutil.a $source_path/ffmpeg-x264/lib/
cp $build_path/third_parties/out/lib/libx264.a $source_path/ffmpeg-x264/lib/


echo "Updating lib for filter liba52"
cp $build_path/third_parties/liba52/liba52/.libs/liba52.a $source_path/liba52/lib/

echo "Updating lib for filter libjpeg"
cp $build_path/third_parties/libjpeg/.libs/libjpeg.a $source_path/libjpeg/lib/


echo "Updating lib for filter libjxl"
cp $build_path/third_parties/libjxl/lib/libjxl.a $source_path/libjxl/lib/
cp $build_path/third_parties/brotli/.libs/libbrotlicommon.a $source_path/libjxl/lib/
cp $build_path/third_parties/brotli/.libs/libbrotlidec.a $source_path/libjxl/lib/
cp $build_path/third_parties/higway/libhwy.a $source_path/libjxl/lib/
cp $source_path/third_parties/emsdk/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten/pic/libc.a $source_path/libjxl/lib/
cp $source_path/third_parties/emsdk/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten/pic/libc++.a $source_path/libjxl/lib/
cp $source_path/third_parties/emsdk/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten/pic/libc++abi.a $source_path/libjxl/lib/

echo "Updating lib for filter libpng"
cp $build_path/third_parties/libpng/libpng16.a $source_path/libpng/lib/

echo "Updating lib for filter openjpeg"
cp $build_path/third_parties/openjpeg/bin/libopenjp2.a $source_path/openjpeg/lib/

echo "Updating lib for filter vorbis"
cp $build_path/third_parties/ogg/libogg.a $source_path/vorbis/lib/
cp $build_path/third_parties/vorbis/lib/libvorbis.a $source_path/vorbis/lib/

echo "Updating lib for filter libmad"
cp $build_path/third_parties/libmad/.libs/libmad.a $source_path/libmad/lib/

echo "Updating lib for filter libxvid"
cp $build_path/third_parties/xvidcore/libxvidcore.a $source_path/libxvid/lib/

echo "Updating lib for filter theora"
cp $build_path/third_parties/theora/lib/.libs/libtheora.a $source_path/theora/lib/
