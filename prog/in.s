player_sprite	dcd 1
player_pos 		dcd 200	; x
				dcd 300	; y
; input: r0 is inputs
; clobber: r1-r3
MOVE_PLAYER
	mov r3, =player_pos
	ldr r1, [r3], #4		; posx
	ldr r2, [r3], #4		; posy

	tst r0, #1			; KEY_UP
	subne r2, r2, #1
	tst r0, #2			; KEY_DWN
	addne r2, r2, #1

	tst r0, #8			; KEY_RIGHT
	addne r1, r1, #1
	tst r0, #4			; KEY_LEFT
	subne r1, r1, #1

	mov r3, =player_pos
	str r1, [r3], #4		; posx
	str r2, [r3], #4		; posy
	mov pc, lr


; start draw player
; clobber: r0-r3
DRAW_PLAYER
	mov r0, #1				; spritenr
	mov r3, =player_pos
	ldr r1, [r3], #4		; posx
	ldr r2, [r3], #4		; posx
	mov r3, #0				; drawable
	swi swi_drawsprite_fg
	mov pc, lr

_start
;	swi swi_initraylib
RUNRAYLIB
	bl MOVE_PLAYER
	bl DRAW_PLAYER
	swi swi_runraylib
	b RUNRAYLIB
	
	swi swi_printreg
END
