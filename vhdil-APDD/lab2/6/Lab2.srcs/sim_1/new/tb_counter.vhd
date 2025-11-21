library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tb_counter is
end tb_counter;

architecture Behavioral of tb_counter is
    component counter
        Port ( UP : in STD_LOGIC;
               DOWN : in STD_LOGIC;
               LOAD : in STD_LOGIC;
               CLR : in STD_LOGIC;
               D : in STD_LOGIC_VECTOR (3 downto 0);
               Q : out STD_LOGIC_VECTOR (3 downto 0);
               CO : out STD_LOGIC;
               BO : out STD_LOGIC);
    end component;
    signal UP, DOWN, LOAD, CLR : STD_LOGIC := '0';
    signal D : STD_LOGIC_VECTOR (3 downto 0) := "1010";
    signal Q : STD_LOGIC_VECTOR (3 downto 0);
    signal CO, BO : STD_LOGIC;
begin
    uut: counter port map (UP => UP, DOWN => DOWN, LOAD => LOAD, CLR => CLR, D => D, Q => Q, CO => CO, BO => BO);
    
    process
    begin
        -- Async reset
        CLR <= '1'; wait for 20 ns; CLR <= '0'; wait for 10 ns;
        
        -- Synchronous load (LOAD='0', pulse any clock, e.g. UP)
        LOAD <= '0'; DOWN <= '1';  -- Hold DOWN high (for compatibility, though load doesn't depend)
        UP <= '1'; wait for 10 ns; UP <= '0'; wait for 20 ns;
        LOAD <= '1';
        assert Q = "1010" report "Load to 1010 failed" severity error;
        
        -- Count up: hold DOWN='1', pulse UP 5 times (to 1111)
        for i in 1 to 5 loop
            UP <= '1'; wait for 10 ns; UP <= '0'; wait for 10 ns;
        end loop;
        assert Q = "1111" report "Up count to 1111 failed" severity error;
        UP <= '0'; assert CO = '0' report "CO low on overflow failed" severity error;  -- UP=0, Q=1111
        
        -- Count down: hold UP='1', pulse DOWN 15 times (from 1111 to 0000)
        UP <= '1';  -- Hold UP high
        for i in 1 to 15 loop
            DOWN <= '1'; wait for 10 ns; DOWN <= '0'; wait for 10 ns;
        end loop;
        assert Q = "0000" report "Down count to 0000 failed" severity error;
        DOWN <= '0'; assert BO = '0' report "BO low on underflow failed" severity error;  -- DOWN=0, Q=0000
        
        -- One more down pulse: should wrap to 1111, BO pulse
        DOWN <= '1'; wait for 10 ns; DOWN <= '0'; wait for 10 ns;
        assert Q = "1111" report "Wrap down failed" severity error;
        
        wait;
    end process;
end Behavioral;