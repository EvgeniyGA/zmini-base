FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://platform-top.h file://bsp.cfg"
SRC_URI += "file://user_2025-04-03-19-38-00.cfg \
            file://user_2025-04-08-05-25-00.cfg \
            file://user_2025-04-08-18-00-00.cfg \
            file://user_2025-04-08-18-21-00.cfg \
            file://user_2025-04-08-18-35-00.cfg \
            file://user_2025-04-08-19-00-00.cfg \
            "

