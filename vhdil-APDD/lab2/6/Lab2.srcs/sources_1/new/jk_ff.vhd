library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity jk_ff is
    Port ( clk : in  STD_LOGIC;
           j : in  STD_LOGIC;
           k : in  STD_LOGIC;
           clr : in  STD_LOGIC;  -- Асинхронный сброс, активный '1'
           load : in  STD_LOGIC; -- Асинхронная загрузка, активный '0'
           data : in  STD_LOGIC; -- Данные для загрузки
           q : out  STD_LOGIC);
end jk_ff;

architecture Behavioral of jk_ff is
    signal q_out : std_logic := '0';
begin
    q <= q_out;
    
    process(clk, clr, load)
    begin
        if clr = '1' then
            q_out <= '0';  -- CLR приоритет
        elsif load = '0' then
            q_out <= data;  -- Async LOAD
        elsif rising_edge(clk) then
            if (j = '0' and k = '0') then
                q_out <= q_out;  -- Hold
            elsif (j = '0' and k = '1') then
                q_out <= '0';    -- Reset
            elsif (j = '1' and k = '0') then
                q_out <= '1';    -- Set
            elsif (j = '1' and k = '1') then
                q_out <= not q_out;  -- Toggle
            end if;
        end if;
    end process;
end Behavioral;