stack			fill 4

player_sprite	dcd 1
player_pos 		dcd 0 ; x
				dcd 0 ; y
player_ofs		dcd 400 ; x
				dcd 225 ; y

tile_size		dcd 64
tile_rows		dcd 3
tile_cols		dcd 8	; word-aligned
map_tiles		dcb 0, 1, 0, 1, 0, 1, 0, 1
				dcb 2, 3, 2, 3, 2, 3, 2, 3
				dcb 1, 0, 1, 0, 1, 0, 1, 0

; sprite nr
; walkable
tile_struct		
tile_grass		dcb 2, 1
tile_stone		dcb 3, 1
tile_dirt		dcb 4, 1
tile_door		dcb 5, 0

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
;r12: tile struct address
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
	ldrb r0, [r11], #1		; increments tile

	adr r12. =tile_struct
	add r12, r12, r0 lsl #2	; 4 bytes per tile
	ldrb r0, [r12]

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

MOVE_PLAYER
; r0: inputs
; r1: posx
; r2: posy
; r3: pos adr
; r4: movement x
; r5: movement y
; r6: movement speed
	mov r4, #0
	mov r5, #0
	mov r6, #5			; movement speed

	tst r0, #3			; mask: 0011
	tstne r0, #12		; mask: 1100
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

	add r1, r1, #32			; ofs: 32px
	mov r1, r1 lsr #6		; mask out 64
	add r2, r2, #64 		; ofs: 64px
	mov r2, r2 lsr #6

	adr r3, =tile_cols
	ldr r3, [r3]		; cols
	mov r4, r1
	mla r4, r4, r3, r2		; x+y*cols
	adr r3, =map_tiles
	add r3, r3, r4
	ldrb r5, [r3]		; r5 = tile #

	adr r3. =tile_struct
	add r3, r3, r5 lsl #2	; 4 bytes per tile
	add r3, r3, #1
	ldrb r0, [r3]			; r0 = walkable?

	mov pc, lr

COLLIDE_TERRAIN
;r0: original posx/out posx
;r1: original posy/out posy
;r2: deltax
;r3: deltay

;r4: # collumns
	adr r4, =tile_cols
	ldr r4, [r4]

;r5: tile adr ofs
	mov r5, r0			; x
	mla r5, r5, r5, r1		; x+y*cols
;r4: tile #
	adr r4, =map_tiles
	add r4, r4, r5
	ldrb r4, [r4]

;r4: walkable
;r5: tile ofs
	adr r5, =tile_struct
	add r5, r5, r4 lsl #2
	add r5, r5, #1
	ldrb r4, [r4]

	swi swi_printreg
	mov pc, lr

DRAW_PLAYER
;r0: sprite nr
;r1: posx
;r2: posy
;r3: adress/drawable
	mov r0, #1				; spritenr
	adr r3, =player_ofs
	ldr r1, [r3], #4		; posx
	ldr r2, [r3], #4		; posx
	mov r3, #0				; drawable
	swi swi_drawsprite_fg
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
