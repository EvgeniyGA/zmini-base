FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://bsp.cfg"
KERNEL_FEATURES:append = " bsp.cfg"
SRC_URI += "file://user_2025-04-03-19-31-00.cfg \
            file://user_2025-04-05-09-58-00.cfg \
            file://user_2025-04-05-10-15-00.cfg \
            file://user_2025-04-07-18-30-00.cfg \
            file://user_2025-04-07-18-44-00.cfg \
            file://user_2025-04-08-00-34-00.cfg \
            file://user_2025-04-10-05-59-00.cfg \
            file://user_2025-04-10-06-48-00.cfg \
            file://user_2025-04-13-14-59-00.cfg \
            file://user_2025-04-13-19-53-00.cfg \
            file://user_2025-04-14-10-20-00.cfg \
            "

