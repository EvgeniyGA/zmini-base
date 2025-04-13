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
set_property -dict {PACKAGE_PIN U15 IOSTANDARD LVCMOS33} [get_ports {HDMI_HPD_tri_i[0]}]
set_property -dict {PACKAGE_PIN H18 IOSTANDARD LVCMOS33} [get_ports HDMI_EN]

#I2C0
set_property -dict {PACKAGE_PIN N20 IOSTANDARD LVCMOS33} [get_ports HDMI_DDC_scl_io]
set_property -dict {PACKAGE_PIN P20 IOSTANDARD LVCMOS33} [get_ports HDMI_DDC_sda_io]

#I2C1
set_property -dict {PACKAGE_PIN N20 IOSTANDARD LVCMOS33} [get_ports IIC_1_0_scl_io]
set_property -dict {PACKAGE_PIN P20 IOSTANDARD LVCMOS33} [get_ports IIC_1_0_sda_io]

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
