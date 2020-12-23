# ARM simulator
Simulates an armv4 machine.

Assembly code is parsed, assembled and executed.
C functions can be called by adding them to the SWI table and calling the swi instruction.

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
ldr(b)
str(b)
ldm
stm
b(l)
swi
adr			; pseudo-instruction, see below
END			; custom instruction, halts the program (opcode 0xFFFFFFFF)
```

The adr pseudo-instruction is translated to the following:
```
ldr rX, [pc, 8]
add pc, #4
dcd IMM
```

## Supported literal mnemonics:
```
dcd			; 4 byte literal
dcb			; 1 byte literal, line padded to 4 byte alignment
fill		; x*4 byte literal
```

## Unsupported instructions:
```
; coprocessor instructions
ldc
mcr
mrc

; PSR instructions
mrs
msr
stc

; swaps
swp
swpb

; 64-bit multiplication
smlal
smull
umlal
umull
```

## varia
- The genops directory contains a program to print a bunch of assembly code. Not all supported mnemonics are printed.
- Half-word (16 bit) adressing is not supported at time of writing.
- byte (8 bit) adressing is kinda janky due to x86 endianness. It should work though.
