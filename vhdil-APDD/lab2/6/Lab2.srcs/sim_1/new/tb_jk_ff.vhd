library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tb_jk_ff is
end tb_jk_ff;

architecture behavior of tb_jk_ff is

    -- Подключение к триггеру JK
    component jk_ff
        Port ( clk : in  STD_LOGIC;
               j : in  STD_LOGIC;
               k : in  STD_LOGIC;
               clr : in  STD_LOGIC;  -- Асинхронный сброс, активный '1'
               load : in  STD_LOGIC; -- Асинхронная загрузка, активный '0'
               data : in  STD_LOGIC; -- Данные для загрузки
               q : out  STD_LOGIC);
    end component;

    -- Сигналы для тестирования
    signal clk : STD_LOGIC := '0';
    signal j : STD_LOGIC := '0';
    signal k : STD_LOGIC := '0';
    signal clr : STD_LOGIC := '0';
    signal load : STD_LOGIC := '1';
    signal data : STD_LOGIC := '0';
    signal q : STD_LOGIC;

    -- Период тактового сигнала
    constant clk_period : time := 10 ns;

begin

    -- Инстанцирование триггера JK
    uut: jk_ff
        Port map (
            clk => clk,
            j => j,
            k => k,
            clr => clr,
            load => load,
            data => data,
            q => q
        );

    -- Генерация тактового сигнала
    clk_process :process
    begin
        clk <= '0';
        wait for clk_period / 2;
        clk <= '1';
        wait for clk_period / 2;
    end process;

    -- Тестовый процесс
    stim_process: process
    begin
        -- Сброс
        clr <= '1';
        wait for clk_period;
        clr <= '0';
        wait for clk_period;

        -- Тестировать режим загрузки
        load <= '0'; 
        data <= '1';
        wait for clk_period;
        load <= '1'; 
        wait for clk_period;

        -- Тестировать различные комбинации j и k
        j <= '0'; k <= '0'; wait for clk_period;  -- Hold
        j <= '0'; k <= '1'; wait for clk_period;  -- Reset
        j <= '1'; k <= '0'; wait for clk_period;  -- Set
        j <= '1'; k <= '1'; wait for clk_period;  -- Toggle
        
        -- Завершение симуляции
        wait;
    end process;

end behavior;