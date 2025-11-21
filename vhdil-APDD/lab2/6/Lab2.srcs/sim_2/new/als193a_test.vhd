library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_TEXTIO.ALL;
use STD.TEXTIO.ALL;

entity als193a_test is
end;

architecture beh of als193a_test is
    component als193a
        port(clr,load,up,down,a,b,c,d:in std_logic; qa,qb,qc,qd,bo,co:out std_logic);
    end component;
    signal clr,load,up,down,a,b,c,d,qa,qb,qc,qd,bo,co : std_logic := '0';
    constant W : time := 25 ns;
    constant P : time := 50 ns;
begin
    uut: als193a port map(clr,load,up,down,a,b,c,d,qa,qb,qc,qd,bo,co);

    process
        variable l : line;
        file f : text open read_mode is "als193a_results.txt";
        variable exp : std_logic_vector(5 downto 0);
        variable e : std_logic_vector(5 downto 0);
    begin
        report "=== реяр мювюр ===" severity note;

        clr<='1'; wait for W; clr<='0'; wait for P-W;
        readline(f,l); hread(l,exp); e:=qa&qb&qc&qd&bo&co;
        assert e=exp report "CLEAR FAIL" severity failure;

        for s in 0 to 15 loop
            (a,b,c,d) <= std_logic_vector(to_unsigned(s,4));
            wait for 10 ns; load<='0'; wait for W; load<='1'; wait for P;
            readline(f,l); hread(l,exp); e:=qa&qb&qc&qd&bo&co;
            assert e=exp report "LOAD "&integer'image(s)&" FAIL" severity failure;

            down<='1';
            for i in 1 to 16 loop
                up<='0'; wait for W; up<='1'; wait for W;
                readline(f,l); hread(l,exp); e:=qa&qb&qc&qd&bo&co;
                assert e=exp report "UP "&integer'image(s)&"."&integer'image(i) severity failure;
            end loop;
            up<='0'; wait for 10 ns;

            up<='1'; down<='0'; wait for P;

            for i in 1 to 16 loop
                down<='0'; wait for W; down<='1'; wait for W;
                readline(f,l); hread(l,exp); e:=qa&qb&qc&qd&bo&co;
                assert e=exp report "DOWN "&integer'image(s)&"."&integer'image(i) severity failure;
            end loop;
            down<='0'; wait for P;
        end loop;

        file_close(f);
        report "=== 513 опнбепнй опнидемн ===" severity note;
        wait;
    end process;
end;