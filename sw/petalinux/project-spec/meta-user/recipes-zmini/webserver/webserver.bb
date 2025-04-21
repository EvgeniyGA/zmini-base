SUMMARY = "MiniZed Gameboard"
DESCRIPTION = "Files for MiniZed webserver-enabled webserver"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

#FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://gpio_driver.py \
            file://gpio_setup.py \
            file://webserver.tar.gz;unpack=0 \
"

do_install() {
    install -d ${D}/home/root/
    cp -r ${WORKDIR}/gpio_driver.py ${D}/home/root/
    cp -r ${WORKDIR}/gpio_setup.py ${D}/home/root/
    install -m 0755 ${WORKDIR}/webserver.tar.gz ${D}/home/root/
}

FILES_${PN} = " \
    /home/root/gpio_driver.py \
    /home/root/gpio_setup.py \
    /home/root/webserver.tar.gz \
"
