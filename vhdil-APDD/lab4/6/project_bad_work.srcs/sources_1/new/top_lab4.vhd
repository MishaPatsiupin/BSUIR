library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Библиотека для дифференциального буфера (нужен для sysclk_p/n)
library UNISIM;
use UNISIM.VComponents.all;

entity top_lab4 is
    Port ( 
        -- Входы с платы (названия как в .ucf/.xdc файле)
        sysclk_p    : in  STD_LOGIC; -- Дифференциальный клок 200 МГц
        sysclk_n    : in  STD_LOGIC;
        pushbuttons : in  STD_LOGIC_VECTOR (4 downto 0); -- Кнопки
        dipswitch   : in  STD_LOGIC_VECTOR (3 downto 0); -- Переключатели
        
        -- Выходы на светодиоды
        ledsmain    : out STD_LOGIC_VECTOR (3 downto 0);
        ledsboard   : out STD_LOGIC_VECTOR (3 downto 0)
    );
end top_lab4;

architecture Behavioral of top_lab4 is

    -- Компонент вашего счетчика
    component als193a
        Port ( clr : in  STD_LOGIC;
               load : in  STD_LOGIC;
               up : in  STD_LOGIC;
               down : in  STD_LOGIC;
               a, b, c, d : in  STD_LOGIC;
               qa, qb, qc, qd : out  STD_LOGIC;
               bo, co : out  STD_LOGIC);
    end component;

    -- Компонент делителя
    component divider
        generic(div_value : integer);
        Port ( CLK_IN : in STD_LOGIC;
               CLK_OUT : out STD_LOGIC);
    end component;

    -- Сигналы
    signal clk_200mhz_int : STD_LOGIC; -- Внутренний клок 200 МГц
    signal slow_clk       : STD_LOGIC; -- Медленный клок (для счета)
    
    -- Сигналы управления счетчиком
    signal signal_up   : STD_LOGIC;
    signal signal_down : STD_LOGIC;
    
    -- Выходы счетчика
    signal qa_out, qb_out, qc_out, qd_out : STD_LOGIC;

begin

    -- 1. Входной буфер для дифференциального тактового сигнала
    IBUFDS_inst : IBUFDS
    generic map (
        DIFF_TERM => FALSE, 
        IBUF_LOW_PWR => TRUE, 
        IOSTANDARD => "DEFAULT")
    port map (
        O => clk_200mhz_int,  -- Выход обычного клока
        I => sysclk_p,        -- Вход P
        IB => sysclk_n        -- Вход N
    );

    -- 2. Делитель частоты (делаем клок видимым глазу)
    div_inst : divider
    generic map (div_value => 20000000) -- Настройка скорости
    port map (
        CLK_IN => clk_200mhz_int,
        CLK_OUT => slow_clk
    );

    -- 3. Логика управления направлением (используем переключатель dipswitch(0))
    -- Если Switch(0) выключен -> Считаем ВВЕРХ
    -- Если Switch(0) включен  -> Считаем ВНИЗ
    -- Счетчик als193a считает вверх, когда на UP идут импульсы, а DOWN='1'.
    -- И наоборот для счета вниз.
    
    process(slow_clk, dipswitch(0))
    begin
        if dipswitch(0) = '0' then
            -- Режим счета ВВЕРХ
            signal_up <= slow_clk;
            signal_down <= '1'; -- Неактивен
        else
            -- Режим счета ВНИЗ
            signal_up <= '1';   -- Неактивен
            signal_down <= slow_clk;
        end if;
    end process;

    -- 4. Инстанцирование счетчика als193a
    counter_inst : als193a
    port map (
        clr  => pushbuttons(0),   -- Центральная кнопка -> Сброс
        load => not pushbuttons(1), -- Кнопка 1 -> Load (в als193 active low, инвертируем кнопку)
        
        up   => signal_up,        -- Импульсы вверх
        down => signal_down,      -- Импульсы вниз
        
        -- Данные для загрузки берем с переключателей 1, 2, 3
        a => '0',                 -- Младший бит жестко 0 или можно dipswitch(1)
        b => dipswitch(1),
        c => dipswitch(2),
        d => dipswitch(3),
        
        -- Выходы на светодиоды
        qa => qa_out,
        qb => qb_out,
        qc => qc_out,
        qd => qd_out,
        
        bo => ledsboard(0), -- Вывод сигнала заема на светодиод платы
        co => ledsboard(1)  -- Вывод сигнала переноса
    );

    -- Подключение выходов к главным светодиодам
    ledsmain(0) <= qa_out;
    ledsmain(1) <= qb_out;
    ledsmain(2) <= qc_out;
    ledsmain(3) <= qd_out;
    
    -- Остальные диоды гасим
    ledsboard(2) <= '0';
    ledsboard(3) <= '0';

end Behavioral;