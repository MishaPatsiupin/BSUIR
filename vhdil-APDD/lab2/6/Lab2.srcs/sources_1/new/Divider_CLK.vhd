library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity Divider_CLK is
    Port ( CLK_IN : in STD_LOGIC;
           CLK_OUT : out STD_LOGIC);
end Divider_CLK;

architecture Behavioral of Divider_CLK is
    signal temp_CLK : std_logic := '0';
    signal counter : unsigned(15 downto 0) := (others => '0');  -- 16-?????? ???????
    
    constant DIVIDE_VALUE : integer := 1000;  -- 200 MHz / 2000 = 100 kHz
    
begin
    process (CLK_IN)
    begin
        if rising_edge(CLK_IN) then
            if counter = DIVIDE_VALUE - 1 then
                counter <= (others => '0');
                temp_CLK <= not temp_CLK;
            else 
                counter <= counter + 1;
            end if;
        end if;
    end process;
    
    CLK_OUT <= temp_CLK;       
end Behavioral;