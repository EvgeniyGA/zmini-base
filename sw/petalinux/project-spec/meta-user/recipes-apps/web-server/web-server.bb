#
# This file is the web-server recipe.
#

SUMMARY = "Simple web-server application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://web-server.sh \
		  "
S = "${WORKDIR}"

RDEPENDS:${PN} += "bash"

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 ${WORKDIR}/web-server.sh ${D}${bindir}
}
