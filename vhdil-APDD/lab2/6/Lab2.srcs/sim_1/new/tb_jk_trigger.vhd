library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tb_jk_trigger is
end tb_jk_trigger;

architecture Behavioral of tb_jk_trigger is
    component jk_trigger
        Port ( J : in STD_LOGIC;
               K : in STD_LOGIC;
               C : in STD_LOGIC;
               S : in STD_LOGIC;
               R : in STD_LOGIC;
               Q : out STD_LOGIC;
               Qbar : out STD_LOGIC);
    end component;
    signal J, K, C, S, R, Q, Qbar : STD_LOGIC;
begin
    uut: jk_trigger port map (J => J, K => K, C => C, S => S, R => R, Q => Q, Qbar => Qbar);
    
    process
    begin
        -- Инициализация
        S <= '0'; R <= '0'; J <= '0'; K <= '0'; C <= '0';
        wait for 10 ns;
        
        -- Тест асинхронного сброса R=1
        R <= '1';
        wait for 20 ns;
        R <= '0';
        wait for 20 ns;
        
        -- Тест асинхронной установки S=1
        S <= '1';
        wait for 20 ns;
        S <= '0';
        wait for 20 ns;
        
        -- Генерация тактового сигнала
        for i in 1 to 20 loop
            C <= '0'; wait for 5 ns;
            C <= '1'; wait for 5 ns;
        end loop;
        
        -- Тест режимов JK (00 - hold, 01 - reset, 10 - set, 11 - toggle)
        -- 00 hold
        J <= '0'; K <= '0';
        wait for 40 ns;
        
        -- 01 reset
        J <= '0'; K <= '1';
        wait for 40 ns;
        
        -- 10 set
        J <= '1'; K <= '0';
        wait for 40 ns;
        
        -- 11 toggle
        J <= '1'; K <= '1';
        wait for 80 ns;  -- Несколько тактов для toggle
        
        wait;
    end process;
end Behavioral;