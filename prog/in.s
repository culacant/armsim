player_sprite	dcd 1
player_pos 		dcd 0 ; x
				dcd 0 ; y
player_ofs		dcd 400 ; x
				dcd 225 ; y

tile_size		dcd 64
tile_rows		dcd 3
tile_cols		dcd 5
map_tiles		dcd 2, 3, 2, 3, 2
				dcd 4, 5, 4, 5, 4
				dcd 3, 2, 3, 2, 3

DRAW_MAP
;r0: tile #
;r1: posx
;r2: posy
;r3: drawable
;r4: cur row
;r5: row cnt
;r6: cur col
;r7: col cnt
;r8: tile size
;r9: player posx
;r10: player posy
;r11: addresses

	adr r11, =player_pos	; player pos
	ldr r9, [r11], #4
	ldr r10, [r11], #4
	adr r11, =player_ofs	; player ofs
	ldr r1, [r11], #4
	ldr r2, [r11], #4
	sub r9, r9, r1			; total ofs for relative pos
	sub r10, r10, r2

	mov r1, #0
	mov r2, #0
	mov r3, #0

	adr r11, =tile_rows
	mov r4, #0
	ldr r5, [r11]

	adr r11, =tile_cols
	mov r6, #0
	ldr r7, [r11]

	adr r11, =tile_size
	ldr r8, [r11]

	adr r11, =map_tiles

draw_map_col
	ldr r0, [r11], #4		; increments tile
	sub r1, r1, r9			; subtract ofs for relative pos
	sub r2, r2, r10
	swi swi_drawsprite_bg
	add r1, r1, r9			; reset pos to absolute
	add r2, r2, r10

	add r6, r6, #1			; cur col
	cmp r6, r7				; reached end of col
	addne r1, r1, r8		; tile size
	addne r3, r3, #1		; drawable
	bne draw_map_col

	add r4, r4 #1
	cmp r4, r5

	; r0 updated in loop
	movne r1, #0
	addne r2, r2, r8
	addne r3, r3, #1
	; r4 taken care of
	; r5 no change
	movne r6, #0
	; r7 no change
	bne draw_map_col
	
	mov pc, lr

; input: r0 is inputs
; clobber: r1-r6
MOVE_PLAYER
	cmp r0, #0
;	moveq pc, lr		; no inputs

	mov r4, #0
	mov r5, #0

	mov r6, #5			; movement speed

	mov r10, #0
	tst r0, #3
	tstne r0, #12
	movne r6, #4		; diagonal speed = 2/3ths normal

	tst r0, #1			; KEY_UP
	subne r5, r5, r6
	tst r0, #2			; KEY_DWN
	addne r5, r5, r6

	tst r0, #8			; KEY_RIGHT
	addne r4, r4, r6
	tst r0, #4			; KEY_LEFT
	subne r4, r4, r6

	adr r3, =player_pos
	ldr r1, [r3], #4		; posx
	ldr r2, [r3], #4		; posy

	add r1, r1, r4
	add r2, r2, r5

	adr r3, =player_pos
	str r1, [r3], #4		; posx
	str r2, [r3], #4		; posy
	mov r0, #0
	mov pc, lr


; start draw player
; clobber: r0-r3
DRAW_PLAYER

	mov r0, #1				; spritenr
	adr r3, =player_ofs
	ldr r1, [r3], #4		; posx
	ldr r2, [r3], #4		; posx
	mov r3, #0				; drawable
	swi swi_drawsprite_fg
	swi swi_printreg
	mov pc, lr

_start
	swi swi_initraylib
RUNRAYLIB
	bl MOVE_PLAYER
	bl DRAW_PLAYER
	bl DRAW_MAP
	swi swi_runraylib
	b RUNRAYLIB
END
