# AC701 Rev 2.0 - ALS193A - Ã»Õ»Ã”Ã
set_property PACKAGE_PIN R5  [get_ports down]      ;# PB1
set_property PACKAGE_PIN U5  [get_ports up]        ;# PB3
set_property IOSTANDARD SSTL15 [get_ports {up down}]

set_property PACKAGE_PIN R8  [get_ports a]
set_property PACKAGE_PIN P8  [get_ports b]
set_property PACKAGE_PIN R7  [get_ports c]
set_property PACKAGE_PIN R6  [get_ports d]
set_property IOSTANDARD SSTL15 [get_ports {a b c d}]

set_property PACKAGE_PIN P6  [get_ports clr]       ;# PB0
set_property PACKAGE_PIN T5  [get_ports load]      ;# PB4
set_property IOSTANDARD SSTL15 [get_ports {clr load}]

set_property PACKAGE_PIN F25 [get_ports qa]
set_property PACKAGE_PIN G25 [get_ports qb]
set_property PACKAGE_PIN G26 [get_ports qc]
set_property PACKAGE_PIN H26 [get_ports qd]
set_property IOSTANDARD LVCMOS25 [get_ports {qa qb qc qd}]

set_property PACKAGE_PIN R3 [get_ports clk]
set_property IOSTANDARD LVDS_25 [get_ports clk]

set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]