
include(../filters.cmake)

Set(BMPFULL_SRC
        ${CMAKE_SOURCE_DIR}/filters/bmpfull/bmpDecFull.c
)

add_filter(bmpfull
        "${BMPFULL_SRC}"
        ""
        _dynCall_BMPFull_register
        ""
        ""
        ""
        "1")