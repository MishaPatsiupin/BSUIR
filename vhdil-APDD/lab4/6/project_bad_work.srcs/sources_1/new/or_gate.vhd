library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity or_gate is
    Port ( x : in  STD_LOGIC;
           y : in  STD_LOGIC;
           z : out  STD_LOGIC);
end or_gate;

architecture Behavioral of or_gate is
begin
    z <= x or y;
end Behavioral;