list   fill 10

; r0: addres
; r1: counter
; r2: smallest
; r3: current
_start
mov r1, #3
mov r2, #4
mov r3, #5
	mul r0, r1, r2
	mla r0, r1, r2, r3
	swi swi_printmem
END
