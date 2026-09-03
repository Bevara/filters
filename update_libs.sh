#!/bin/bash

echo "Setting environnement"

# find source path
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

# echo "Updating libs for filter ffmpeg-flac"
# cp $build_path/third_parties/ffmpeg-flac/libavcodec/libavcodec.a $source_path/ffmpeg-flac/lib/
# cp $build_path/third_parties/ffmpeg-flac/libavfilter/libavfilter.a $source_path/ffmpeg-flac/lib/
# cp $build_path/third_parties/ffmpeg-flac/libavformat/libavformat.a $source_path/ffmpeg-flac/lib/
# cp $build_path/third_parties/ffmpeg-flac/libavutil/libavutil.a $source_path/ffmpeg-flac/lib/


# echo "Updating libs for filter ffmpeg-hevc"
# cp $build_path/third_parties/ffmpeg-hevc/libavcodec/libavcodec.a $source_path/ffmpeg-hevc/lib/
# cp $build_path/third_parties/ffmpeg-hevc/libavfilter/libavfilter.a $source_path/ffmpeg-hevc/lib/
# cp $build_path/third_parties/ffmpeg-hevc/libavformat/libavformat.a $source_path/ffmpeg-hevc/lib/
# cp $build_path/third_parties/ffmpeg-hevc/libavutil/libavutil.a $source_path/ffmpeg-hevc/lib/

# echo "Updating libs for filter ffmpeg-mpeg1"
# cp $build_path/third_parties/ffmpeg-mpeg1/libavcodec/libavcodec.a $source_path/ffmpeg-mpeg1/lib/
# cp $build_path/third_parties/ffmpeg-mpeg1/libavfilter/libavfilter.a $source_path/ffmpeg-mpeg1/lib/
# cp $build_path/third_parties/ffmpeg-mpeg1/libavformat/libavformat.a $source_path/ffmpeg-mpeg1/lib/
# cp $build_path/third_parties/ffmpeg-mpeg1/libavutil/libavutil.a $source_path/ffmpeg-mpeg1/lib/

# echo "Updating libs for filter ffmpeg-x264"
# cp $build_path/third_parties/ffmpeg-x264/libavcodec/libavcodec.a $source_path/ffmpeg-x264/lib/
# cp $build_path/third_parties/ffmpeg-x264/libavformat/libavformat.a $source_path/ffmpeg-x264/lib/
# cp $build_path/third_parties/ffmpeg-x264/libavutil/libavutil.a $source_path/ffmpeg-x264/lib/
# cp $build_path/third_parties/out/lib/libx264.a $source_path/ffmpeg-x264/lib/


# echo "Updating libs for filter ffdmx"
# cp $build_path/third_parties/ffmpeg-dmx/libavcodec/libavcodec.a $source_path/ffdmx/lib/
# cp $build_path/third_parties/ffmpeg-dmx/libavformat/libavformat.a $source_path/ffdmx/lib/
# cp $build_path/third_parties/ffmpeg-dmx/libavutil/libavutil.a $source_path/ffdmx/lib/


# echo "Updating libs for filter ffmpeg-full"
# cp $build_path/third_parties/ffmpeg-full/libavcodec/libavcodec.a $source_path/ffmpeg-full/lib/
# cp $build_path/third_parties/ffmpeg-full/libavformat/libavformat.a $source_path/ffmpeg-full/lib/
# cp $build_path/third_parties/ffmpeg-full/libavutil/libavutil.a $source_path/ffmpeg-full/lib/
# cp $build_path/third_parties/ffmpeg-full/libavfilter/libavfilter.a $source_path/ffmpeg-full/lib/
# cp $build_path/third_parties/ffmpeg-full/libavdevice/libavdevice.a $source_path/ffmpeg-full/lib/
# cp $build_path/third_parties/ffmpeg-full/libswscale/libswscale.a $source_path/ffmpeg-full/lib/

copy_lib() {
    local search_dir="$1"
    local lib_name="$2"
    local dest_dir="$3"
    local found
    found=$(find "$search_dir" -name "$lib_name" 2>/dev/null | head -1)
    if [ -n "$found" ]; then
        cp "$found" "$dest_dir"
    else
        echo "WARNING: $lib_name not found under $search_dir - filter using it may fail to link"
    fi
}

echo "Updating libs for filter isobmff"
cp $build_path/third_parties/ffmpeg-dmx/libavcodec/libavcodec.a $source_path/isobmff/lib/
cp $build_path/third_parties/ffmpeg-dmx/libavformat/libavformat.a $source_path/isobmff/lib/
cp $build_path/third_parties/ffmpeg-dmx/libavutil/libavutil.a $source_path/isobmff/lib/

echo "Updating lib for filter liba52"
cp $build_path/third_parties/liba52/liba52/.libs/liba52.a $source_path/liba52/lib/

echo "Updating lib for filter libjpeg"
cp $build_path/third_parties/libjpeg/.libs/libjpeg.a $source_path/libjpeg/lib/


echo "Updating lib for filter libjxl"
cp $build_path/third_parties/libjxl/lib/libjxl.a $source_path/libjxl/lib/
cp $build_path/third_parties/brotli/.libs/libbrotlicommon.a $source_path/libjxl/lib/
cp $build_path/third_parties/brotli/.libs/libbrotlidec.a $source_path/libjxl/lib/
cp $build_path/third_parties/higway/libhwy.a $source_path/libjxl/lib/

echo "Updating lib for filter libpng"
cp $build_path/third_parties/libpng/libpng16.a $source_path/libpng/lib/

echo "Updating lib for filter openjpeg"
cp $build_path/third_parties/openjpeg/bin/libopenjp2.a $source_path/openjpeg/lib/

echo "Updating lib for filter ogg"
copy_lib $build_path/third_parties/ogg libogg.a $source_path/ogg/lib/

echo "Updating lib for filter vorbis"
cp $build_path/third_parties/ogg/libogg.a $source_path/vorbis/lib/
cp $build_path/third_parties/vorbis/lib/libvorbis.a $source_path/vorbis/lib/

echo "Updating lib for filter libmad"
cp $build_path/third_parties/libmad/.libs/libmad.a $source_path/libmad/lib/

echo "Updating lib for filter libxvid"
cp $build_path/third_parties/xvidcore/libxvidcore.a $source_path/libxvid/lib/

echo "Updating lib for filter theora"
cp $build_path/third_parties/theora/lib/.libs/libtheora.a $source_path/theora/lib/

echo "Updating lib for filter h264bsd"
copy_lib $build_path/third_parties/h264bsd libh264bsd.a $source_path/h264bsd/lib/

# Note: qdbmp, rfpcm, bifsdec, avidmx, webmdmx and avif deliberately have no
# entry here - their CMakeLists.txt link no prebuilt static lib at all
# (e.g. webmdmx compiles a vendored nestegg.c directly as filter source).

# copy_lib searches recursively under a third_parties build directory for a
# named .a and copies the first match, rather than hardcoding an exact path -
# used below for libraries whose CMake/autotools output subdirectory isn't
# already a verified, known-correct path in this script (unlike the entries
# above, which have been running successfully). Prints a warning instead of
# failing hard if a lib is missing, so one missing/renamed third-party build
# doesn't abort the whole update.

echo "Updating lib for filter libx264 (native encoder, not ffmpeg-x264)"
cp $build_path/third_parties/out/lib/libx264.a $source_path/libx264/lib/

echo "Updating lib for filter libx265"
cp $build_path/third_parties/x265_git/libx265.a $source_path/libx265/lib/

echo "Updating lib for filter liblame"
cp $build_path/third_parties/lame-4.0/libmp3lame/.libs/libmp3lame.a $source_path/liblame/lib/

echo "Updating lib for filter libopus"
copy_lib $build_path/third_parties/opus libopus.a $source_path/libopus/lib/

echo "Updating lib for filter libopusenc"
copy_lib $build_path/third_parties/opus libopus.a $source_path/libopusenc/lib/

echo "Updating lib for filter libvpx"
copy_lib $build_path/third_parties/libvpx libvpx.a $source_path/libvpx/lib/

echo "Updating lib for filter libaom"
copy_lib $build_path/third_parties/libaom libaom.a $source_path/libaom/lib/
# libaom_version.a's exact build target is unverified - flag if missing
copy_lib $build_path/third_parties/libaom libaom_version.a $source_path/libaom/lib/

echo "Updating lib for filter libde265"
copy_lib $build_path/third_parties/libde265 libde265.a $source_path/libde265/lib/

echo "Updating lib for filter libheif"
copy_lib $build_path/third_parties/libheif libheif.a $source_path/libheif/lib/
copy_lib $build_path/third_parties/libde265 libde265.a $source_path/libheif/lib/
# libc++/libc++abi deliberately NOT copied here any more - see the comment in
# libheif/CMakeLists.txt: a second C++ runtime inside the side module breaks
# libc++'s static state across the module boundary.

echo "Updating lib for filter libraw"
copy_lib $build_path/third_parties/libraw libraw.a $source_path/libraw/lib/

echo "Updating lib for filter libtiff"
copy_lib $build_path/third_parties/libtiff libtiff.a $source_path/libtiff/lib/

echo "Updating lib for filter libfaad"
copy_lib $build_path/third_parties/libfaad libfaad.a $source_path/libfaad/lib/

echo "Updating lib for filter libflac"
# build_thirdparties.sh builds this in $build_path/flac (not "libflac")
copy_lib $build_path/third_parties/flac libFLAC.a $source_path/libflac/lib/

echo "Updating lib for filter libaif"
# build_thirdparties.sh builds this in $build_path/libaiff (not "libaif")
copy_lib $build_path/third_parties/libaiff libaiff.a $source_path/libaif/lib/

echo "Updating lib for filter libmpg123"
# build_thirdparties.sh builds this in $build_path/mpg123 (not "libmpg123")
copy_lib $build_path/third_parties/mpg123 libmpg123.a $source_path/libmpg123/lib/

echo "Updating lib for filter libmpeg2"
copy_lib $build_path/third_parties/libmpeg2 libmpeg2.a $source_path/libmpeg2/lib/

echo "Updating lib for filter libmidi"
copy_lib $build_path/third_parties/timidity libarc.a $source_path/libmidi/lib/
copy_lib $build_path/third_parties/timidity libunimod.a $source_path/libmidi/lib/
# libtimidity_core.a: build_thirdparties.sh's "Building timidity" step only
# runs "make -C libarc" and "make -C libunimod" - no target appears to
# produce libtimidity_core.a at all. Left here so the gap is visible instead
# of silently missing; if it never appears, the filter's own CMakeLists.txt
# needs checking to see if this lib is genuinely required or stale.
copy_lib $build_path/third_parties/timidity libtimidity_core.a $source_path/libmidi/lib/

echo "Updating lib for filter poppler"
copy_lib $build_path/third_parties/poppler libpoppler.a $source_path/poppler/lib/
copy_lib $build_path/third_parties/poppler libpoppler-cpp.a $source_path/poppler/lib/
# freetype has no dedicated build step in build_thirdparties.sh - poppler's
# own CMake build appears to bundle/vendor it (FONT_CONFIGURATION=generic),
# so this searches poppler's own build tree rather than a separate one
copy_lib $build_path/third_parties/poppler libfreetype.a $source_path/poppler/lib/
# poppler's DCT (embedded JPEG) support needs libjpeg, which neither solver
# exports - see the comment in poppler/CMakeLists.txt. Taken from the shared
# libjpeg build rather than poppler's tree, which does not vendor it.
copy_lib $build_path/third_parties/libjpeg libjpeg.a $source_path/poppler/lib/

echo "Updating lib for filter libbpg"
copy_lib $build_path/third_parties/libbpg libbpgdec.a $source_path/libbpg/lib/
