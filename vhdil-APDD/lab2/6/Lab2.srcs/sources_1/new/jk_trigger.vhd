library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity jk_trigger is
    Port ( J : in STD_LOGIC;
           K : in STD_LOGIC;
           C : in STD_LOGIC;
           S : in STD_LOGIC;
           R : in STD_LOGIC;
           Q : out STD_LOGIC;
           Qbar : out STD_LOGIC);
end jk_trigger;

architecture Behavioral of jk_trigger is
    signal q_int : STD_LOGIC := '0';
begin
    process (C, S, R)
    begin
        if S = '1' then
            q_int <= '1';
        elsif R = '1' then
            q_int <= '0';
        elsif rising_edge(C) then
            if J = '1' and K = '0' then
                q_int <= '1';
            elsif J = '0' and K = '1' then
                q_int <= '0';
            elsif J = '1' and K = '1' then
                q_int <= not q_int;
            end if;
        end if;
    end process;
    Q <= q_int;
    Qbar <= not q_int;
end Behavioral;