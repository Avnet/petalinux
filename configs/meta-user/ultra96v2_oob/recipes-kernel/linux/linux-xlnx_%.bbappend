SRC_URI += "file://bsp.cfg"
SRC_URI += "file://user_2019-10-31-20-33-00.cfg"
            
SRC_URI_append = " file://fix_u96v2_pwrseq_simple.patch"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
