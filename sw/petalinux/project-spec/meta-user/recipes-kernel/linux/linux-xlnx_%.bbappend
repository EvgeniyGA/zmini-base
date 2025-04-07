FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://bsp.cfg"
KERNEL_FEATURES:append = " bsp.cfg"
SRC_URI += "file://user_2025-04-03-19-31-00.cfg \
            file://user_2025-04-05-09-58-00.cfg \
            file://user_2025-04-05-10-15-00.cfg \
            "

