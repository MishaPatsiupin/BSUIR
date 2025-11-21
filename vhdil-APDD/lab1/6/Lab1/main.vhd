----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 06.09.2025 09:07:51
-- Design Name: 
-- Module Name: main - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity main is
port (
    x : in std_logic_vector (7 downto 0);
    el : in std_logic;
    eo : out std_logic;
    gs : out std_logic;
    a : out std_logic_vector (2 downto 0)
);
end main;

architecture Behavioral of main is
    signal eo_inside : std_logic;

    signal el_not : std_logic;
    signal x_not : std_logic_vector (7 downto 0);
    
    signal a0_1, a0_2, a0_3, a0_4 : std_logic;
    signal a1_1, a1_2, a1_3, a1_4 : std_logic;
    signal a2_1, a2_2, a2_3, a2_4 : std_logic;
    
    signal a0_int, a1_int, a2_int : std_logic;
    signal a_active : std_logic_vector(2 downto 0);
          
begin
    
    eo_inside <= not (x(0) and x(1) and x(2) and x(3) and x(4) and x(5) and x(6) and x(7) and (not el));
    --eo <= eo_inside;
    
    el_not <= not el;
    x_not <= not x;
    
    --gs <= (not eo_inside) or (not el_not);

    a0_1 <= x_not(1) and (not x_not(2)) and (not x_not(4)) and (not x_not(6)) and el_not;
    a0_2 <= x_not(3) and (not x_not(4)) and (not x_not(6)) and el_not;
    a0_3 <= x_not(5) and (not x_not(6)) and el_not;
    a0_4 <= x_not(7) and el_not;
    a0_int <= not (a0_1 or a0_2 or a0_3 or a0_4);

    a1_1 <= x_not(2) and (not x_not(4)) and (not x_not(5)) and el_not; 
    a1_2 <= x_not(3) and (not x_not(4)) and (not x_not(5)) and el_not;
    a1_3 <= x_not(6) and el_not;
    a1_4 <= x_not(7) and el_not;
    a1_int <= not (a1_1 or a1_2 or a1_3 or a1_4);
    
    a2_1 <= x_not(4) and el_not;
    a2_2 <= x_not(5) and el_not;
    a2_3 <= x_not(6) and el_not;
    a2_4 <= x_not(7) and el_not;
    a2_int <= not (a2_1 or a2_2 or a2_3 or a2_4);
    
    a_active <= a2_int & a1_int & a0_int;
    
    eo <= '1' when el = '1' else eo_inside;
    gs <= '1' when el = '1' else ((not eo_inside) or (not el_not));
    a <= "111" when el = '1' else a_active;


end Behavioral;
