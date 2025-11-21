library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity als193a is
    Port ( clr : in  STD_LOGIC;
           load : in  STD_LOGIC;  -- активный '0'
           up : in  STD_LOGIC;
           down : in  STD_LOGIC;
           a : in  STD_LOGIC;  -- LSB data
           b : in  STD_LOGIC;
           c : in  STD_LOGIC;
           d : in  STD_LOGIC;  -- MSB data
           qa : out  STD_LOGIC;  -- LSB out
           qb : out  STD_LOGIC;
           qc : out  STD_LOGIC;
           qd : out  STD_LOGIC;  -- MSB out
           bo : out  STD_LOGIC;  -- borrow out, активный '0'
           co : out  STD_LOGIC); -- carry out, активный '0'
end als193a;

architecture Structural of als193a is
    component jk_ff
        Port ( clk : in  STD_LOGIC;
               j : in  STD_LOGIC;
               k : in  STD_LOGIC;
               clr : in  STD_LOGIC;
               q : out  STD_LOGIC);
    end component;
    
    component or_gate
        Port ( x : in  STD_LOGIC;
               y : in  STD_LOGIC;
               z : out  STD_LOGIC);
    end component;
    
    signal clk_int : STD_LOGIC;
    signal qa_int, qb_int, qc_int, qd_int : STD_LOGIC;
    signal j0, k0, j1, k1, j2, k2, j3, k3 : STD_LOGIC;
    signal up_c0, up_c1, up_c2, up_c3 : STD_LOGIC;
    signal down_c0, down_c1, down_c2, down_c3 : STD_LOGIC;
    signal cond0, cond1, cond2, cond3 : STD_LOGIC;
    
begin
    -- OR для CLK: UP or DOWN
    u_or: or_gate port map (up, down, clk_int);
    
    -- up_cond (toggle логика для вверх)
    up_c0 <= '1';
    up_c1 <= qa_int;
    up_c2 <= qa_int and qb_int;
    up_c3 <= qa_int and qb_int and qc_int;
    
    -- down_cond (toggle логика для вниз)
    down_c0 <= '1';
    down_c1 <= not qa_int;
    down_c2 <= not qa_int and not qb_int;
    down_c3 <= not qa_int and not qb_int and not qc_int;
    
    -- Выбор cond по режиму (до фронта такта)
    cond0 <= up_c0 when (down = '1' and up = '0') else 
             down_c0 when (up = '1' and down = '0') else '0';
    cond1 <= up_c1 when (down = '1' and up = '0') else 
             down_c1 when (up = '1' and down = '0') else '0';
    cond2 <= up_c2 when (down = '1' and up = '0') else 
             down_c2 when (up = '1' and down = '0') else '0';
    cond3 <= up_c3 when (down = '1' and up = '0') else 
             down_c3 when (up = '1' and down = '0') else '0';
    
    -- J/K с mux для LOAD ('0' - загрузка)
    j0 <= a when load = '0' else cond0;
    k0 <= not a when load = '0' else cond0;
    j1 <= b when load = '0' else cond1;
    k1 <= not b when load = '0' else cond1;
    j2 <= c when load = '0' else cond2;
    k2 <= not c when load = '0' else cond2;
    j3 <= d when load = '0' else cond3;
    k3 <= not d when load = '0' else cond3;
    
    -- Триггеры
    ff0: jk_ff port map (clk_int, j0, k0, clr, qa_int);  -- LSB
    ff1: jk_ff port map (clk_int, j1, k1, clr, qb_int);
    ff2: jk_ff port map (clk_int, j2, k2, clr, qc_int);
    ff3: jk_ff port map (clk_int, j3, k3, clr, qd_int);  -- MSB
    
    qa <= qa_int;
    qb <= qb_int;
    qc <= qc_int;
    qd <= qd_int;
    
    -- CO: '0' если все Q='1' и UP='0'
    co <= not ((qa_int and qb_int and qc_int and qd_int) and (not up));
    
    -- BO: '0' если все Q='0' и DOWN='0'
    bo <= not (((not qa_int) and (not qb_int) and (not qc_int) and (not qd_int)) and (not down));
end Structural;