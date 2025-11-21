library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
entity debounce is port(clk, btn: in std_logic; btn_out: out std_logic); end;
architecture arc of debounce is
    signal cnt : integer range 0 to 1_999_999 := 0;
    signal q   : std_logic := '0';
begin
    process(clk)
    begin
        if rising_edge(clk) then
            if btn /= q then cnt <= 0; q <= btn;
            elsif cnt /= 1_999_999 then cnt <= cnt + 1;
            else btn_out <= q; end if;
        end if;
    end process;
end;