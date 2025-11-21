library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity top_als193a is port (
    up, down, clr, load, a, b, c, d : in  std_logic;
    qa, qb, qc, qd                  : out std_logic;
    clk                             : in  std_logic
); end;

architecture arc of top_als193a is
    component als193a port(
        up,down,clr,load : in  std_logic;
        a,b,c,d          : in  std_logic;
        qa,qb,qc,qd      : out std_logic
    ); end component;
begin
    U1: als193a port map(up, down, clr, load, a, b, c, d, qa, qb, qc, qd);
end;