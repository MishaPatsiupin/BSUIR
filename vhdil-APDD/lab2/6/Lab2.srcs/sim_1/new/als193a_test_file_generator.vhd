library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use STD.TEXTIO.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;

entity als193a_test_file_generator is
    generic ( CLK_PERIOD : time := 50 ns );
end als193a_test_file_generator;

architecture Behavioral of als193a_test_file_generator is

    signal clr, load, up, down, a, b, c, d : std_logic := '0';
    signal qa, qb, qc, qd, bo, co          : std_logic;

    component als193a
        port ( clr, load, up, down, a, b, c, d : in  std_logic;
               qa, qb, qc, qd, bo, co        : out std_logic );
    end component;

    -- std_logic ? boolean (для записи в файл)
    function to_boolean(s : std_logic) return boolean is
    begin
        return s = '1';
    end function;

begin
    UUT : als193a port map ( clr, load, up, down, a, b, c, d,
                             qa, qb, qc, qd, bo, co );

    --------------------------------------------------------------------
    -- 256 комбинаций ? файл D:\vhdl\Lab2\Lab2.sim\sim_2\behav\xsim
    --------------------------------------------------------------------
    stim : process
        file   f      : text;
        variable L    : line;
        variable vec  : std_logic_vector(7 downto 0);
    begin
        file_open(f, "als193a_input_values.txt", write_mode);

        -- заголовок
        write(L, string'("clr load up down a b c d qa qb qc qd bo co"));
        writeline(f, L);

        for i in 0 to 255 loop
            vec := std_logic_vector(to_unsigned(i, 8));

            clr  <= vec(7);
            load <= vec(6);
            up   <= vec(5);
            down <= vec(4);
            a    <= vec(3);
            b    <= vec(2);
            c    <= vec(1);
            d    <= vec(0);

            wait for CLK_PERIOD;               -- стабилизация

            write(L, to_boolean(clr));  write(L, ' ');
            write(L, to_boolean(load)); write(L, ' ');
            write(L, to_boolean(up));   write(L, ' ');
            write(L, to_boolean(down)); write(L, ' ');
            write(L, to_boolean(a));    write(L, ' ');
            write(L, to_boolean(b));    write(L, ' ');
            write(L, to_boolean(c));    write(L, ' ');
            write(L, to_boolean(d));    write(L, ' ');
            write(L, to_boolean(qa));   write(L, ' ');
            write(L, to_boolean(qb));   write(L, ' ');
            write(L, to_boolean(qc));   write(L, ' ');
            write(L, to_boolean(qd));   write(L, ' ');
            write(L, to_boolean(bo));   write(L, ' ');
            write(L, to_boolean(co));
            writeline(f, L);
        end loop;

        file_close(f);
        report "Файл als193a_input_values.txt успешно создан (257 строк)" severity note;
        wait;
    end process;
end Behavioral;