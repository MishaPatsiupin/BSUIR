library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity divider is


    generic(div_value : integer := 20000000);
    Port ( 
        CLK_IN : in STD_LOGIC;
        CLK_OUT : out STD_LOGIC
    );
end divider;

architecture Behavioral of divider is
    signal clk_counter_value : std_logic_vector (31 downto 0) := (others => '0');
    signal curr_clk_out : std_logic := '0'; 
begin
    process(clk_in)
    begin
        if (rising_edge(clk_in)) then
            if (clk_counter_value = conv_std_logic_vector(div_value, 32)) then
                clk_counter_value <= (others => '0');
                curr_clk_out <= not curr_clk_out;
            else
                clk_counter_value <= clk_counter_value + 1;
            end if;
        end if;
    end process;
    clk_out <= curr_clk_out;   
end Behavioral;