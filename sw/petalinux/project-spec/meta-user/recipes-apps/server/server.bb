#
# This file is the server recipe.
#

SUMMARY = "Simple web-server application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://backon.sh \
	file://webserver/ \
		  "
S = "${WORKDIR}"

RDEPENDS:${PN} += "bash"

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/backon.sh ${D}${bindir}/backon.sh
	cp -r ${WORKDIR}/webserver ${D}${bindir}
}

#FILES_${PN}= "/home/petalinux/"
