# Zynq-MINI-20 Base Project
------
Contents: 
Base Vivado and Petalinux project for demonstration:
- [X] HDMI Output
- [X] LCD Display (SPI)
    - for test:
        - cat /dev/urandom > /dev/fb1
- [ ] DPU for vitis Ai

## Hardware Requirements:
- Zynq Mini-20 Board

## Software Requirements
- Ubuntu 24.02 host system for compilation
- Vivado/Vitis 2024.2
- Petalinux 2024.2

## Compile BSP project:
------
Compile everything: 
```bash
make
```

Create and compile Vivado project: 
```bash
make fpga
```

Create and compile petalinux project
```bash
make petalinux
```

BSP directory structure: 
------
```bash
├── build                       <--- Build Outputs (FPGA and Boot binaries)
├── hw                          <--- All source files related to Vivado Design 
│   ├── build                   <--- Vivado Project  
│   ├── scripts                 <--- TCL scripts to recreate project in batch mode
│   │   └── settings.tcl        <--- Set FPGA type, project name, and number of processors for compilation 
│   └── source
│       ├── constraints         <--- Constraints location. Files will be imported during creation
│       ├── hdl                 <--- Put HDL IP blocks (non block design) here
│       ├── ip                  <--- Put IP blocks (used by ip integrator) here
│       └── scripts
│           └── bd.tcl          <--- TCL script to recreate the block design.
└── sw
    ├── petalinux               <--- Petalinux project 
    ├── vitis                   <--- Project folder containing bare metal application 
    │   ├── build               <--- Boot image is located here (BOOT.bin)
    │   ├── scripts             <--- TCL scripts for batch mode
    │   ├── src                 <--- Bare metal source code files
    │   └── ws                  <--- Vitis workspace
    └── xsa                     <--- Hardware description file, exported by vivado
```
