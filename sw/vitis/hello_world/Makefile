include ../../../Makefile.inc

APP_NAME := hello_world
SHELL := /bin/bash
VIVADO := source /tools/Xilinx/Vivado/$(VERSION)/settings64.sh
XSCT := xsct
WS_DIR := $(shell pwd)/ws
SRC_DIR := $(shell pwd)/src
TMP_IMG_DIR := $(WS_DIR)/tmp
XSA_DIR := $(WS_DIR)/../../../xsa
SCRIPT_DIR := $(shell pwd)/scripts

# Name of the hardware platform that gets imported from xsa file
HW_PLATFORM_NAME := bd_wrapper

.PHONY: all
all: clean ws bootimg

.PHONY: clean
clean: 
	rm -rf ws *.str *.log .Xil build Packages

.PHONY: ws
ws:
#	mkdir ws build
	mkdir build
#	cp -r $(XSA_DIR) ws
#cd ws && 
	$(VIVADO) &&  bash -c "vitis -s $(SCRIPT_DIR)/do_ws.py"
	
#$(XSCT) ./../scripts/create_ws.tcl $(HW_PLATFORM_NAME) $(SRC_DIR)

.PHONY: bootimg
bootimg:
	rm -rf ws/tmp
	cd ws && mkdir tmp
	$(VIVADO) && bash -c "sh scripts/create_bif.sh $(WS_DIR) $(HW_PLATFORM_NAME) $(TMP_IMG_DIR) $(APP_NAME)"
#$(VIVADO) && bash -c "sh scripts/create_bif_uboot.sh $(WS_DIR) $(HW_PLATFORM_NAME) $(TMP_IMG_DIR)"
	@cp $(TMP_IMG_DIR)/*.bin $(TMP_IMG_DIR)/*.bif ./build
	@rm -rf ws/tmp
	@echo ""
	@echo "INFO: Bootimage was copied to $(shell pwd)/build/BOOT.bin"