##TDMS Clock
set_property -dict {PACKAGE_PIN H17 IOSTANDARD TMDS_33} [get_ports TMDS_clk_n]
set_property -dict {PACKAGE_PIN H16 IOSTANDARD TMDS_33} [get_ports TMDS_clk_p]

#TDMS Data (qty. 3)
set_property -dict {PACKAGE_PIN D20 IOSTANDARD TMDS_33} [get_ports {TMDS_data_n[0]}]
set_property -dict {PACKAGE_PIN D19 IOSTANDARD TMDS_33} [get_ports {TMDS_data_p[0]}]
set_property -dict {PACKAGE_PIN B20 IOSTANDARD TMDS_33} [get_ports {TMDS_data_n[1]}]
set_property -dict {PACKAGE_PIN C20 IOSTANDARD TMDS_33} [get_ports {TMDS_data_p[1]}]
set_property -dict {PACKAGE_PIN A20 IOSTANDARD TMDS_33} [get_ports {TMDS_data_n[2]}]
set_property -dict {PACKAGE_PIN B19 IOSTANDARD TMDS_33} [get_ports {TMDS_data_p[2]}]

#HPD
set_property -dict {PACKAGE_PIN V17 IOSTANDARD LVCMOS33} [get_ports {HDMI_HPD_tri_i[0]}]
set_property -dict {PACKAGE_PIN H18 IOSTANDARD LVCMOS33} [get_ports HDMI_EN]

#I2C0
set_property -dict {PACKAGE_PIN N20 IOSTANDARD LVCMOS33} [get_ports HDMI_DDC_scl_io]
set_property -dict {PACKAGE_PIN P20 IOSTANDARD LVCMOS33} [get_ports HDMI_DDC_sda_io]

#I2C1
set_property -dict {PACKAGE_PIN N18 IOSTANDARD LVCMOS33} [get_ports IIC_1_0_scl_io]
set_property -dict {PACKAGE_PIN T20 IOSTANDARD LVCMOS33} [get_ports IIC_1_0_sda_io]

### SPI_0 ##################################################
#
set_property -dict { PACKAGE_PIN W18   IOSTANDARD LVCMOS33 PULLUP false }    [get_ports { SPI_0_0_sck_io }];     # SPI Clock           W18
set_property -dict { PACKAGE_PIN V18   IOSTANDARD LVCMOS33 PULLUP false }    [get_ports { SPI_0_0_io0_io }];     # MOSI                V18
set_property -dict { PACKAGE_PIN U14   IOSTANDARD LVCMOS33 PULLUP false }    [get_ports { SPI_0_0_io1_io }];     # MISO                U19
set_property -dict { PACKAGE_PIN V15   IOSTANDARD LVCMOS33 PULLUP false }    [get_ports { SPI_0_0_ss2_o }];      # CS2                 V15
set_property -dict { PACKAGE_PIN P15   IOSTANDARD LVCMOS33 PULLUP false }    [get_ports { SPI_0_0_ss1_o }];      # CS1                 P15
set_property -dict { PACKAGE_PIN U15   IOSTANDARD LVCMOS33 PULLUP false }    [get_ports { SPI_0_0_ss_io }];      # CS0                 U15

### LCD GPIO
set_property -dict {PACKAGE_PIN U19 IOSTANDARD LVCMOS33} [get_ports {LCD_LED_tri_o[0]}]
set_property -dict {PACKAGE_PIN W15 IOSTANDARD LVCMOS33} [get_ports {LCD_RESET_tri_o[0]}]
set_property -dict {PACKAGE_PIN U17 IOSTANDARD LVCMOS33} [get_ports {LCD_DC_tri_o[0]}]

#Onboard PL LED
set_property -dict {PACKAGE_PIN W13 IOSTANDARD LVCMOS33} [get_ports {led[0]}]
set_property -dict {PACKAGE_PIN V12 IOSTANDARD LVCMOS33} [get_ports {led[1]}]
set_property -dict {PACKAGE_PIN U12 IOSTANDARD LVCMOS33} [get_ports {led[2]}]
set_property -dict {PACKAGE_PIN T12 IOSTANDARD LVCMOS33} [get_ports {led[3]}]

#Onboard PL Button
set_property -dict {PACKAGE_PIN M20 IOSTANDARD LVCMOS33} [get_ports {btn[0]}]
set_property -dict {PACKAGE_PIN M19 IOSTANDARD LVCMOS33} [get_ports {btn[1]}]

#Onboard PS LED
#set_property -dict {PACKAGE_PIN G6 IOSTANDARD LVCMOS33} [get_ports {PS_LED_tri_o[0]}]
