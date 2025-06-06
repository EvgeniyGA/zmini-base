include Makefile.inc

BUILD := $(shell pwd)/build
VIVADO := source /tools/Xilinx/Vivado/$(VERSION)/settings64.sh

.PHONY: all
all: fpga vitis petalinux
#all: git-init fpga u-boot vitis petalinux

.PHONY: petalinux
petalinux:
	$(MAKE) -C sw/petalinux

.PHONY: u-boot
u-boot:
	$(VIVADO) && $(MAKE) -C sw/u-boot-xlnx xilinx_zynq_virt_defconfig
	$(VIVADO) && $(MAKE) -C sw/u-boot-xlnx -j

.PHONY: vitis
vitis:
	$(MAKE) -C sw/vitis/hello_world
#	$(MAKE) -C sw/vitis/free_rtos

.PHONY: fpga
fpga:
	$(MAKE) -C hw

.PHONY: clean
clean: clean-hw clean-sw
	rm -rf $(BUILD)
	mkdir -p $(BUILD)

.PHONY: clean-hw
clean-hw:
	$(MAKE) -C hw clean

.PHONY: clean-sw
clean-sw:
	$(MAKE) -C sw/vitis/hello_world clean
#	$(MAKE) -C sw/vitis/free_rtos clean
	$(MAKE) -C sw/petalinux clean
#	$(VIVADO) && $(MAKE) -C sw/u-boot-xlnx distclean
	rm -rf $(BUILD)/vitis/*
	rm -rf $(BUILD)/petalinux/*

.PHONY: git-init
git-init:
	git submodule update --init --recursive
	
.PHONY: install_board_files
install_board_files: 
	sudo cp -r hw/source/misc/board_files /tools/Xilinx/Vivado/$(VERSION)/data/boards

.PHONY: update-bd
update-bd:
	$(MAKE) -C hw update-bd

# todo
# write_bd_tcl [get_property DIRECTORY [current_project]]/../source/scripts/bd.tcl -include_layout -force

