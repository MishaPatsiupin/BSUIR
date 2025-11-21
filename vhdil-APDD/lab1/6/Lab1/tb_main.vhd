library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tb_main is
end tb_main;

architecture Behavioral of tb_main is
    signal x : std_logic_vector(7 downto 0);
    signal el : std_logic;
    signal eo : std_logic;
    signal gs : std_logic;
    signal a : std_logic_vector(2 downto 0);
    
    constant DELAY : time := 20 ns;
    
begin
    uut: entity work.main
    port map (
        x => x,
        el => el,
        eo => eo,
        gs => gs,
        a => a
    );
    
    stim_proc: process
    begin
            -- Test 0: Высший приоритет (вход 7 активен)
        report "Test 0: XXXX";
        x <= "XXXXXXXX";
        el <= 'X';
        wait for DELAY;
    
        -- Test 1: EI = '1' (disabled) - все выходы должны быть неактивны
        report "Test 1: EI = '1' (disabled)";
        x <= "11111111";
        el <= '1';
        wait for DELAY;
        assert eo = '1' and gs = '1' and a = "111" 
            report "Error in disabled mode" severity error;
        
        -- Test 2: EI = '0', все входы = '1' - нет активных входов
        report "Test 2: No active inputs";
        x <= "11111111";
        el <= '0';
        wait for DELAY;
        assert eo = '0' and gs = '1' and a = "111" 
            report "Error: No active inputs" severity error;
        
        -- Test 3: Высший приоритет (вход 7 активен)
        report "Test 3: Highest priority (input 7)";
        x <= "0XXXXXXX";  -- 7-й вход активен (0)
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "000" 
            report "Error: Input 7 encoding" severity error;
        
        -- Test 4: Вход 6 активен (7-й неактивен)
        report "Test 4: Input 6 active";
        x <= "10XXXXXX";  -- 7=1, 6=0
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "001" 
            report "Error: Input 6 encoding" severity error;
        
        -- Test 5: Вход 5 активен
        report "Test 5: Input 5 active";
        x <= "110XXXXX";  -- 7,6=1, 5=0
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "010" 
            report "Error: Input 5 encoding" severity error;
        
        -- Test 6: Вход 4 активен
        report "Test 6: Input 4 active";
        x <= "1110XXXX";  -- 7,6,5=1, 4=0
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "011" 
            report "Error: Input 4 encoding" severity error;
        
        -- Test 7: Вход 3 активен
        report "Test 7: Input 3 active";
        x <= "11110XXX";  -- 7,6,5,4=1, 3=0
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "100" 
            report "Error: Input 3 encoding" severity error;
        
        -- Test 8: Вход 2 активен
        report "Test 8: Input 2 active";
        x <= "111110XX";  -- 7,6,5,4,3=1, 2=0
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "101" 
            report "Error: Input 2 encoding" severity error;
        
        -- Test 9: Вход 1 активен
        report "Test 9: Input 1 active";
        x <= "1111110X";  -- 7,6,5,4,3,2=1, 1=0
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "110" 
            report "Error: Input 1 encoding" severity error;
        
        -- Test 10: Низший приоритет (вход 0 активен)
        report "Test 10: Lowest priority (input 0)";
        x <= "11111110";  -- все кроме 0=1, 0=0
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "111" 
            report "Error: Input 0 encoding" severity error;
        
        -- Test 11: Приоритетная логика (активны несколько входов)
        report "Test 11: Multiple inputs active - priority test";
        x <= "01010101";  -- Активны 7,5,3,1 - должен выбрать 7
        el <= '0';
        wait for DELAY;
        assert eo = '1' and gs = '0' and a = "000" 
            report "Error: Priority test failed" severity error;
        
        report "All tests completed";
        wait;
    end process;
end Behavioral;