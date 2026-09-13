cmake_minimum_required(VERSION 3.7)
project(Filters)

include(filters.cmake)

add_definitions(-fpic)

set(filters
  libpng
  libjpeg
  libjxl
  libraw
  libheif
  libtiff
  openjpeg
  qdbmp
  #ffmpeg-flac
  #ffmpeg-mpeg1
  #ffmpeg-hevc
  #ffmpeg-x264
  #ffmpeg-full
  ffdmx
  rfpcm
  liba52
  libmad
  vorbis
  isobmff
  theora
  ogg
  bifsdec
  libxvid
   avidmx
   flvdmx
   libfaad
  libflac
  libaif
  libopus
  libmpg123
  libmpeg2
  libmidi
  libde265
  libOpenVVC
  libaom
  webmdmx
  avif
  libvpx
  poppler
  qoi
  libwebp
  libgif
  librecoil
  libdds
  libcharls
  libflif
  libjbig
  libjbig2
  libicns
  libxmp
  libgme
  libwavpack
  libamr
  libmpc
  libdca
  libape
  libmng
  libdjvu
  libmupdf
  libjxr
  libpgf
  libpsd
  simpleimg
  libxcf
  libspeex
  libg711
  libalac
  libgsm
  libsbc
  libtta
  liblc3
  libcodec2
  libuavs3d
  libdavs2
  libxevd
  libutvideo
  libschro
  libilbc
  libsilk
  libisac
  rgbyuv
  btplay
  libbpg
  h264bsd
  ffmpeg-h26x
  ffmpeg-ffv1
  #ffmpeg-vc2
  ffmpeg-g726
  libx264
  libx265
  liblame
  libfaac
  libopusenc
)

foreach(filter ${filters})
    add_subdirectory(${filter})
    file(GLOB WASM_FILTERS
      "${CMAKE_BINARY_DIR}/filters/${filter}/*.wasm"
    )

  file(COPY ${WASM_FILTERS} DESTINATION ${CMAKE_BINARY_DIR})
endforeach()


# Should always be at the end of this file
set(WASM_FILES ${WASM_FILES} PARENT_SCOPE)

string(JOIN "," JSON_DESC ${FILTERS_JSON_DESC})

file(WRITE ${CMAKE_BINARY_DIR}/filter_list.json "{${JSON_DESC}}")
