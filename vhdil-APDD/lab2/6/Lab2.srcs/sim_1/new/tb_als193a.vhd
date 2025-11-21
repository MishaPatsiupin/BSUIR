library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_als193a is
    -- Testbench без портов
end tb_als193a;

architecture Behavioral of tb_als193a is
    component als193a
        Port ( clr : in  STD_LOGIC;
               load : in  STD_LOGIC;
               up : in  STD_LOGIC;
               down : in  STD_LOGIC;
               a : in  STD_LOGIC;
               b : in  STD_LOGIC;
               c : in  STD_LOGIC;
               d : in  STD_LOGIC;
               qa : out  STD_LOGIC;
               qb : out  STD_LOGIC;
               qc : out  STD_LOGIC;
               qd : out  STD_LOGIC;
               bo : out  STD_LOGIC;
               co : out  STD_LOGIC);
    end component;
    
    signal clr_tb : STD_LOGIC := '0';
    signal load_tb : STD_LOGIC := '1';
    signal up_tb : STD_LOGIC := '0';
    signal down_tb : STD_LOGIC := '1';  -- Для up
    signal a_tb, b_tb, c_tb, d_tb : STD_LOGIC := '0';
    signal qa_tb, qb_tb, qc_tb, qd_tb : STD_LOGIC;
    signal bo_tb, co_tb : STD_LOGIC;
    
    constant PULSE_WIDTH : time := 25 ns;
    constant PERIOD : time := 50 ns;
    
begin
    uut: als193a 
        port map (clr => clr_tb, load => load_tb, up => up_tb, down => down_tb,
                  a => a_tb, b => b_tb, c => c_tb, d => d_tb,
                  qa => qa_tb, qb => qb_tb, qc => qc_tb, qd => qd_tb,
                  bo => bo_tb, co => co_tb);
    
    stim_proc: process
    begin
        -- 1. CLR='1' (0-50 нс)
        clr_tb <= '1'; wait for PULSE_WIDTH; clr_tb <= '0'; wait for PERIOD - PULSE_WIDTH;
        
        -- 2. LOAD='0' с 1101 (100-150 нс)
        a_tb <= '1'; b_tb <= '0'; c_tb <= '1'; d_tb <= '1';
        wait for 25 ns; load_tb <= '0'; wait for PULSE_WIDTH; load_tb <= '1';
        wait for PERIOD;
        
        -- 3. 5 UP (down='1', 250-475 нс)
        down_tb <= '1';
        for i in 1 to 5 loop
            up_tb <= '0'; wait for PULSE_WIDTH;
            up_tb <= '1'; wait for PULSE_WIDTH;
        end loop;
        up_tb <= '0'; wait for 25 ns;
        
        -- Переход (500 нс)
        up_tb <= '1'; down_tb <= '0'; wait for PERIOD;
        
        -- 4. 5 DOWN (up='1', 550-775 нс)
        for i in 1 to 5 loop
            down_tb <= '0'; wait for PULSE_WIDTH;
            down_tb <= '1'; wait for PULSE_WIDTH;
        end loop;
        down_tb <= '0'; wait for PERIOD * 3;
        
        wait;
    end process;
    
    -- Мониторинг
    monitor_proc: process
    begin
        wait for 1 ns;
        report "Time: " & time'image(now) & " | CLR: " & std_logic'image(clr_tb) &
               " | LOAD: " & std_logic'image(load_tb) & " | UP: " & std_logic'image(up_tb) &
               " | DOWN: " & std_logic'image(down_tb) & " | Data(ABCD): " & 
               std_logic'image(a_tb) & std_logic'image(b_tb) & std_logic'image(c_tb) & std_logic'image(d_tb) &
               " | Q(A B C D): " & std_logic'image(qa_tb) & std_logic'image(qb_tb) &
               std_logic'image(qc_tb) & std_logic'image(qd_tb) & " | CO: " & std_logic'image(co_tb) &
               " | BO: " & std_logic'image(bo_tb);
        wait for 25 ns;
    end process;

end Behavioral;