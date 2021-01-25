player_sprite	dcb 7, 0, 0, 0		; spritenr, ofsx, ofsy, flags
player_animdata	dcb 1, 0, 3, 4		; flags, state, facing, frame
;flags:
;1: playing
;states:
;0: cast/idle
;1: stab
;2: walk
;3: slash
;4: death (single facing, will break)
player_pos 		dcd 128 ; x
				dcd 129 ; y

player_ofs		dcd 288 ; x
				dcd 288 ; y

hurtbox_cnt		dcd 10
hurtbox_arr
; attacker:
; 255 = inactive
; 254 = player
				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

				dcd 0,0,0,0		; posx, posy, width, height
				dcb 0,0,0,0		; attacker, type, dmg

mob_cnt			dcd 3
mob_arr
				dcb 6, 0, 0, 0	; sprite
				dcd 150			; posx
				dcd 150			; posy
				dcb 1, 0, 1, 0	; hp, armor, atk, ???
				dcb 1, 0, 0, 7	; flags, state, facing, frame

				dcb 6, 0, 0, 0
				dcd 200
				dcd 150
				dcb 1, 0, 1, 0	; hp, armor, atk, ???
				dcb 1, 0, 1, 7	; flags, state, facing, frame

				dcb 6, 0, 0, 0
				dcd 100
				dcd 250
				dcb 1, 0, 1, 0	; hp, armor, atk, ???
				dcb 1, 0, 2, 7	; flags, state, facing, frame



tile_size		dcd 64
tile_rows		dcd 63
tile_cols		dcd 64 ; word-aligned
map_tiles		
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			dcb 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; sprite nr
; walkable
tile_struct		
tile_door		dcb 5, 0
tile_stone		dcb 3, 1
tile_dirt		dcb 4, 1
tile_grass		dcb 2, 1

DRAW_MAP
;r0: sprite index
;r1: sprite x
;r2: sprite y
;r3: unused
;r4: current tile x
;r5: initial tile x
;r6: final tile x
;r7: initial/current tile y
;r8: final tile y
;r9: pixel offset x
;r10: pixel offset y
;r11: sprite size
;r12: addresses/map #rows/map #cols
	adr r12, =player_pos
	ldr r9, [r12], #4		; posx
	ldr r10, [r12], #4		; posy

	add r5, r9, #32			; Ix
	mov r5, r5 lsr #6		; mask out 64 for tile index
	add r6, r5, #6			; Fx
	subs r5, r5, #6			; skip negative tiles
	movmi r5, #0
	mov r4, r5				; Cx

	add r7, r10, #64 		; Iy = Cy
	mov r7, r7 lsr #6		; mask out 64 for tile index
	add r8, r7, #6			; Fy
	subs r7, r7, #6			; skip negative tiles
	movmi r7, #0

	adr r12, =player_ofs
	ldr r1, [r12], #4		; ofsx
	ldr r2, [r12], #4		; ofsy
	sub r9, r9, r1			; total ofs
	sub r10, r10, r2
	
	adr r12, =tile_rows		; # rows
	ldr r12, [r12]
	cmp r7, r12
	movge r7, r12

	adr r12, =tile_cols		; # cols
	ldr r12, [r12]
	cmp r6, r12
	movge r6, r12

	mov r11, #64			; sprite size

draw_map_row
	mla r2, r7, r12, r4		; tile address ofs

	adr r1, =map_tiles
	add r1, r1, r2
	ldrb r1, [r1]			; tile #
	
	adr r2, =tile_struct
	add r2, r2, r1, lsl #2	; tile = 4 bytes
	ldrb r0, [r2]			; first byte is sprite #

	mul r1, r4, r11
	sub r1, r1, r9			; subtract ofs
	mul r2, r7, r11
	sub r2, r2, r10			; subtract ofs
	
	swi swi_drawsprite_bg
	add r4, r4, #1			; Cx
	cmp r4, r6
	bne draw_map_row

	add r7, r7, #1			; Cy
	mov r4, r5				; reset Cx
	cmp r7, r8				; Cy = Fy?
	bne draw_map_row

	mov pc, lr

PLAYER_TICK
;animation and facing logic
;r0: input
;r6: anim
;	2: walk
;	3: slash
;r7: frame
;r8: address
;r9: jump address
	adr r8, =player_animdata

	ldrb r7, [r8, #3]		; frame
	subs r7, r7, #1
	movmi r7, #7			; loop anim back
	strb r7, [r8, #3]
;0: cast/idle
;1: stab
;2: walk
;3: slash
;4: death (single facing, will break)


	ldrb r6, [r8, #1]		; state
	cmp r6, #0
	beq state_idle
; no stab yet
	cmp r6, #2
	beq state_movement
	cmp r6, #3
	beq state_attack

state_idle
	tst r0, #0xf		; movement key, set state to move
	movne r6, #2
	tst r0, #0xf0		; attack key, set state to attack
	movne r6, #3


	adr r8, =player_animdata
	cmp r6, #0
	movne r7, #7		; reset animation
	strbne r6, [r8, #1]	; state
	strbne r7, [r8, #3]	; frame
	b state_done

;;;;;;;;;;;;;;;;;;;;; MOVEMENT ;;;;;;;;;;;;;;;;;;;;; 
state_movement
	adr r8, =player_animdata
	cmp r0, #0			; no input, set state to idle
	moveq r6, #0
	tst r0, #0xf0		; attack key, set state to attack
	movne r6, #3

	cmp r6, #2			; state changed, exit early
	strbne r6, [r8, #1]
	bne state_done

;set facing
	tst r0, #1		; KEY_UP
	movne r7, #0
	tst r0, #2		; KEY_DOWN
	movne r7, #2
	tst r0, #4		; KEY_LEFT
	movne r7, #1
	tst r0, #8		; KEY_RIGHT
	movne r7, #3
;store facing
	strb r7, [r8, #2]		; facing

;movement logic
;r0: input
;r1: posx
;r2: posy
;r3: movement x
;r4: movement y
;r5: movement speed
;r8: address
	mov r3, #0
	mov r4, #0
	mov r5, #5				;default movement speed

	tst r0, #3				;mask UPDOWN
	tstne r0, #12			;mask LEFTRIGHT
	movne r5, #4			;diagonal movement speed

	adr r8, =player_pos
	ldr r1, [r8, #0]		; posx
	ldr r2, [r8, #4]		; posy

	tst r0, #1				; KEY_UP
	mvnne r4, r5
	tst r0, #2				; KEY_DOWN
	movne r4, r5
	tst r0, #4				; KEY_LEFT
	mvnne r3, r5
	tst r0, #8				; KEY_RIGHT
	movne r3, r5

	add r1, r1, r3
	stmib sp!, {lr}			; store LR
	bl COLLIDE_TERRAIN
	ldmda sp!, {lr}
	;walkable in r9
	cmp r9, #0
	subeq r1, r1, r3

	add r2, r2, r4
	stmib sp!, {lr}			; store LR
	bl COLLIDE_TERRAIN
	ldmda sp!, {lr}
	;walkable in r9
	cmp r9, #0
	subeq r2, r2, r4

	adr r8, =player_pos
	str r1, [r8, #0]		; posx
	str r2, [r8, #4]		; posy

	b state_done
;;;;;;;;;;;;;;;;;;;;; ATTACK ;;;;;;;;;;;;;;;;;;;;; 
state_attack
;r0: sprite #
;r1: sprite x, attack minx
;r2: sprite y, attack miny
;r3: facing
;r7: frame
;r12: address

	adr r12, =player_animdata
	cmp r7, #0				; r7 holds frame value
	moveq r3, #0
	strbeq r3, [r12, #1]		; frame == 0, set idle
	beq state_done


	adr r12, =player_ofs
	mov r0, #3
	ldr r1, [r12], #4		; posx
	ldr r2, [r12], #4		; posy

	adr r3, =player_animdata
	ldrb r3, [r3, #2]		; facing

; default attack
; size: 64x64
; dmg: 1
	cmp r3, #0				; UP
	addeq r1, r1, #0		; attack ofs x
	subeq r2, r2, #10		; attack ofs y
	cmp r3, #2				; DOWN
	addeq r1, r1, #0
	addeq r2, r2, #60
	cmp r3, #1				; LEFT
	subeq r1, r1, #42
	addeq r2, r2, #10
	cmp r3, #3				; RIGHT
	addeq r1, r1, #44
	addeq r2, r2, #10
; draw attack
	swi swi_drawsprite_fg
; do attack handling on frame 4
	cmp r7, #4				; r7 holds frame value
	bne state_done			; r7 frame != 4, no attack handling this frame

	b state_done

state_done
	mov pc, lr

COLLIDE_TERRAIN
;r9: total tile index/walkable
;r1: posx
;r2: posy
;r10: tile index x
;r11: tile index y
;r12: addresses/#cols/tile ofs
	add r10, r1, #32			; ofs: 32px
	mov r10, r10 lsr #6			; mask out 64 for tile index
	add r11, r2, #64 			; ofs: 64px
	mov r11, r11 lsr #6

	adr r12, =tile_cols
	ldr r12, [r12]				; cols
	mla r9, r11, r12, r10		; y*cols + x; ofs in r9
	
	adr r12, =map_tiles
	add r12, r12, r9
	ldrb r10, [r12]				; tile #

	adr r12, =tile_struct		
	add r12, r12, r10, lsl #2	; 2nd byte = walkable
	ldrb r9, [r12, #1]			; walkable in r0

	mov pc, lr

DRAW_PLAYER
;r0: sprite nr
;r1: posx
;r2: posy
;r3: address

;r4: flags
;r5: anim
;r6: facing
;r7: cur frame
	adr r3, =player_sprite
	ldr r0, [r3]

	adr r3, =player_animdata
	ldrb r4, [r3, #0]		; flags
	ldrb r5, [r3, #1]		; state
	ldrb r6, [r3, #2]		; facing
	ldrb r7, [r3, #3]		; frame

	add r5, r6, r5 lsl #2	; state*4 + facing
	orr r0, r0, r5 lsl #16	; Y-frame in 2nd byte
	orr r0, r0, r7 lsl #8

	adr r3, =player_ofs
	ldr r1, [r3], #4		; posx
	ldr r2, [r3], #4		; posy
	swi swi_drawsprite_fg
	mov pc, lr

MOB_TICK
;r0: sprite nr
;r1: posx
;r2: posy
;r3: mob it
;r5: mob cnt
;r7: frame
;r8: address
;r10: address
;r11: ofsx
;r12: ofsy
	adr r10, =player_ofs
	ldr r7, [r10], #4		; ofsx
	ldr r8, [r10], #4		; ofsy

	adr r10, =player_pos
	ldr r11, [r10], #4		; posx
	ldr r12, [r10], #4		; posy

	sub r11, r7, r11
	sub r12, r8, r12

	adr r5, =mob_cnt
	ldr r5, [r5]			; mob_cnt
	adr r10, =mob_arr
	sub r10, r10, #4
	mov r3, #0
tick_mob_loop
	add r10, r10, #16
	ldrb r6, [r10, #0]		; hp
	cmp r6, #0
	moveq r6, #1
	strbeq r6, [r10, #5]		;4+1: state

	add r10, r10, #4
	ldrb r6, [r10, #0]		; flags
	ldrb r7, [r10, #1]		; state
	ldrb r8, [r10, #2]		; facing
	ldrb r9, [r10, #3]		; frame

	subs r9, r9, #1
	movmi r9, #7			; loop anim back
	strb r9, [r10, #3]

swi swi_printreg
	add r3, r3, #1
	cmp r3, r5
	blt tick_mob_loop
	mov pc, lr

DRAW_MOBS
;r0: sprite nr
;r1: posx
;r2: posy
;r3: mob iterator/ address
;r4: rfu, probs frame
;r5: mob cnt
;r6: ofsx
;r7: ofsy
;r8: address
	adr r3, =player_ofs
	ldr r4, [r3], #4		; ofsx
	ldr r5, [r3], #4		; ofsy

	adr r3, =player_pos
	ldr r6, [r3], #4		; posx
	ldr r7, [r3], #4		; posy

	sub r6, r4, r6
	sub r7, r5, r7

	mov r3, #0				; mob_it
	adr r5, =mob_cnt
	ldr r5, [r5]			; mob_cnt

	adr r8, =mob_arr

draw_mob_loop
	ldr r0, [r8], #4		; sprite
	ldr r1, [r8], #4		; posx
	ldr r2, [r8], #4		; posy
	add r8, r8, #4			; flags

	ldrb r9, [r8, #0]		; flags
	ldrb r10, [r8, #1]		; state
	ldrb r11, [r8, #2]		; facing
	ldrb r12, [r8, #3]		; frame
	add r8, r8, #4			; next mob

	add r10, r11, r10 lsl #2	; state*4 + facing
	orr r0, r0, r10 lsl #16	; Y-frame in 2nd byte
	orr r0, r0, r12 lsl #8
;swi swi_printreg

	add r1, r1, r6			; apply ofs
	add r2, r2, r7

	add r3, r3, #1
	swi swi_drawsprite_fg

	cmp r3, r5
	blt draw_mob_loop
	mov pc, lr

_start
	adr sp, =stack
	swi swi_initraylib
RUNRAYLIB
	bl PLAYER_TICK
	bl MOB_TICK
	bl DRAW_MOBS
	bl DRAW_PLAYER
	bl DRAW_MAP
	swi swi_runraylib
	tst r0, #16			; KEY_Q
	bne DONE
	b RUNRAYLIB

DONE
	END

stack			fill 10
