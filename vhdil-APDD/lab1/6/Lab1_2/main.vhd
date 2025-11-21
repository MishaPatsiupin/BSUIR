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
begin
    process
        variable eo_inside : std_logic;
        variable el_not : std_logic;
        variable x_not : std_logic_vector (7 downto 0);
        
        variable a0_1, a0_2, a0_3, a0_4 : std_logic;
        variable a1_1, a1_2, a1_3, a1_4 : std_logic;
        variable a2_1, a2_2, a2_3, a2_4 : std_logic;
        
        variable a0_int, a1_int, a2_int : std_logic;
    begin
        -- ѕоследовательные операторы вычислени€
        el_not := not el;
        x_not := not x;
        
        eo_inside := not (x(0) and x(1) and x(2) and x(3) and x(4) and x(5) and x(6) and x(7) and (not el));

        a0_1 := x_not(1) and (not x_not(2)) and (not x_not(4)) and (not x_not(6)) and el_not;
        a0_2 := x_not(3) and (not x_not(4)) and (not x_not(6)) and el_not;
        a0_3 := x_not(5) and (not x_not(6)) and el_not;
        a0_4 := x_not(7) and el_not;
        a0_int := not (a0_1 or a0_2 or a0_3 or a0_4);

        a1_1 := x_not(2) and (not x_not(4)) and (not x_not(5)) and el_not; 
        a1_2 := x_not(3) and (not x_not(4)) and (not x_not(5)) and el_not;
        a1_3 := x_not(6) and el_not;
        a1_4 := x_not(7) and el_not;
        a1_int := not (a1_1 or a1_2 or a1_3 or a1_4);
        
        a2_1 := x_not(4) and el_not;
        a2_2 := x_not(5) and el_not;
        a2_3 := x_not(6) and el_not;
        a2_4 := x_not(7) and el_not;
        a2_int := not (a2_1 or a2_2 or a2_3 or a2_4);
        
        -- ѕоследовательные операторы управлени€ выходами
        if el = '1' then
            eo <= '1';
            gs <= '1';
            a <= "111";
        else
            eo <= eo_inside;
            gs <= (not eo_inside) or (not el_not);
            a(0) <= a0_int;
            a(1) <= a1_int;
            a(2) <= a2_int;
        end if;
    end process;
end Behavioral;