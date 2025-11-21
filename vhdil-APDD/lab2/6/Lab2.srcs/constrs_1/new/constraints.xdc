# Clock definition 
create_clock -name sysclk -period 20.000 [get_ports sysclk_p]  -- 50 MHz вместо 200 MHz

# False paths для асинхронных входов
set_false_path -from [get_ports {IN_buttons[*]}] -to [all_registers]
set_false_path -from [get_ports {IN_switches[*]}] -to [all_registers]

# Input delays
set_input_delay -clock [get_clocks sysclk] -max 3.000 [get_ports {IN_buttons[*]}]
set_input_delay -clock [get_clocks sysclk] -min 1.000 [get_ports {IN_buttons[*]}]
set_input_delay -clock [get_clocks sysclk] -max 3.000 [get_ports {IN_switches[*]}]
set_input_delay -clock [get_clocks sysclk] -min 1.000 [get_ports {IN_switches[*]}]

# Output delays
set_output_delay -clock [get_clocks sysclk] -max 3.000 [get_ports {LED_out[*]}]
set_output_delay -clock [get_clocks sysclk] -min 1.000 [get_ports {LED_out[*]}]