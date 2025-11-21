library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity d_trigger is
    Port ( D : in STD_LOGIC;
           C : in STD_LOGIC;
           R : in STD_LOGIC;
           Q : out STD_LOGIC);
end d_trigger;

architecture Structural of d_trigger is
    component jk_trigger
        Port ( J : in STD_LOGIC;
               K : in STD_LOGIC;
               C : in STD_LOGIC;
               S : in STD_LOGIC;
               R : in STD_LOGIC;
               Q : out STD_LOGIC;
               Qbar : out STD_LOGIC);
    end component;
    signal j_sig, k_sig, q_int, qbar_int : STD_LOGIC;
begin
    j_sig <= D;
    k_sig <= not D;
    uut_jk: jk_trigger port map (
        J => j_sig, K => k_sig, C => C, S => '0', R => R,
        Q => q_int, Qbar => qbar_int
    );
    Q <= q_int;
end Structural;