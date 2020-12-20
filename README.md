# ARM simulator
Simulates an armv4 machine.

Assembly code is parsed, assembled and executed.
C functions can be called by swi instruction.

## Supported instructions:
```
mov
mvn
add
adc
sub
sbc
rsb
rsc
and
eor
bic
orr
cmp
cmn
tst
teq
mul
mla
ldr
str
b(l)
swi
adr
```
adr instruction is translated to the following:
```
ldr rX, [pc, 8]
add pc, #4
dcd IMM
```

## Supported literal mnemonics:
```
dcd
dcb
fill
```

## Untested instructions:
```
ldm
stm
```

Execution ends with mnemonic END, opcode 0xFFFFFFFF

## varia
The genops directory contains a program to print a bunch of assembly code. Not all supported mnemonics are printed.
