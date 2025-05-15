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
  ffmpeg-x264
  rfpcm
  liba52
  libmad
  vorbis
  isobmff
  theora
  ogg
  bifsdec
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
