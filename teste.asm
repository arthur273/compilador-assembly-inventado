mod1: begin
mod2: extern
public data
public datb
input data
input datb
input datc
load data
sub datb
jmpp ab
load datb
sub datc
jmpn cab
output datb
copy datb,maior
jmp par
ab: load data
sub datc
jmpn cab
output data
copy data,maior
jmp par
cab: copy datc,maior
output datc
par: load maior
div dois
mul dois
sub maior
jmpz fim
output um
jmp final
fim: output zero
final: jmp mod2
datb: space
data: space
datc: space
maior: space
dois: const 2
um: const 1
zero: const 0
end
