include(filters.cmake)

Set(NANOJPEG_SRC
        ${CMAKE_SOURCE_DIR}/filters/nanojpeg.c
        ${THIRD_PARTIES}/nanojpeg/trunk/nanojpeg/nanojpeg.c
)

add_filter(nanojpeg
        "${NANOJPEG_SRC}"
        ""
        _dynCall_nanojpeg_register
        ""
        ""
        ""
        "1")

SET(IMG_DEC_SRC
        ${GPAC_LOCATION}/src/filters/dec_img.c
        ${GPAC_LOCATION}/src/media_tools/img.c
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

SET(JPEG_LIB
        ${THIRD_PARTIES_BINARIES}/libjpeg/.libs/libjpeg.a
)

SET(IMG_FLAGS
        GPAC_HAS_JPEG
        GPAC_HAS_PNG
)

SET(IMG_INC
        ${THIRD_PARTIES}/libpng-code
        ${THIRD_PARTIES}/jpeg-9e
        ${THIRD_PARTIES_BINARIES}/libpng
        ${THIRD_PARTIES_BINARIES}/libjpeg
)

SET(IMG_LIBS
        ${PNG_LIB}
        ${JPEG_LIB}
)

add_filter(imgdec
        "${IMG_DEC_SRC}"
        "${IMG_LIBS}"
        _dynCall_imgdec_register
        "${IMG_FLAGS}"
        "${IMG_INC}"
        ""
        "1")

SET(RFIMG_SRC
        ${GPAC_LOCATION}/src/filters/reframe_img.c
)

add_filter(rfimg
        "${RFIMG_SRC}"
        "${IMG_LIBS}"
        _dynCall_img_reframe_register
        "${IMG_FLAGS}"
        "${IMG_INC}"
        ""
        "1")

SET(PNGENC_SRC
        ${GPAC_LOCATION}/src/filters/enc_png.c
)

SET(PNGENC_LIBS
        ${PNG_LIB}
)

add_filter(pngenc
        "${PNGENC_SRC}"
        "${PNGENC_LIBS}"
        _dynCall_pngenc_register
        GPAC_HAS_PNG
        "${IMG_INC}"
        ""
        "1")

SET(JPGENC_SRC
        ${GPAC_LOCATION}/src/filters/enc_jpg.c
)

add_filter(jpgenc
        "${JPGENC_SRC}"
        "${JPEG_LIB}"
        _dynCall_jpgenc_register
        GPAC_HAS_JPEG
        "${IMG_INC}"
        ""
        "1")

SET(J2KDEC_INC
        ${THIRD_PARTIES}/openjpeg/src/lib/openjp2
        ${THIRD_PARTIES_BINARIES}/openjpeg/src/lib/openjp2
)

SET(J2KDEC_LIB
        ${THIRD_PARTIES_BINARIES}/openjpeg/bin/libopenjp2.a
)

SET(J2KDEC_SRC
        ${GPAC_LOCATION}/src/filters/dec_j2k.c
)

add_filter(j2kdec
        "${J2KDEC_SRC}"
        "${J2KDEC_LIB}"
        _dynCall_j2kdec_register
        GPAC_HAS_JP2
        "${J2KDEC_INC}"
        ""
        "1")

SET(LIBMAD_SRC
        ${GPAC_LOCATION}/src/filters/dec_mad.c
        ${GPAC_LOCATION}/src/filters/reframe_mp3.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
)

SET(LIBMAD_LIB
        ${THIRD_PARTIES_BINARIES}/libmad/.libs/libmad.a
)

SET(LIBMAD_INC
        ${THIRD_PARTIES_BINARIES}/libmad
        ${GPAC_LOCATION}/include
)

add_filter(libmad
        "${LIBMAD_SRC}"
        "${LIBMAD_LIB}"        
        ['_dynCall_maddec_register','_dynCall_mp3_dmx_register']
        GPAC_HAS_MAD
        "${LIBMAD_INC}"
        ""
        "1")

SET(FAAD_SRC
        ${GPAC_LOCATION}/src/filters/reframe_adts.c
        ${GPAC_LOCATION}/src/filters/dec_faad.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

SET(FAAD_INCLUDE
        ${THIRD_PARTIES}/faad2-2.8.8/include
)

SET(FAAD_LIB
        ${THIRD_PARTIES_BINARIES}/libfaad/libfaad/.libs/libfaad.a
)

add_filter(libfaad
        "${FAAD_SRC}"
        "${FAAD_LIB}"
        ['_dynCall_faad_register','_dynCall_adts_dmx_register']
        GPAC_HAS_FAAD
        "${FAAD_INCLUDE}"
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

SET(OPENHEVCDEC_INCLUDE
        ${THIRD_PARTIES}/openHEVC
)

SET(OPENHEVCDEC_LIB
        ${THIRD_PARTIES_BINARIES}/openHEVC/libopenhevc/libopenhevc.a
)


SET(OPENHEVCDEC_SRC
        ${GPAC_LOCATION}/src/filters/dec_openhevc.c
        ${GPAC_LOCATION}/src/filters/tilesplit.c
        ${GPAC_LOCATION}/src/filters/tileagg.c
        ${GPAC_LOCATION}/src/filters/hevcsplit.c
        ${GPAC_LOCATION}/src/filters/hevcmerge.c
)

add_filter(libopenhevc
        "${OPENHEVCDEC_SRC}"
        "${OPENHEVCDEC_LIB}"
        ['_dynCall_ohevcdec_register','dynCall_tilesplit_register','_dynCall_tileagg_register','_dynCall_hevcsplit_register','_dynCall_hevcmerge_register']
        "-DGPAC_HAS_OPENHEVC -DGPAC_OPENHEVC_STATIC -DOPENHEVC_HAS_AVC_BASE"
        "${OPENHEVCDEC_INCLUDE}"
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
)

SET(FF_INCLUDE
        ${THIRD_PARTIES}/ffmpeg
        ${THIRD_PARTIES_BINARIES}/ffmpeg
        ${THIRD_PARTIES}/x264
        ${THIRD_PARTIES_BINARIES}/x264
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

add_filter(ffmpeg
        "${FF_SRC}"
        "${FF_LIB}"
        ['_dynCall_ffdec_register','_dynCall_ffenc_register','_dynCall_ffbsf_register','_dynCall_ffavf_register','_dynCall_ffavf_register','_dynCall_ffdmx_register','_dynCall_ffsws_register','_dynCall_ffmx_register']
        GPAC_HAS_FFMPEG
        "${FF_INCLUDE}"
        ""
        "1")

SET(RFFLAC_SRC
        ${GPAC_LOCATION}/src/filters/reframe_flac.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

SET(RFFLAC_LIB

)

add_filter(rfflac
        "${RFFLAC_SRC}"
        "${RFFLAC_LIB}"
        _dynCall_flac_dmx_register
        ""
        ""
        ""
        "1")


SET(XIPH_SRC
        ${GPAC_LOCATION}/src/filters/dmx_ogg.c
        ${GPAC_LOCATION}/src/media_tools/gpac_ogg.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
        ${GPAC_LOCATION}/src/filters/dec_vorbis.c
        ${GPAC_LOCATION}/src/filters/dec_theora.c
)

SET(XIPH_INCLUDE
        ${THIRD_PARTIES}/theora/include
        ${THIRD_PARTIES}/ogg/include
        ${THIRD_PARTIES_BINARIES}/ogg/include
        ${THIRD_PARTIES}/vorbis/include
)

SET(XIPH_LIB
        ${THIRD_PARTIES_BINARIES}/theora/lib/.libs/libtheora.a
        ${THIRD_PARTIES_BINARIES}/vorbis/lib/libvorbis.a
        ${THIRD_PARTIES_BINARIES}/ogg/libogg.a
)

SET(XIPH_FLAG
        GPAC_HAS_THEORA
        GPAC_HAS_VORBIS
)

add_filter(xiph
        "${XIPH_SRC}"
        "${XIPH_LIB}"
        ['_dynCall_theoradec_register','_dynCall_vorbisdec_register','_dynCall_oggdmx_register']
        "${XIPH_FLAG}"
        "${XIPH_INCLUDE}"
        ""
        "1")

SET(XVIDDEC_SRC
        ${GPAC_LOCATION}/src/filters/dec_xvid.c
)

SET(XVIDDEC_INCLUDE
        ${THIRD_PARTIES}/xvidcore/src
)

SET(XVIDDEC_LIB
        ${THIRD_PARTIES_BINARIES}/xvidcore/libxvidcore.a
)

add_filter(xviddec
        "${XVIDDEC_SRC}"
        "${XVIDDEC_LIB}"
        _dynCall_xviddec_register
        GPAC_HAS_XVID
        "${XVIDDEC_INCLUDE}"
        ""
        "1")

SET(RFAMR_SRC
        ${GPAC_LOCATION}/src/filters/reframe_amr.c
)

SET(RFAMR_LIB
)

add_filter(rfamr
        "${RFAMR_SRC}"
        "${RFAMR_LIB}"
        _dynCall_amrdmx_register
        ""
        ""
        ""
        "1")

SET(M2PSDMX_SRC
        ${GPAC_LOCATION}/src/filters/dmx_mpegps.c
        ${GPAC_LOCATION}/src/media_tools/mpeg2_ps.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
        ${GPAC_LOCATION}/src/media_tools/media_import.c
)

SET(M2PSDMX_LIB

)

add_filter(m2psdmx
        "${M2PSDMX_SRC}"
        "${M2PSDMX_LIB}"
        _dynCall_m2psdmx_register
        ""
        ""
        ""
        "1")

SET(RFMPGVID_SRC
        ${GPAC_LOCATION}/src/filters/reframe_mpgvid.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

SET(RFMPGVID_LIB
)

add_filter(rfmpgvid
        "${RFMPGVID_SRC}"
        "${RFMPGVID_LIB}"
        _dynCall_mpgviddmx_register
        ""
        ""
        ""
        "1")

SET(MP4DMX_SRC
        ${GPAC_LOCATION}/src/filters/isoffin_read.c
        ${GPAC_LOCATION}/src/filters/isoffin_load.c
        ${GPAC_LOCATION}/src/filters/isoffin_read_ch.c
        ${GPAC_LOCATION}/src/isomedia/isom_read.c
        ${GPAC_LOCATION}/src/isomedia/isom_store.c
        ${GPAC_LOCATION}/src/isomedia/box_funcs.c
        ${GPAC_LOCATION}/src/isomedia/box_code_base.c
        ${GPAC_LOCATION}/src/isomedia/box_dump.c
        ${GPAC_LOCATION}/src/isomedia/box_code_meta.c
        ${GPAC_LOCATION}/src/isomedia/avc_ext.c
        ${GPAC_LOCATION}/src/isomedia/box_code_drm.c
        ${GPAC_LOCATION}/src/isomedia/box_code_3gpp.c
        ${GPAC_LOCATION}/src/isomedia/box_code_apple.c
        ${GPAC_LOCATION}/src/isomedia/box_code_adobe.c
        ${GPAC_LOCATION}/src/isomedia/isom_intern.c
        ${GPAC_LOCATION}/src/isomedia/data_map.c
        ${GPAC_LOCATION}/src/isomedia/iff.c
        ${GPAC_LOCATION}/src/isomedia/sample_descs.c
        ${GPAC_LOCATION}/src/isomedia/track.c
        ${GPAC_LOCATION}/src/isomedia/media.c
        ${GPAC_LOCATION}/src/isomedia/stbl_read.c
        ${GPAC_LOCATION}/src/media_tools/webvtt.c
)

SET(MP4DMX_LIB
)

add_filter(mp4dmx
        "${MP4DMX_SRC}"
        "${MP4DMX_LIB}"
        _dynCall_mp4dmx_register
        ""
        ""
        ""
        "1")

SET(MP4MX_LIB
)

SET(MP4MX_SRC
        ${GPAC_LOCATION}/src/filters/mux_isom.c
        ${GPAC_LOCATION}/src/isomedia/isom_intern.c
        ${GPAC_LOCATION}/src/filters/mux_ts.c
        ${GPAC_LOCATION}/src/isomedia/drm_sample.c
        ${GPAC_LOCATION}/src/isomedia/track.c
        ${GPAC_LOCATION}/src/media_tools/media_import.c
)

add_filter(mp4mx
        "${MP4MX_SRC}"
        "${MP4MX_LIB}"
        _dynCall_mp4_mux_register
        ""
        ""
        ""
        "1")

SET(RFNALU_SRC
        ${GPAC_LOCATION}/src/filters/reframe_nalu.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
)

SET(RFNALU_LIB
)

add_filter(rfnalu
        "${RFNALU_SRC}"
        "${RFNALU_LIB}"
        _dynCall_naludmx_register
        ""
        ""
        ""
        "1")

SET(AVIDMX_LIB
)


SET(AVIDMX_SRC
        ${GPAC_LOCATION}/src/filters/dmx_avi.c
        ${GPAC_LOCATION}/src/media_tools/avilib.c
        ${GPAC_LOCATION}/src/media_tools/media_import.c
)

add_filter(avidmx
        "${AVIDMX_SRC}"
        "${AVIDMX_LIB}"
        _dynCall_avidmx_register
        ""
        ""
        ""
        "1")

add_filter(ufvtt
        ${GPAC_LOCATION}/src/filters/write_vtt.c
        ""
        _dynCall_vttmx_register
        ""
        ""
        ""
        "1")

add_filter(tx3g2ttml
        ${GPAC_LOCATION}/src/filters/write_tx3g.c
        ""
        _dynCall_tx3g2ttml_register
        ""
        ""
        ""
        "1")

add_filter(tx3g2vtt
        ${GPAC_LOCATION}/src/filters/write_tx3g.c
        ""
        _dynCall_tx3g2ttml_register
        ""
        ""
        ""
        "1")

add_filter(tx3g2srt
        ${GPAC_LOCATION}/src/filters/write_tx3g.c
        ""
        _dynCall_tx3g2ttml_register
        ""
        ""
        ""
        "1")

add_filter(ufttxt
        ${GPAC_LOCATION}/src/filters/write_tx3g.c
        ""
        _dynCall_tx3g2ttml_register
        ""
        ""
        ""
        "1")

SET(WRITEQCP_LIB
)

add_filter(writeqcp
        ${GPAC_LOCATION}/src/filters/write_qcp.c
        "${WRITEQCP_LIB}"
        _dynCall_qcpmx_register
        ""
        ""
        ""
        "1")

add_filter(nhntw
        ${GPAC_LOCATION}/src/filters/write_nhnt.c
        ""
        _dynCall_nhntdump_register
        ""
        ""
        ""
        "1")

add_filter(nhmlw
        ${GPAC_LOCATION}/src/filters/write_nhml.c
        ""
        _dynCall_nhmldump_register
        ""
        ""
        ""
        "1")

add_filter(writeuf
        ${GPAC_LOCATION}/src/filters/write_generic.c
        ""
        _dynCall_writeuf_register
        ""
        ""
        ""
        "1")

add_filter(vflip
        ${GPAC_LOCATION}/src/filters/vflip.c
        ""
        _dynCall_vflip_register
        ""
        ""
        ""
        "1")

add_filter(vcrop
        ${GPAC_LOCATION}/src/filters/vcrop.c
        ""
        _dynCall_vcrop_register
        ""
        ""
        ""
        "1")

add_filter(unframer
        ${GPAC_LOCATION}/src/filters/unframer.c
        ""
        _dynCall_unframer_register
        ""
        ""
        ""
        "1")

add_filter(ttml2srt
        ${GPAC_LOCATION}/src/filters/ttml_conv.c
        ""
        _dynCall_ttmlconv2_register
        ""
        ""
        ""
        "1")

add_filter(ttml2vtt
        ${GPAC_LOCATION}/src/filters/ttml_conv.c
        ""
        _dynCall_dynCall_ttmlconv_register
        ""
        ""
        ""
        "1")

add_filter(tssplit
        ${GPAC_LOCATION}/src/filters/tssplit.c
        ""
        _dynCall_m2tssplit_register
        ""
        ""
        ""
        "1")

add_filter(ufobu
        ${GPAC_LOCATION}/src/filters/rewrite_obu.c
        ""
        _dynCall_obumx_register
        ""
        ""
        ""
        "1")

add_filter(ufnalu
        ${GPAC_LOCATION}/src/filters/rewrite_nalu.c
        ""
        _dynCall_nalumx_register
        ""
        ""
        ""
        "1")

add_filter(ufvc1
        ${GPAC_LOCATION}/src/filters/rewrite_mp4v.c
        ""
        _dynCall_vc1mx_register
        ""
        ""
        ""
        "1")

add_filter(ufmhas
        ${GPAC_LOCATION}/src/filters/rewrite_mhas.c
        ""
        _dynCall_mhasmx_register
        ""
        ""
        ""
        "1")

SET(UFLATM_SRC
        ${GPAC_LOCATION}/src/filters/rewrite_adts.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

add_filter(uflatm
        "${UFLATM_SRC}"
        ""
        _dynCall_latm_mx_register
        ""
        ""
        ""
        "1")

add_filter(rewind
        ${GPAC_LOCATION}/src/filters/rewind.c
        ""
        _dynCall_rewind_register
        ""
        ""
        ""
        "1")

add_filter(restamp
        ${GPAC_LOCATION}/src/filters/restamp.c
        ""
        _dynCall_restamp_register
        ""
        ""
        ""
        "1")

SET(RFTRUEHD_SRC
        ${GPAC_LOCATION}/src/filters/reframe_truehd.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

SET(RFTRUEHD_LIB

)

add_filter(rftruehd
        "${RFTRUEHD_SRC}"
        "${RFTRUEHD_LIB}"
        _dynCall_rftruehd_register
        ""
        ""
        ""
        "1")

SET(RFRAW_SRC
        ${GPAC_LOCATION}/src/filters/reframe_rawvid.c
        ${GPAC_LOCATION}/src/utils/constants.c
)

SET(RFRAW_LIB

)


add_filter(rfrawvid
        "${RFRAW_SRC}"
        "${RFRAW_LIB}"
        _dynCall_rawvidreframe_register
        ""
        ""
        ""
        "1")

SET(RFPCM_SRC
        ${GPAC_LOCATION}/src/filters/reframe_rawpcm.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
        ${GPAC_LOCATION}/src/utils/constants.c
)

SET(RFPCM_LIB

)


add_filter(rfpcm
        "${RFPCM_SRC}"
        "${RFPCM_LIB}"
        _dynCall_pcmreframe_register
        ""
        ""
        ""
        "1")

SET(RFQCP_SRC
        ${GPAC_LOCATION}/src/filters/reframe_qcp.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

SET(RFQCP_LIB

)

add_filter(rfqcp
        "${RFQCP_SRC}"
        "${RFQCP_LIB}"
        _dynCall_qcpdmx_register
        ""
        ""
        ""
        "1")

add_filter(rfprores
        ${GPAC_LOCATION}/src/filters/reframe_prores.c
        ""
        _dynCall_proresdmx_register
        ""
        ""
        ""
        "1")

SET(RFMHAS_LIB

)

add_filter(rfmhas
        ${GPAC_LOCATION}/src/filters/reframe_mhas.c
         "${RFMHAS_LIB}"
         _dynCall_rfmhas_register
        ""
        ""
        ""
        "1")

SET(LATM_SRC
        ${GPAC_LOCATION}/src/filters/reframe_latm.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
)

SET(LATM_LIB

)


add_filter(rflatm
        "${LATM_SRC}"
        "${LATM_LIB}"
        _dynCall_latm_dmx_register
        ""
        ""
        ""
        "1")

SET(RFH263_SRC
        ${GPAC_LOCATION}/src/filters/reframe_h263.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
)

add_filter(rfh263
        "${RFH263_SRC}"
        ""
        _dynCall_h263dmx_register
        ""
        ""
        ""
        "1")

SET(RFAV1_SRC
        ${GPAC_LOCATION}/src/filters/reframe_av1.c
        ${GPAC_LOCATION}/src/media_tools/av_parsers.c
        ${GPAC_LOCATION}/src/utils/alloc.c
        ${GPAC_LOCATION}/src/utils/error.c
        ${GPAC_LOCATION}/src/utils/bitstream.c
        ${GPAC_LOCATION}/src/utils/list.c
        ${GPAC_LOCATION}/src/odf/descriptors.c
)

SET(RFAV1_LIB

)


add_filter(rfav1
        "${RFAV1_SRC}"
        "${RFAV1_LIB}"
        _dynCall_av1dmx_register
        ""
        ""
        ""
        "1")

add_filter(m2tsmx
        ${GPAC_LOCATION}/src/filters/mux_ts.c
        ""
        _dynCall_tsmux_register
        ""
        ""
        ""
        "1")

add_filter(oggmx
        ${GPAC_LOCATION}/src/filters/mux_ogg.c
        ""
        _dynCall_oggmux_register
        ""
        ""
        ""
        "1")

add_filter(gsfmx
        ${GPAC_LOCATION}/src/filters/mux_gsf.c
        ""
        _dynCall_gsfmx_register
        ""
        ""
        ""
        "1")

 SET(RFSRT_LIB

)


SET(RFSRT_SRC
        ${GPAC_LOCATION}/src/filters/load_text.c
        ${GPAC_LOCATION}/src/utils/utf.c
)

add_filter(rfsrt
        "${RFSRT_SRC}"
        "${RFSRT_LIB}"
        _dynCall_rfsrt_register
        ""
        ""
        ""
        "1")

SET(VTT2TX3G_SRC
        ${GPAC_LOCATION}/src/filters/load_text.c
        ${GPAC_LOCATION}/src/utils/utf.c
)

SET(VTT2TX3G_LIB

)

add_filter(vtt2tx3g
        "${VTT2TX3G_SRC}"
        "${VTT2TX3G_LIB}"
        _dynCall_vtt2tx3g_register
        ""
        ""
        ""
        "1")

SET(TXTIN_LIB

)

SET(TXTIN_SRC
        ${GPAC_LOCATION}/src/filters/load_text.c
        ${GPAC_LOCATION}/src/utils/utf.c
)

add_filter(txtin
        "${TXTIN_SRC}"
        "${TXTIN_LIB}"
        _dynCall_txtin_register
        ""
        ""
        ""
        "1")

SET(SVG_LIBS

)

SET(SVGIN_SRC
        ${GPAC_LOCATION}/src/filters/load_svg.c
)

add_filter(svgplay
        "${SVGIN_SRC}"
        "${SVG_LIBS}"
        _dynCall_svgin_register
        ""
        ""
        ""
        "1")

SET(BTPLAY_LIB

)

SET(BTPLAY_SRC
        ${GPAC_LOCATION}/src/filters/load_bt_xmt.c
        ${GPAC_LOCATION}/src/utils/utf.c
)

add_filter(btplay
        "${BTPLAY_SRC}"
        "${BTPLAY_LIB}"
        _dynCall_ctxload_register
        ""
        ""
        ""
        "1")

SET(CRYPTIN_LIBS

)

add_filter(cryptin
        ${GPAC_LOCATION}/src/filters/io_fcryp.c
        "${CRYPTIN_LIBS}"
        _dynCall_cryptfin_register
        ""
        ""
        ""
        "1")

SET(CRYPTOUT_LIBS

)

add_filter(cryptout
        ${GPAC_LOCATION}/src/filters/io_fcryp.c
        "${CRYPTOUT_LIBS}"
        _dynCall_cryptfout_register
        ""
        ""
        ""
        "1")

add_filter(probe
        ${GPAC_LOCATION}/src/filters/inspect.c
        ""
        _dynCall_probe_register
        ""
        ""
        ""
        "1")

add_filter(inspect
        ${GPAC_LOCATION}/src/filters/inspect.c
        ""
        _dynCall_inspect_register
        ""
        ""
        ""
        "1")

SET(FLIST_SRC
        ${GPAC_LOCATION}/src/filters/filelist.c
        ${GPAC_LOCATION}/src/utils/utf.c
)

add_filter(flist
        "${FLIST_SRC}"
        ""
        _dynCall_filelist_register
        ""
        ""
        ""
        "1")

add_filter(cecrypt
        ${GPAC_LOCATION}/src/filters/encrypt_cenc_isma.c
        ""
        _dynCall_cenc_encrypt_register
        ""
        ""
        ""
        "1")

SET(VOBSUBDMX_LIB

)

add_filter(vobsubdmx
        ${GPAC_LOCATION}/src/filters/dmx_vobsub.c
        "${VOBSUBDMX_LIB}"
        _dynCall_vobsubdmx_register
        ""
        ""
        ""
        "1")

add_filter(safdmx
        ${GPAC_LOCATION}/src/filters/dmx_saf.c
        ""
        _dynCall_safdmx_register
        ""
        ""
        ""
        "1")

add_filter(nhntr
        ${GPAC_LOCATION}/src/filters/dmx_nhnt.c
        ""
        _dynCall_nhntdmx_register
        ""
        ""
        ""
        "1")

add_filter(nhmlr
        ${GPAC_LOCATION}/src/filters/dmx_nhml.c
        ""
        _dynCall_nhmldmx_register
        ""
        ""
        ""
        "1")

SET(M2TSDMX_LIB

)

SET(M2TSDMX_SRC
        ${GPAC_LOCATION}/src/filters/dmx_m2ts.c
        ${GPAC_LOCATION}/src/media_tools/mpegts.c
        ${GPAC_LOCATION}/src/utils/alloc.c
        ${GPAC_LOCATION}/src/utils/error.c
        ${GPAC_LOCATION}/src/utils/list.c
)

add_filter(m2tsdmx
        "${M2TSDMX_SRC}"
        "${M2TSDMX_LIB}"
        _dynCall_m2tsdmx_register
        ""
        ""
        ""
        "1")

SET(GSFDMX_LIB
        ${FF_LIB}

)

add_filter(gsfdmx
        ${GPAC_LOCATION}/src/filters/dmx_gsf.c
        "${GSFDMX_LIB}"
        _dynCall_gsfdmx_register
        ""
        ""
        ""
        "1")

SET(DASHIN_LIB

)

add_filter(dashin
        ${GPAC_LOCATION}/src/filters/dmx_dash.c
        "${DASHIN_LIB}"
        _dynCall_dashdmx_register
        ""
        ""
        ""
        "1")

add_filter(cdcrypt
        ${GPAC_LOCATION}/src/filters/decrypt_cenc_isma.c
        ""
        _dynCall_cenc_decrypt_register
        ""
        ""
        ""
        "1")

add_filter(vttdec
        ${GPAC_LOCATION}/src/filters/dec_webvtt.c
        ""
        _dynCall_vttdec_register
        ""
        ""
        ""
        "1")

add_filter(ttxtdec
        ${GPAC_LOCATION}/src/filters/dec_ttxt.c
        ""
        _dynCall_ttxtdec_register
        ""
        ""
        ""
        "1")

add_filter(ttmldec
        ${GPAC_LOCATION}/src/filters/dec_ttml.c
        ""
        _dynCall_ttmldec_register
        ""
        ""
        ""
        "1")

add_filter(odfdec
        ${GPAC_LOCATION}/src/filters/dec_odf.c
        ""
        _dynCall_odf_dec_register
        ""
        ""
        ""
        "1")

SET(LSRDRC_SRC
        ${GPAC_LOCATION}/src/filters/dec_laser.c
        ${GPAC_LOCATION}/src/compositor/scene.c
)

add_filter(lsrdec
        "${LSRDRC_SRC}"
        ""
        _dynCall_lsrdec_register
        ""
        ""
        ""
        "1")

add_filter(bifsdec
        ${GPAC_LOCATION}/src/filters/dec_bifs.c
        ""
        _dynCall_bifs_dec_register
        ""
        ""
        ""
        "1")

SET(DASHER_SRC
        ${GPAC_LOCATION}/src/filters/dasher.c
        ${GPAC_LOCATION}/src/media_tools/mpd.c
)

add_filter(dasher
        "${DASHER_SRC}"
        ""
        _dynCall_dasher_register
        ""
        ""
        ""
        "1")

SET(BSRW_SRC
        ${GPAC_LOCATION}/src/filters/bsrw.c
        ${GPAC_LOCATION}/src/utils/os_config_init.c
)

SET(BSRW_LIB

)

add_filter(bsrw
        "${BSRW_SRC}"
        "${BSRW_LIB}"
        _dynCall_bsrw_register
        ""
        ""
        ""
        "1")

add_filter(bssplit
        ${GPAC_LOCATION}/src/filters/bs_split.c
        ""
        _dynCall_bs_split_register
        ""
        ""
        ""
        "1")

add_filter(bsagg
        ${GPAC_LOCATION}/src/filters/bs_agg.c
        ""
        _dynCall_bs_agg_register
        ""
        ""
        ""
        "1")

SET(LIBJXL_SRC
        ${CMAKE_SOURCE_DIR}/filters/jxl/reframe_jxl.c
        ${CMAKE_SOURCE_DIR}/filters/jxl/dec_jxl.c
)

SET(LIBJXL_LIB
        ${THIRD_PARTIES_BINARIES}/libjxl/lib/libjxl.a
        ${THIRD_PARTIES_BINARIES}/libjxl/third_party/highway/libhwy.a
        ${THIRD_PARTIES_BINARIES}/libjxl/third_party/brotli/libbrotlicommon-static.a
        ${THIRD_PARTIES_BINARIES}/libjxl/third_party/brotli/libbrotlidec-static.a
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

# Should always be at the end of this file
set(WASM_FILES ${WASM_FILES} PARENT_SCOPE)

string(JOIN "," JSON_DESC ${FILTERS_JSON_DESC})

file(WRITE ${CMAKE_BINARY_DIR}/filter_list.json "{${JSON_DESC}}")