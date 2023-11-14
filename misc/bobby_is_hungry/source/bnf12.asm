;Author: belial
;E-Mail: christian.ammann@phobosys.de
;
;Notes:
;
;PISTOL = 0 -> Play with Joystick
;PISTOL = 1 -> Play with Pistol, show normal game screen
;PISTOL = 2 -> Trigger was pressed, show whyte screen
;PISTOL = 3 -> Cross has new position, check one frame for collision check
;
;HUHN_HEALTH_DIRECTION = %00000000
;D0 = Color
;D1 = Direction
;D2 D3 = Speed
;D4 D5 D6 D7 = Sprite 

	processor 6502
	include "vcs.h"
	include "macro.h"
	include "moorhuhn.h"

	ORG $1000
	
Reset

	ldx #0
	txa

Clear   dex
	txs
	pha
	bne Clear
	
	jsr Initialisation
	
StartOfFrame

	lda #2
	sta VSYNC

	; 3 scanlines of VSYNCH signal...

	sta WSYNC
	sta WSYNC
	sta WSYNC

	lda #0		;[2]
	sta VSYNC	;[3]          

	; Vertical blank...
	
	lda TIMER 	;[5]
	sta TIM64T	;[8]
	;---------------------------
	;game logic begins
	
	;if NTSC, slow down game logic
	#if PAL = 0
	ldx NTSCBREAKER
	dex
	beq no_game_logic
	stx NTSCBREAKER
	jmp pistol_whitescreen_stuff
no_game_logic
	ldx #6
	stx NTSCBREAKER
	ldx PLAYERY0
	stx PLAYERYTEMP0
	jmp vblank_loop
	#endif
	
	;make a break after we had a white screen
	;for pistol coordinates stuff
pistol_whitescreen_stuff
	lda #PISTOL
	cmp #2
	bne reset_check
	jmp load_pistol
	
	;check wether reset button was pressed
reset_check
	lda SWCHB
	and #%00000001
	bne no_reset_pressed
	jsr Initialisation
no_reset_pressed
	lda GAMESTATUS
	bne running_game
	
start_screen
	;generate sound
	jsr LaunchSoundEngine
	clc
	lda RAND1
	adc #7
	sta RAND1
	bcc no_carry_rand
	inc RAND2
no_carry_rand
	jsr CheckControler
	jmp vblank_loop
	
running_game
	;generate sound
	jsr LaunchSoundEngine
	jsr Timing
	jsr LoadScore
	jsr MoveChickens
	jsr LoadChickens
	
	;check wether to use Pistol oder Joystick
	ldx PISTOL
	bne load_pistol
	jsr LoadAiming

	jmp vblank_loop
load_pistol
	jsr LoadPistol
	
	;game logic ends
	;---------------------------
vblank_loop
	ldx INTIM
	bne vblank_loop
	sta WSYNC
	#if PAL = 0
	sta WSYNC
	#endif

	; deactivate the blank pattern
	lda #0
	sta VBLANK              

	; the kernel
	; --------------------------
	ldx GAMESTATUS
	bne ShowGame
	
	jsr Title
	jmp finished_kernel
ShowGame
	ldx PISTOL
	cpx #2
	bne show_running_game
show_pistol_screen
	jsr GetGun_Kernel
	jmp finished_kernel
show_running_game
	jsr Game

finished_kernel	
	;-------------------------------
	; end of screen - enter blanking
	
	lda #%10000010
	sta VBLANK
	lda #0  
	sta COLUBK
	sta COLUPF
	
	; overscan...

	ldx #OVERSCAN
overscan_kernel	
	sta WSYNC
	dex
	bne overscan_kernel
	                  
	jmp StartOfFrame
		
;---------------------------
	;constants
;---------------------------

	ORG $1100

digit_0_offset
POS	SET digit_0
	word POS
digit_1_offset
POS	SET digit_1
	word POS
digit_2_offset
POS	SET digit_2
	word POS
digit_3_offset
POS	SET digit_3
	word POS
digit_4_offset
POS	SET digit_4
	word POS
digit_5_offset
POS	SET digit_5
	word POS
digit_6_offset
POS	SET digit_6
	word POS
digit_7_offset
POS	SET digit_7
	word POS
digit_8_offset
POS	SET digit_8
	word POS
digit_9_offset
POS	SET digit_9
	word POS	
		
digit_5
	byte #%00000000
	byte #%01111100
	byte #%00000010
	byte #%00000010
	byte #%00111100
	byte #%01000000
	byte #%01000000
	byte #%01111110
	byte #%00000000
		
digit_0
	byte #%00000000
	byte #%00111100
	byte #%01000010
	byte #%01000010
	byte #%01000010
	byte #%01000010
	byte #%01000010
	byte #%00111100
	byte #%00000000	
	
digit_1
	byte #%00000000
	byte #%00000100
	byte #%00000100
	byte #%00000100
	byte #%00000100
	byte #%00100100
	byte #%00010100
	byte #%00001100
	byte #%00000000

digit_2
	byte #%00000000
	byte #%01111110
	byte #%00100000
	byte #%00010000
	byte #%00001000
	byte #%00000100
	byte #%01000010
	byte #%00111100
	byte #%00000000
	
digit_3
	byte #%00000000
	byte #%01111100
	byte #%00000010
	byte #%00000010
	byte #%01111100
	byte #%00000010
	byte #%00000010
	byte #%01111100
	byte #%00000000
	
digit_4
	byte #%00000000
	byte #%00001000
	byte #%00001000
	byte #%01111110
	byte #%01001000
	byte #%00100000
	byte #%00010000
	byte #%00001000
	byte #%00000000
		
digit_6
	byte #%00000000
	byte #%00111100
	byte #%01000010
	byte #%01000010
	byte #%01111100
	byte #%01000000
	byte #%01000000
	byte #%00111110
	byte #%00000000
		
digit_7
	byte #%00000000
	byte #%01000000
	byte #%00100000
	byte #%00010000
	byte #%00001000
	byte #%00000100
	byte #%00000010
	byte #%01111110
	byte #%00000000
		
digit_8
	byte #%00000000
	byte #%00111100
	byte #%01000010
	byte #%01000010
	byte #%00111100
	byte #%01000010
	byte #%01000010
	byte #%00111100
	byte #%00000000
	
digit_9
	byte #%00000000
	byte #%00111100
	byte #%00000010
	byte #%00000010
	byte #%00111110
	byte #%01000010
	byte #%01000010
	byte #%00111100
	byte #%00000000
		
char_p_offset
POS	SET char_p
	word POS
char_p	
	byte #%00000000
	byte #%01000000
	byte #%01000000
	byte #%01000000
	byte #%01111100
	byte #%01000010
	byte #%01000010
	byte #%01111100
	byte #%00000000
	
char_i_offset
POS	SET char_i
	word POS
char_i	
	byte #%00000000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00000000

char_s_offset
POS	SET char_s
	word POS
char_s	
	byte #%00000000
	byte #%01111000
	byte #%00000100
	byte #%00000100
	byte #%00111000
	byte #%01000000
	byte #%01000000
	byte #%00111100
	byte #%00000000
	
char_t_offset
POS	SET char_t
	word POS
char_t	
	byte #%00000000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%11111110
	byte #%00000000
	
char_o_offset
POS	SET char_o
	word POS
char_o	
	byte #%00000000
	byte #%00111100
	byte #%01000010
	byte #%01000010
	byte #%01000010
	byte #%01000010
	byte #%01000010
	byte #%00111100
	byte #%00000000

char_l_offset
POS	SET char_l
	word POS
char_l	
	byte #%00000000
	byte #%01111110
	byte #%01000000
	byte #%01000000
	byte #%01000000
	byte #%01000000
	byte #%01000000
	byte #%01000000
	byte #%00000000
	
char_j_offset
POS	SET char_j
	word POS
char_j
	byte #%00000000
	byte #%00011000
	byte #%00100100
	byte #%01000010
	byte #%00000010
	byte #%00000010
	byte #%00000010
	byte #%01111110
	byte #%00000000	

char_y_offset
POS	SET char_y
	word POS
char_y	
	byte #%00000000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00010000
	byte #%00101000
	byte #%01000100
	byte #%10000010
	byte #%00000000
	
char_ic_offset
POS	SET char_ic
	word POS	
char_ic
	byte #%00000000
	byte #%10001110
	byte #%10010000
	byte #%10100000
	byte #%10100000
	byte #%10100000
	byte #%10010000
	byte #%10001110
	byte #%00000000

char_k_offset
POS	SET char_k
	word POS	
char_k	
	byte #%00000000
	byte #%01000100
	byte #%01001000
	byte #%01010000
	byte #%01100000
	byte #%01010000
	byte #%01001000
	byte #%01000100
	byte #%00000000

char_oy_offset
POS	SET char_oy
	word POS	
char_oy	
	byte #%0000000
	byte #%0100010
	byte #%1010010
	byte #%1010010
	byte #%1010010
	byte #%1010010
	byte #%1010101
	byte #%0100101
	byte #%0000000	

no_chicken_offset
POS	SET no_chicken
	word POS
explosion_1_offset
POS	SET explosion_1
	word POS
explosion_2_offset
POS	SET explosion_2
	word POS
explosion_3_offset
POS	SET explosion_3
	word POS
chicken_1_offset
POS	SET chicken_1
	word POS
chicken_2_offset
POS	SET chicken_2
	word POS
chicken_2_back_offset
POS	SET chicken_2
	word POS
chicken_3_offset
POS	SET chicken_3
	word POS
chicken_4_offset
POS	SET chicken_4
	word POS
chicken_5_offset
POS	SET chicken_5
	word POS
chicken_5_back_offset
POS	SET chicken_5
	word POS
chicken_6_offset
POS	SET chicken_6
	word POS
chicken_7_offset
POS	SET chicken_7
	word POS
chicken_8_offset
POS	SET chicken_8
	word POS
chicken_8_back_offset
POS	SET chicken_8
	word POS
chicken_9_offset
POS	SET chicken_9
	word POS

explosion_1
	byte #%00000000
	byte #%00011000
	byte #%01111110
	byte #%11111111
	byte #%01111110
	byte #%00011000
	byte #%00000000
	
explosion_2
	byte #%00000000
	byte #%00011000
	byte #%01111110
	byte #%11111111
	byte #%01111110
	byte #%00011000
	byte #%00000000
	
explosion_3
	byte #%00000000
	byte #%00011000
	byte #%01111110
	byte #%11111111
	byte #%01111110
	byte #%00011000
	byte #%00000000
				
chicken_1
	byte #%00100100
	byte #%00100100
	byte #%00011000
	byte #%10111101
	byte #%01111110
	byte #%00011000
	byte #%00011000
		
chicken_2
	byte #%00100100
	byte #%00100100
	byte #%00011000
	byte #%00111100
	byte #%11111111
	byte #%00011000
	byte #%00011000
			
chicken_3
	byte #%00100100
	byte #%00100100
	byte #%00011000
	byte #%00111100
	byte #%00111100
	byte #%01011010
	byte #%10011001
			
chicken_4
	byte #%00000000
	byte #%00000000
	byte #%00011000
	byte #%00011000
	byte #%00100100
	byte #%01000010
	byte #%00000000
			
chicken_5
	byte #%00000000
	byte #%00000000
	byte #%00011000
	byte #%01111110
	byte #%00000000
	byte #%00000000
	byte #%00000000
			
chicken_6
	byte #%00000000
	byte #%01000010
	byte #%00111100
	byte #%00011000
	byte #%00000000
	byte #%00000000
	byte #%00000000
			
chicken_7
	byte #%00000000
	byte #%01000010
	byte #%00111100
	byte #%00011000
	byte #%00111100
	byte #%11011011
	byte #%00000000
		
chicken_8
	byte #%00000000
	byte #%01000010
	byte #%00111100
	byte #%00011000
	byte #%11111111
	byte #%00011000
	byte #%00000000
		
chicken_9
	byte #%00000000
	byte #%01000010
	byte #%00111100
	byte #%10011001
	byte #%01111110
	byte #%00011000
	byte #%00000000
	
no_chicken
	byte #%00000000
	byte #%00000000
	byte #%00000000
	byte #%00000000
	byte #%00000000
	byte #%00000000

aiming
	REPEAT 50
	byte #0
	REPEND
	byte #%00011100
	byte #%00101010
	byte #%01111111
	byte #%00101010
	byte #%00011100
	REPEAT 50
	byte #0
	REPEND

next_chicken_table
	byte #%00000000 ;0000
	byte #%00100000 ;0001
	byte #%00110000 ;0010
	byte #%00000000 ;0011
	byte #%01010000 ;0100
	byte #%01110000 ;0101
	byte #%01000000 ;0110
	byte #%01100000 ;0111
	byte #%10010000 ;1000
	byte #%10110000 ;1001
	byte #%10000000 ;1010
	byte #%10100000 ;1011
	byte #%11010000 ;1100
	byte #%11110000 ;1101
	byte #%11000000 ;1110
	byte #%11100000 ;1111

muntable
	.word mun0
	.word mun1
	.word mun2
	.word mun3
	.word mun4
	
mun4
	byte #%00000000
	byte #%11111110
	byte #%10101010
	byte #%10101010
	byte #%10101010
	byte #%11111110
	byte #%00000000	
mun3
	byte #%00000000
	byte #%11111110
	byte #%10101000
	byte #%10101000
	byte #%10101000
	byte #%11111110
	byte #%00000000	
mun2
	byte #%00000000
	byte #%11111110
	byte #%10100000
	byte #%10100000
	byte #%10100000
	byte #%11111110
	byte #%00000000	
mun1
	byte #%00000000
	byte #%11111110
	byte #%10000000
	byte #%10000000
	byte #%10000000
	byte #%11111110
	byte #%00000000	
mun0
	byte #%00000000
	byte #%11111110
	byte #%00000000
	byte #%00000000
	byte #%00000000
	byte #%11111110
	byte #%00000000
	
;---------------------------
	;Subroutines 
;---------------------------

Timing
	lda TIME2
	pha
	inc TIME1
	bne end_timing
	inc TIME2
	bne end_timing
	inc TIME3
	bne end_timing
end_timing
	lda TIME2
	cmp #%00011100
	bcc no_interuption
	lda #0
	sta RAND1
	sta RAND2
no_interuption
	pla
	sec
	sbc TIME2
	beq no_season_change
	jsr RandomBit
	lda RAND1
	and #%00000001
	sta SEASON
no_season_change
	rts

RandomBit
    	lda RAND2
	and #%00010000
	asl
	asl
	asl
	eor RAND2
	asl
	rol RAND1
	rol RAND2
	rts

	;Returns random number in A
RandomByte
	ldx #8
RandomByte1
    	jsr RandomBit
    	dex
    	bne RandomByte1
    	lda RAND1
    	rts
    
	;A = Score to add, already BCD
AddToScore
	sed
	clc
	adc SCORE3
	sta SCORE3
	bcc finished_score_inc
	clc
	lda #1
	adc SCORE2
	sta SCORE2
	bcc finished_score_inc
	clc
	lda #1
	adc SCORE1
	sta SCORE1
finished_score_inc
	cld
	rts
	
	;A = Decrease Score
SubstractFromScore
	ldx SCORE1
	bne here_we_go
	ldx SCORE2
	bne here_we_go
	ldx SCORE3
	bne here_we_go
	jmp finished_substraction
here_we_go
	sed
	lda SCORE3
	sec
	sbc #1
	sta SCORE3
	bcs finished_substraction
	sec
	lda SCORE2
	sbc #1
	sta SCORE2
	bcs finished_substraction
	sec
	lda SCORE1
	sbc #1
	sta SCORE1
finished_substraction
	cld
	rts

Initialisation
	ldx #30
	stx PLAYERY0
	ldx #50
	stx PLAYERX0
	lda #6
	sta NTSCBREAKER
	lda #$ff
	sta CHICKENHIT
	lda #1
	sta CROSSREADY
	sta SOUNDTABLESELECTOR
	lda #4
	sta AMUNITION
	
	;some stuff which needs to be zerod
	lda #0
	sta PISTOL
	sta GAMESTATUS
	sta JOYSTICKMOVEDUP
	sta JOYSTICKMOVEDDOWN
	sta SCORE1
	sta SCORE2
	sta SCORE3
	sta TIME1
	sta TIME2
	sta TIME3
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	sta SEASON
	rts
	
CheckControler
	lda INPT4
	and #%10000000
	bne not_fired1
	lda #1
	sta GAMESTATUS
	;disable Sound when enter a new game
	lda #0
	sta SOUNDTABLESELECTOR
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	sta CROSSREADY
	
not_fired1
	lda JOYSTICKMOVEDDOWN
	bne waitforreleasedown 
	
	lda SWCHA
	and #%00100000
	bne not_down
	lda PISTOL
	eor #%00000001
	sta PISTOL
	lda #1
	sta JOYSTICKMOVEDDOWN
	jmp finished_selection
waitforreleasedown
	lda SWCHA
	and #%00100000
	beq finished_selection
	lda #0
	sta JOYSTICKMOVEDDOWN	
	jmp finished_selection
	
not_down
	lda JOYSTICKMOVEDUP
	bne waitforreleaseup
	
	lda SWCHA
	and #%00010000
	bne finished_selection
	lda PISTOL
	eor #%00000001
	sta PISTOL
	lda #1
	sta JOYSTICKMOVEDUP
	jmp finished_selection
waitforreleaseup
	lda SWCHA
	and #%00010000
	beq finished_selection
	lda #0
	sta JOYSTICKMOVEDUP	

finished_selection	
	rts

LoadPistol
	lda PISTOL
	cpx #1
	beq normal_game2
	cpx #2
	beq cross_positioned2
	
pistol_collision_detection
	ldx SOUNDTABLESELECTOR
	cpx #2
	beq dont_interrupt_hit2
	cpx #3
	beq dont_interrupt_hit2
	cpx #5
	beq dont_interrupt_hit2
	cpx #6
	beq dont_interrupt_hit2
	;start shot sound
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #4
	sta SOUNDTABLESELECTOR
dont_interrupt_hit2

	;we shot, check mun
	ldx AMUNITION
	beq out_of_mun
	dex
	stx AMUNITION
	
	;we have enough mun, check what was hit
	ldx CHICKENHIT
	cpx #$ff
	beq no_chicken_was_hit3
	lda #$ff
	sta CHICKENHIT
	dex
	jmp we_hit_a_chicken
	
	;we are out of mun
out_of_mun
	ldx CHICKENHIT
	lda #$ff
	sta CHICKENHIT
	cpx #10
	beq reload_the_gun
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #6
	sta SOUNDTABLESELECTOR
no_chicken_was_hit3
	jmp no_chicken_was_hit
	
reload_the_gun
	lda #4
	sta AMUNITION
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #2
	sta SOUNDTABLESELECTOR
	jmp no_chicken_was_hit
	
	;to avoid out of range jumps
normal_game2
	jmp normal_game
cross_positioned2
	jmp cross_positioned

	;we hit a chicken at X
we_hit_a_chicken
	;maybe the munbox?
	cpx #9
	beq reload_the_gun
	
	txa
	asl
	asl
	tax
	lda HUHN_HEALTH_DIRECTION0,x
	asl
	asl
	asl
	asl
	cmp #4
	bcc no_chicken_was_hit
	;chicken is a valid target
	lda HUHN_HEALTH_DIRECTION0,x
	sta TEMPVAR1
	and #%00001111
	ora #%00010000
	sta HUHN_HEALTH_DIRECTION0,x
	lda TEMPVAR1
	and #%00001100
	lsr
	lsr
	pha
	lda TEMPVAR1
	and #%11110000
	cmp #%11000000
	bcs no_small_chicken1
	cmp #%10000000
	bcc no_small_chicken1
	pla
	tax
	inx
	txa
	pha
no_small_chicken1
	lda TEMPVAR1
	and #%00000001
	sec
	sbc SEASON
	beq add_some_points
	pla
	jsr SubstractFromScore
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #5
	sta SOUNDTABLESELECTOR
	jmp no_chicken_was_hit
add_some_points		
	pla
	jsr AddToScore
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #3
	sta SOUNDTABLESELECTOR
no_chicken_was_hit
	lda #1
	sta PISTOL
	jmp finished_loadpistol
	
cross_positioned
	lda #$ff
	sta CHICKENHIT
	lda Xpos
	sta PLAYERX0
	lda Ypos
	lsr
	sta PLAYERY0
	jsr finished_paddle0 ;correct if too much left, right, up or down
	lda #3
	sta PISTOL
	rts

normal_game
	lda #$ff
	sta CHICKENHIT
	lda SWCHA
	and #%00000001
	bne pistol_was_pressed
pistol_not_pressed
	lda #1
	sta CROSSREADY
	jmp finished_loadpistol
pistol_was_pressed
	lda CROSSREADY
	beq finished_loadpistol
	
	lda #0
	sta CROSSREADY
	lda PISTOL
	cpx #1
	bne finished_loadpistol
	lda #2
	sta PISTOL

finished_loadpistol
	ldx PLAYERY0
	stx PLAYERYTEMP0
	rts
	
LoadAiming
	lda INPT4
	and #%10000000
	bne no_fire_pressed2
	
	;fire pressed, but is cross ready?
	lda CROSSREADY
	beq fire_pressed2
	
	;cross is ready! Thest wether we can start normal shoot sound
	ldx SOUNDTABLESELECTOR
	cpx #2
	beq dont_interrupt_hit
	cpx #2
	beq dont_interrupt_hit
	cpx #3
	beq dont_interrupt_hit
	cpx #5
	beq dont_interrupt_hit
	;start shot sound
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #4
	sta SOUNDTABLESELECTOR
dont_interrupt_hit
	
	;dec munition
	ldy AMUNITION
	beq no_mun_anymore
	dey
	sty AMUNITION
	
	;test wether we hit something
	ldx CHICKENHIT
	cpx #$ff
	beq fire_pressed2
	dex
	jmp we_hit_at_x
	
no_mun_anymore
	ldx CHICKENHIT
	cpx #10
	beq reload_weapon
	
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #6
	sta SOUNDTABLESELECTOR
	jmp fire_pressed2
	
no_fire_pressed2
	jmp no_fire_pressed
	
we_hit_at_x
	;we hit a chicken at X
	cpx #9
	bne no_reload1
	;playeer wants to reload its weapon
reload_weapon
	lda #4
	sta AMUNITION
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #2
	sta SOUNDTABLESELECTOR
fire_pressed2
	jmp fire_pressed
	
no_reload1
	txa
	asl
	asl
	tax
	lda HUHN_HEALTH_DIRECTION0,x
	asl
	asl
	asl
	asl
	cmp #4
	bcc fire_pressed
	;chicken is a valid target
	lda HUHN_HEALTH_DIRECTION0,x
	sta TEMPVAR1
	and #%00001111
	ora #%00010000
	sta HUHN_HEALTH_DIRECTION0,x
	lda TEMPVAR1
	and #%00001100
	lsr
	lsr
	pha
	lda TEMPVAR1
	and #%11110000
	cmp #%11000000
	bcs no_small_chicken2
	cmp #%10000000
	bcc no_small_chicken2
	pla
	tax
	inx
	txa
	pha
no_small_chicken2
	lda TEMPVAR1
	and #%00000001
	sec
	sbc SEASON
	beq add_some_points2
	pla
	jsr SubstractFromScore
	;start hit sound
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #5
	sta SOUNDTABLESELECTOR
	jmp fire_pressed
add_some_points2	
	pla
	jsr AddToScore
	;start hit sound
	lda #0
	sta SOUNDDELAYCOUNTER
	sta SOUNDTABLEINDEX
	lda #3
	sta SOUNDTABLESELECTOR
	
fire_pressed
	lda #0
	sta CROSSREADY
	jmp start_moving
no_fire_pressed
	lda #1
	sta CROSSREADY

start_moving
	lda #$ff
	sta CHICKENHIT
	
	lda SWCHA
	and #%10000000
	bne to_left
	ldx PLAYERX0
	inx
	inx
	stx PLAYERX0
to_left
	lda SWCHA
	and #%01000000
	bne to_down
	ldx PLAYERX0
	dex
	dex
	stx PLAYERX0
to_down
	lda SWCHA
	and #%00100000
	bne to_up
	ldx PLAYERY0
	dex
	stx PLAYERY0
to_up	
	lda SWCHA
	and #%00010000
	bne finished_paddle0
	ldx PLAYERY0
	inx
	stx PLAYERY0
	
finished_paddle0
	lda PLAYERX0
	cmp #151
	bcc too_much_left
	ldx #150
	stx PLAYERX0
too_much_left
	cmp #2
	bcs too_much_up
	ldx #2
	stx PLAYERX0
too_much_up
	lda PLAYERY0
	cmp #49
	bcc too_much_down
	ldx #48
	stx PLAYERY0
too_much_down
	cmp #5
	bcs finished_aiming_correction
	ldx #5
	stx PLAYERY0
finished_aiming_correction
	;load Player Y Pos
	ldx PLAYERY0
	stx PLAYERYTEMP0
	rts

RandomTable
	byte #0
	byte #1
	byte #2
	byte #3
	byte #4
	byte #5
	byte #6
	byte #7
	byte #8
	byte #2
	byte #3
	byte #6
	byte #0
	byte #7
	byte #1
	byte #3

LoadChickens
	;check where to create a new chicken
	jsr RandomBit
	jsr RandomBit
	jsr RandomBit
	jsr RandomBit
	lda RAND1
	and #%00001111
	tax
	lda RandomTable,x
	asl
	asl
	tax
	lda HUHN_HEALTH_DIRECTION0,x
	bne end_chicken_creation
	txa
	pha
	;choose chicken
	jsr RandomBit
	jsr RandomBit
	lda RAND1
	and #%00000011
	clc
	ror
	ror
	ror
	sta TEMPVAR1
	;choose chicken speed 
	jsr RandomBit
	lda RAND1
	and #%00000001
	beq set_speed_1
set_speed_2
	lda #%00001000
	jmp speed_chosen
set_speed_1
	lda #%00000100
speed_chosen	
	sta TEMPVAR2
	;choose chicken health and direction
	jsr RandomBit
	jsr RandomBit
	lda RAND1
	and #%00000011
	ora TEMPVAR1
	ora TEMPVAR2
	tay
	pla
	tax
	sty HUHN_HEALTH_DIRECTION0,x
	tya
	and #%00000010
	bne start_left
start_right
	ldy #140
	sty HUHNX0,x
	jmp end_chicken_creation
start_left
	ldy #3
	sty HUHNX0,x
end_chicken_creation

	;load chicken image pointers
	ldx #32
loadchicken_loop
	lda HUHN_HEALTH_DIRECTION0,x
	lsr
	lsr
	lsr
	lsr
	asl
	tay
	lda no_chicken_offset,y
	sta HUHN_ADR_A0,x
	lda no_chicken_offset+1,y
	sta HUHN_ADR_B0,x
	txa
	beq end_loadchicken_loop
	sec
	sbc #4
	tax
	jmp loadchicken_loop

end_loadchicken_loop	
	rts
	
LoadScore
	ldx #0
loadscoreloop
	txa
	lsr
	lsr
	tax
	lda SCORE1,x
	pha
	txa
	asl
	asl
	tax
	pla
	lsr
	lsr
	lsr
	lsr
	asl
	tay
	lda digit_0_offset,y
	sta GRTABLE,x
	lda digit_0_offset+1,y
	sta GRTABLE+1,x	
	txa
	lsr
	lsr
	tax
	lda SCORE1,x
	pha
	txa
	asl
	asl
	tax
	pla
	and #%00001111
	asl
	tay
	lda digit_0_offset,y
	sta GRTABLE+2,x
	lda digit_0_offset+1,y
	sta GRTABLE+3,x
	inx
	inx
	inx
	inx
	cpx #12
	bne loadscoreloop
	rts

	;Move each chicken
MoveChickens
	ldx #0
move_chicken_loop
	lda HUHN_HEALTH_DIRECTION0,x
	beq next_chicken_plz
	ror
	ror
	and #%00000011
	tay
	bcs add_speed_to_chicken
	eor #%11111111
	tay
	iny
add_speed_to_chicken
	sty TEMPVAR1
	lda HUHNX0,x
	clc
	adc TEMPVAR1
	sta HUHNX0,x
	cmp #141
	bcs destroy_chicken
	cmp #3
	bcc destroy_chicken
	
	;chicken gets next frame
	lda HUHN_HEALTH_DIRECTION0,x
	pha
	lsr
	lsr
	lsr
	lsr
	beq destroy_chicken2
	tay
	pla
	and #%00001111
	ora next_chicken_table,y
	sta HUHN_HEALTH_DIRECTION0,x
	jmp next_chicken_plz
	
destroy_chicken2
	pla
destroy_chicken
	lda #0
	sta HUHN_HEALTH_DIRECTION0,x	
next_chicken_plz		
	clc
	txa
	adc #4
	tax
	cpx #36
	bne move_chicken_loop
	rts
	
LoadPistolName
	ldx char_p_offset
	stx GRTABLE
	ldx char_p_offset+1
	stx GRTABLE+1
	ldx char_i_offset
	stx GRTABLE+2
	ldx char_i_offset+1
	stx GRTABLE+3
	
	ldx char_s_offset
	stx GRTABLE+4
	ldx char_s_offset+1
	stx GRTABLE+5
	ldx char_t_offset
	stx GRTABLE+6
	ldx char_t_offset+1
	stx GRTABLE+7
	
	ldx char_o_offset
	stx GRTABLE+8
	ldx char_o_offset+1
	stx GRTABLE+9
	ldx char_l_offset
	stx GRTABLE+10
	ldx char_l_offset+1
	stx GRTABLE+11
	rts
	
LoadJoystickName
	ldx char_j_offset
	stx GRTABLE
	ldx char_j_offset+1
	stx GRTABLE+1
	ldx char_oy_offset
	stx GRTABLE+2
	ldx char_oy_offset+1
	stx GRTABLE+3
	
	ldx char_s_offset
	stx GRTABLE+4
	ldx char_s_offset+1
	stx GRTABLE+5
	ldx char_t_offset
	stx GRTABLE+6
	ldx char_t_offset+1
	stx GRTABLE+7
	
	ldx char_ic_offset
	stx GRTABLE+8
	ldx char_ic_offset+1
	stx GRTABLE+9
	ldx char_k_offset
	stx GRTABLE+10
	ldx char_k_offset+1
	stx GRTABLE+11
	rts
		
;---------------------------
	;Kernels 
;---------------------------
	
	;kernel for the start screen
Title	jsr Logo ;131
		
	ldx #10
title_loop1	
	sta WSYNC
	dex
	bne title_loop1 ;10
	
	jsr LoadJoystickName ;1
	lda PISTOL
	beq white_joystick
	lda #ORANGE
	jmp paint_joystick
white_joystick
	lda #WHITE
paint_joystick
	jsr Highreskernel ;21
	
	jsr LoadPistolName ;1
	lda PISTOL
	bne white_pistol
	lda #ORANGE
	jmp paint_pistol
white_pistol
	lda #WHITE
paint_pistol
	jsr Highreskernel ;21
	
	ldx #BLACK
	stx COLUBK	
	ldx #GRAPHICKERNEL-190
title_screen	
	sta WSYNC
	dex
	bne title_screen
	rts

	;kernel for the logo of the start screen	
Logo
	ldx #BLACK
	stx COLUBK
	ldx #ORANGE
	stx COLUPF
	
	sta WSYNC
	ldx #5		;[4] + 2
logo_loop2
	lda #%11000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%10000000	;[11] + 2
	sta PF1		;[13] + 3
	sleep 19	;[16] + 19
	lda #0		;[35] + 2
	sta PF0		;[37] + 3
	sta PF1		;[40] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop2	;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop3
	lda #%01000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%01000000	;[11] + 2
	sta PF1		;[13] + 3
	lda #%10000110	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00010000	;[33] + 2
	sta PF0		;[35] + 3
	sleep 11	;[38] + 11
	lda #0		;[49] + 2
	sta PF1		;[51] + 3
	sta PF2		;[54] + 3
	sta PF0		;[57] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop3  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop4
	lda #%01000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%01000000	;[11] + 2
	sta PF1		;[13] + 3
	lda #%01000101	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00010000	;[33] + 2
	sta PF0		;[35] + 3
	sleep 11	;[38] + 11
	lda #0		;[49] + 2
	sta PF1		;[51] + 3
	sta PF2		;[54] + 3
	sta PF0		;[57] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop4  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop5
	lda #%01000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%01000000	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00000100	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00010000	;[33] + 2
	sta PF0		;[35] + 3
	sleep 11	;[38] + 11
	lda #0		;[49] + 2
	sta PF1		;[51] + 3
	sta PF2		;[54] + 3
	sta PF0		;[57] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop5  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop6
	lda #%11000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%10000000	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00000100	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00010000	;[33] + 2
	sta PF0		;[35] + 3
	sleep 11	;[38] + 11
	lda #0		;[49] + 2
	sta PF1		;[51] + 3
	sta PF2		;[54] + 3
	sta PF0		;[57] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop6  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop7
	lda #%11000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%11000111	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00011100	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%01110000	;[33] + 2
	sta PF0		;[35] + 3
	sleep 11	;[38] + 11
	lda #0		;[49] + 2
	sta PF1		;[51] + 3
	sta PF2		;[54] + 3
	sta PF0		;[57] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop7  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop8
	lda #%01000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00101000	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00100101	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%10010000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%01000001	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #0		;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop8  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop9
	lda #%01000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00101000	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00100101	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%10010000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%00100010	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #0		;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop9  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop10
	lda #%01000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00101000	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00100101	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%10010000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%00010100	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #0		;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop10  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop11
	lda #%11000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%11000111	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00011100	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%01110000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%00001000	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #0		;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop11  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop12
	lda #0		;[6] + 2
	sta PF0		;[8] + 3
	sta PF1		;[11] + 8
	sleep 21
	lda #%00010000
	sta PF1
	sta WSYNC
	dex
	bne logo_loop12
	
	ldx #5		;[4] + 2
logo_loop13
	lda #%11000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%11111111	;[11] + 2
	sta PF1		;[13] + 3
	lda #%11111111	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%11110000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%00100000	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #0		;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop13 ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop14
	lda #%00100000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #0		;[11] + 2
	sta PF1		;[13] + 3
	sleep 12	;[16] + 12
	lda #0		;[38] + 2
	sta PF0		;[40] + 3
	lda #%10100000	;[43] + 2
	sta PF1		;[45] + 3
	sta WSYNC
	dex
	bne logo_loop14
	
	ldx #5		;[4] + 2
logo_loop15
	lda #%10000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%11000100	;[11] + 2
	sta PF1		;[13] + 3
	sleep 14	;[16] + 14
	lda #%0		;[40] + 2
	sta PF0		;[42] + 3
	lda #%01000000	;[45] + 2
	sta PF1		;[47] + 3
	sta WSYNC
	dex
	bne logo_loop15
	
	ldx #5		;[4] + 2
logo_loop16
	lda #%01000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00101010	;[11] + 2
	sta PF1		;[13] + 3
	sleep 14	;[16] + 14
	lda #%0		;[40] + 2
	sta PF0		;[42] + 3
	sta PF1		;[45] + 3
	sta WSYNC
	dex
	bne logo_loop16
	 
	ldx #5		;[4] + 2
logo_loop17
	lda #%01000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00000010	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00000110	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00000000	;[33] + 2
	sta PF0		;[35] + 3
	sta PF1		;[38] + 3
	sta PF2		;[41] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop17 ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop18
	lda #%01010000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00000010	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00001000	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00000000	;[33] + 2
	sta PF0		;[35] + 3
	sta PF1		;[38] + 3
	sta PF2		;[41] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop18 ;[2] + 2/3
	
	
	ldx #5		;[4] + 2
logo_loop19
	lda #%10000000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%11000010	;[11] + 2
	sta PF1		;[13] + 3
	lda #%00001000	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00000000	;[33] + 2
	sta PF0		;[35] + 3
	sta PF1		;[38] + 3
	sta PF2		;[41] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop19 ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop20
	lda #%00010000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00100010	;[11] + 2
	sta PF1		;[13] + 3
	lda #%10010011	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%10100000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%00011001	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #%10001010	;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop20  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop21
	lda #%00010000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00100011	;[11] + 2
	sta PF1		;[13] + 3
	lda #%10010100	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%01100000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%10100101	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #%10001001	;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop21  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop22
	lda #%01010000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%00100011	;[11] + 2
	sta PF1		;[13] + 3
	lda #%10010100	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00100000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%10100101	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #%01010000	;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop22  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop23
	lda #%10010000  ;[6] + 2
	sta PF0		;[8] + 3
	lda #%11000010	;[11] + 2
	sta PF1		;[13] + 3
	lda #%01100100	;[16] + 2
	sta PF2		;[18] + 3
	sleep 12	;[21] + 12
	lda #%00100000	;[33] + 2
	sta PF0		;[35] + 3		
	lda #%10011101	;[38] + 2
	sta PF1		;[40] + 3
	sleep 10	;[43] + 10
	lda #%01010000	;[53] + 2 
	sta PF2		;[55] + 3
	sta WSYNC
	dex		;[0] + 2
	bne logo_loop23  ;[2] + 2/3
	
	ldx #5		;[4] + 2
logo_loop24
	lda #0
	sta PF0
	sta PF1
	sta PF2		;[14] + 3
	lda #%00000100  ;[17] + 2
	sleep 31
	sta PF1
	lda #%00100000
	sta PF2
	sta WSYNC
	dex
	bne logo_loop24
	
	ldx #5		;[4] + 2
logo_loop25
	lda #0
	sta PF0
	lda #%00000100
	sta PF1		
	lda #%00000000  
	sta PF2
	lda #%00000100
	sleep 21	;[21] + 29
	sta PF1
	lda #%00010000
	sta PF2
	sta WSYNC
	dex
	bne logo_loop25
	
	ldx #5		;[4] + 2
logo_loop26
	lda #0
	sta PF0
	lda #%00000011
	sta PF1		
	lda #%11111111  
	sta PF2
	lda #%11110000
	sleep 21	;[21] + 19
	sta PF0
	lda #%11111000
	sta PF1		;[47] + 3
	lda #%00001000
	sta PF2
	sta WSYNC
	dex
	bne logo_loop26
	
	ldx #5		;[4] + 2
logo_loop27
	lda #%00010000
	sta PF0
	lda #0
	sta PF1
	sta PF2
	sleep 7		;[19] + 7
	lda #0
	sta PF0
	sleep 18	;[31] + 18
	lda #%00001000
	sta PF2
	sta WSYNC
	dex
	bne logo_loop27
	
	ldx #5
logo_loop28
	lda #255
	sta PF0
	sta PF1
	sta PF2
	sleep 36	;[16] + 36
	lda #%00000111
	sta PF2
	sta WSYNC
	dex
	bne logo_loop28

	lda #0
	sta PF0
	sta PF1
	sta PF2
	rts
	
	;complete graphic kernel for a running game
Game
	sta WSYNC
	ldx #LIGHTBLUE
	stx COLUBK
	lda #BLACK
	jsr Highreskernel

	ldx #0
	lda PLAYERX0
	jsr PositionASpriteSubroutine
	sta PLAYERHMOVE0
	ldx #5
	stx NUSIZ1
	ldx #0
	stx NUSIZ0
	stx CXCLR
	;set color of first chicki and aiming cross
	lda HUHN_HEALTH_DIRECTION0
	and #%00000001
	tax
	lda chicken_color_table,x
	sta COLUP1
	ldx SEASON
	lda chicken_color_table,x
	sta COLUP0
	sta WSYNC
	sleep 6	
	
	;scanline 62
	;25 scanlines of kernal already used	
	lda #0
	jsr Chickenkernel	
	lda #1
	jsr Chickenkernel
	lda #2
	jsr Chickenkernel
	lda #3
	jsr Chickenkernel
	lda #4
	jsr Chickenkernel
	lda #5
	jsr Chickenkernel
	lda #6
	jsr Chickenkernel
	lda #7
	jsr Chickenkernel
	lda #8
	jsr Chickenkernel
	lda #9
	jsr Amunitionkernel
	
	ldx #0
	stx GRP0
	
	;check wether last chicken met aiming cross
	lda CXPPMM
	and #%10000000
	beq no_p1p0_collision2
	lda #10
	sta CHICKENHIT
	sta CXCLR
no_p1p0_collision2
	
	ldx #LIGHTBLUE
	stx COLUBK	
	ldx #GRAPHICKERNEL-192
	beq chickens_gone 
main_screen	
	sta WSYNC
	dex
	bne main_screen
	

chickens_gone 
	jsr SunKernelSelection
	rts
	
	;takes 10 cycles
	;Position Ammunition
Amunitionkernel
	ldx PLAYERYTEMP0
	ldy aiming,x
	sty GRP0
	inx
	stx PLAYERYTEMP0
	pha
	
	lda AMUNITION
	asl
	tax
	lda muntable,x
	sta TEMPVAR1
	inx
	lda muntable,x
	sta TEMPVAR2
	lda #72
	sta HMCLR
	
	ldx #1
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine
	ldx PLAYERYTEMP0
	ldy aiming,x
	sty GRP0
	inx
	stx PLAYERYTEMP0
	lda PLAYERHMOVE0
	beq no_p0_finetune1
	sta HMP0
no_p0_finetune1
	lda #0
	sta PLAYERHMOVE0
	
	;check wether last chicken met aiming cross
	lda CXPPMM
	and #%10000000
	beq no_p1p0_collision1
	pla
	sta CHICKENHIT
	sta CXCLR
	pha
no_p1p0_collision1
	pla
	lda #BLACK
	sta COLUP1	
	sta WSYNC
	sta HMOVE
	
	ldy #6
	ldx PLAYERYTEMP0
ck1_loop11
	lda (TEMPVAR1),y
	sta GRP1
	sta WSYNC
	dey
	lda (TEMPVAR1),y
	sta GRP1
	lda aiming,x
	sta GRP0
	inx
	sta WSYNC
	dey
	bne ck1_loop11
	lda #0
	sta GRP1
	stx PLAYERYTEMP0
no_color_set2
	sta WSYNC
	rts
	
	;takes 10 cycles
	;chicken = A
Chickenkernel
	ldx PLAYERYTEMP0
	ldy aiming,x
	sty GRP0
	inx
	stx PLAYERYTEMP0
	pha
	
	asl
	asl
	tax
	lda HUHN_ADR_A0,x
	sta TEMPVAR1
	lda HUHN_ADR_B0,x
	sta TEMPVAR2
	lda HUHNX0,x
	sta HMCLR
	
	ldx #1
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine
	ldx PLAYERYTEMP0
	ldy aiming,x
	sty GRP0
	inx
	stx PLAYERYTEMP0
	lda PLAYERHMOVE0
	beq no_p0_finetune
	sta HMP0
no_p0_finetune
	lda #0
	sta PLAYERHMOVE0
	
	;check wether last chicken met aiming cross
	lda CXPPMM
	and #%10000000
	beq no_p1p0_collision
	pla
	sta CHICKENHIT
	sta CXCLR
	pha
no_p1p0_collision
		
	sta WSYNC
	sta HMOVE
	
	ldy #6
	ldx PLAYERYTEMP0
ck1_loop1
	lda (TEMPVAR1),y
	sta GRP1
	sta WSYNC
	dey
	lda (TEMPVAR1),y
	sta GRP1
	lda aiming,x
	sta GRP0
	inx
	sta WSYNC
	dey
	bne ck1_loop1
	lda #0
	sta GRP1
	stx PLAYERYTEMP0
	
	;set next chicken color
	pla
	cmp #9 ;last chicken?
	beq no_color_set
	clc
	adc #1
	asl
	asl
	tax
	lda HUHN_HEALTH_DIRECTION0,x
	and #%00000001
	tax
	lda chicken_color_table,x
	sta COLUP1
no_color_set

	sta WSYNC
	rts
	
chicken_color_table
	byte #RED
	byte #DARKGREEN

	ALIGN   256
	
GetGun_Kernel
                lda     #160            ; timer for 135 scanlines
                sta     TIM64T  
                lda     #$00    
                sta     WSYNC   
                sta     GRP0    
                sta     GRP1    
                sta     Xpos            ; reset coordinates to no hit on screen
                sta     Ypos
                ldx     #123            ; counter for hit scanline
		lda     #$2e            ; bright yellow(PAL) / orange(NTSC)
                sta     COLUBK 
                sta     WSYNC   
                bit     INPT3
                nop
                nop            
                nop            
                nop            
                nop            
CoarseTest:     dex            
                beq     CEndRelay       ; test is over with no hit detected
                nop            
                nop            
                nop            
                nop            
                nop            
                nop            
                nop            
                bit     INPT5           ; test lightgun sensor
                bpl     HitX01to03      ; did we hit within the last 5 cycles
                bit     INPT5   
                bpl     HitX04to08   
                bit     INPT5   
                bpl     HitX09to13   
                bit     INPT5   
                bpl     HitX14to18   
                bit     INPT5   
                bpl     HitX19to23   
                bit     INPT5   
                bpl     HitX24to28   
                bit     INPT5   
                bpl     HitX29to33   
                bit     INPT5   
                bpl     HitX34to38   
                bit     INPT5   
                bpl     HitX39to43   
                bit     INPT5   
                bpl     HitX44to48   
                bit     INPT5   
                bpl     HitX49to53   
                jmp     CoarseTest
CEndRelay:      jmp     CEndFrame       ; test is over with no hit detected

HitX49to53:     lda     #158            ; hit was at pixel 158 or before
                bne     AdjustHpos   
HitX44to48:     lda     #143    
                bne     AdjustHpos   
HitX39to43:     lda     #128    
                bne     AdjustHpos   
HitX34to38:     lda     #113    
                bne     AdjustHpos   
HitX29to33:     lda     #98     
                bne     AdjustHpos   
HitX24to28:     lda     #83     
                bne     AdjustHpos   
HitX19to23:     lda     #68     
                bne     AdjustHpos   
HitX14to18:     lda     #53     
                bne     AdjustHpos   
HitX09to13:     lda     #38     
                bne     AdjustHpos   
HitX04to08:     lda     #23     
                bne     AdjustHpos   
HitX01to03:     lda     #8      
                bit     INPT5   

AdjustHpos:     sta     Xpos
                stx     Ypos     
                ldy     #4              ; 5 lines for adjusting horz. hit pos.
HitCycleAdjust: ldx     #9    
HCADelay:       dex            
                bpl     HCADelay        ; delay between lines is 75 cycles
                nop            
                bit     INPT5   
                bit     INPT5           ; happens 1 cycle earlier in each line
                bmi     CEndFrame       ; if no hit detected anymore ...
                sec            
                sbc     #3              ; ... hit was 3 pixel (1 cycle) earlier
                sta     Xpos     
                nop            
                dey            
                bpl     HitCycleAdjust

CEndFrame:      lda     INTIM
                bne     CEndFrame
		;135 scanlines already used
		sta WSYNC
		
		ldx #GRAPHICKERNEL-135
finished_blank_loop
		sta WSYNC
		dex
		bne finished_blank_loop
		lda #0
		sta COLUBK
                rts	

;-------------------------------------
	;No page boundary Crossing plz
;-------------------------------------

	ORG $1F00
	
PositionASpriteSubroutine ;taken from vdub_bobby

	sec            ;doing this before so that I have more time
                       ;during the next scanline.
	sta WSYNC      ;begin line 1

DivideLoop
	sbc #15
	bcs DivideLoop                 ;+4/5 4/9.../54
	tay                            ;+2 6
	lda FineAdjustTableEnd,Y       ;+5 11
	nop
	nop            ;+4     15/20/etc.   - 4 free cycles!
	sta HMP0,X     ;+4     19/24/...
	sta RESP0,X    ;+4     23/28/33/38/43/48/53/58/63/68/73
	sta WSYNC
	rts            ;+6      9
	
FineAdjustTableBegin

	byte #%01100000 ;left 6
	byte #%01010000
	byte #%01000000
	byte #%00110000
	byte #%00100000
	byte #%00010000
	byte #%00000000 ;left/right 0
	byte #%11110000
	byte #%11100000
	byte #%11010000
	byte #%11000000
	byte #%10110000
	byte #%10100000
	byte #%10010000
	byte #%10000000 ;right 8

FineAdjustTableEnd = FineAdjustTableBegin - 241
	
	;takes 21 WSYNCS
	;colour of text is in A	
Highreskernel
	sta WSYNC		; [0]
	ldy #16			; [0] + 2
	sty ROW			; [2] + 3
	; Set Delay and Three Copies
	ldy #3			; [5] + 2
	sty VDELP0		; [7] + 3
	sty VDELP1		; [10] + 3
	sty NUSIZ0		; [13] + 3
	sty NUSIZ1		; [16] + 3
	; Set Colour
	tay			; [19] + 2
	sty COLUP0		; [21] + 3
	sty COLUP1		; [24] + 3
	; Clear Any Reflections
	ldy #0			; [27] + 2
	sty REFP0		; [29] + 3
	sty REFP1		; [32] + 3
	; Position Sprites
	SLEEP 3			; [35] + 3
	sty RESP0		; [38] + 3
	sty RESP1		; [41] + 3
	; Fine Tune Position of P1
	SLEEP 5			; [44] + 5
	lda #$20		; [49] + 2	Shift P1 Left By 1 Pixel
	sta HMP1		; [51] + 3
	lda #%00010000		; [54] + 2
	sta HMP0		; [56] + 3
	sta WSYNC		; go
	sta HMOVE		; [0] + 3
	sleep 59		; [3] + 9
ScoreLoop
	; Fetch Current Line
	lda ROW			; [62] + 3
	lsr			; [65] + 2	2LK
	tay			; [67] + 2
	; Display First 3 Digits
	lda (GRTABLE),Y		; [69] + 5
	sta GRP0		; [74] + 3	> 54
	lda (GRTABLE+2),Y	; [1] + 5
	sta GRP1		; [6] + 3	< 42
	lda (GRTABLE+4),Y	; [9] + 5
	sta GRP0		; [14] + 3	< 44
	; Pre-fetch Remaining 3 Digits
	lda (GRTABLE+6),Y	; [17] + 5
	tax			; [22] + 2
	lda (GRTABLE+8),Y	; [24] + 5
	sta TEMPVAR1		; [29] + 3
	lda (GRTABLE+10),Y	; [32] + 5
	tay			; [37] + 2
	lda TEMPVAR1		; [39] + 3
	; Display Remaining 3 Digits
	stx GRP1		; [42] + 3	> 44 < 47
	sta GRP0		; [45] + 3	> 46 < 50
	sty GRP1		; [48] + 3	> 49 < 52
	sta GRP0		; [51] + 3	> 52 < 55
	; Update Counter
	dec ROW			; [54] + 5
	bpl ScoreLoop		; [59] + 2/3
	; Clear Everything
 	lda #0			; [61] + 2
	sta GRP0		; [63] + 3
	sta GRP1		; [66] + 3
	sta GRP0		; [69] + 3
 	sta VDELP0		; [72] + 3
	sta VDELP1		; [75] + 3
	sta WSYNC
	rts
EndHighReskernel
	if (>Highreskernel != >EndHighReskernel)
          echo "WARNING: Score Kernel crosses a page boundary!"
        endif

;-------------------------------------
	;Bankswitch Routines
;-------------------------------------
	ORG $1FB0
Start_Game
	jmp Reset

SunKernelSelection
	ldx $1FF5 ;go to bank 2
	nop
	nop
	nop
	nop
	nop
	nop
	rts ;1fbc
	
LaunchSoundEngine
	ldx $1FF6 ;go to bank 3 1fbd
	nop ;1fc0
	nop
	nop
	nop
	nop
	nop
	rts

	ORG $1FFA

	.word Reset          ; NMI
	.word Reset          ; RESET
	.word Reset          ; IRQ
	
;-------------------------------------
	;Bank 2
;-------------------------------------
	ORG $2000
	RORG $1000
Bank2
	jmp Return_Bank1_Start
	
SunKernels
	sta WSYNC
	lda TIME2
	
	cmp #%00010110
	bcc not_kernel_4
	jsr Sunkernel_4
	rts
not_kernel_4
	cmp #%00010011
	bcc not_kernel_3
	jsr Sunkernel_3
	rts
not_kernel_3
	cmp #%00001101
	bcc not_kernel_2
	jsr Sunkernel_2
	rts
not_kernel_2
	cmp #%00000111
	bcc not_kernel_1
	jsr Sunkernel_1
	rts
not_kernel_1
	jsr Sunkernel_0
	rts
	
;takes 64+4 WSYNCS
Sunkernel_4
	lda #8
	ldx #0
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	
	lda #30
	ldx #1
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	sta WSYNC
	sta HMOVE
	
	ldx #GREEN
	stx COLUP0
	stx COLUP1
	ldx #%00000101
	stx NUSIZ0
	stx NUSIZ1
	
	ldx #1
	stx CTRLPF
	sta WSYNC
	
	ldy #5
loop1
	ldx #DARKBROWN
	stx COLUPF
	ldx #%00110000
	stx PF0
	sleep 4
	ldx #LIGHTBLUE
	stx COLUBK
	sta WSYNC
	dey
	bne loop1
	
	ldy #3
loop2
	ldx #%01110000
	stx PF0
	sta WSYNC
	dey
	bne loop2
	
	ldy #3
loop3
	ldx #%11110000
	stx PF0
	ldx #%10000000
	stx PF1
	sta WSYNC
	dey
	bne loop3
	
	ldy #9
loop4
	lda #%11000000
	sta PF1
	sta WSYNC
	inx
	dey
	bne loop4
	
	ldy #3
	ldx #0
loop5
	lda #%11100000
	sta PF1
	lda tree_1,x
	sta GRP0
	sta WSYNC
	inx 
	dey
	bne loop5
	
	ldy #6
loop6
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	sta WSYNC
	inx
	dey
	bne loop6
	
	ldy #3
loop7
	ldx #%11111100
	stx PF1
	ldx #%11000000	
	stx PF2
	sleep 16
	ldx #YELL
	stx COLUPF
	sleep 12
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop7
	
	ldy #3
loop8
	ldx #%11111110
	stx PF1
	sleep 24
	ldx #YELL
	stx COLUPF
	sleep 10
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop8
	
	ldy #3
loop9
	ldx #%11111111
	stx PF1
	sleep 20
	ldx #%11100000
	stx PF2
	ldx #YELL
	stx COLUPF
	sleep 10
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop9
	
	ldy #3
loop10
	sleep 24
	ldx #%11110000
	stx PF2
	ldx #YELL
	stx COLUPF
	sleep 12
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop10
	
	ldy #3
loop11
	sleep 24
	ldx #%11110001
	stx PF2
	sleep 2
	ldx #YELL
	stx COLUPF
	sleep 10
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop11
	
	ldy #3
loop12
	ldx #DARKBLUE
	stx COLUBK
	sleep 8
	ldx #%11110011
	stx PF2
	sleep 14
	ldx #ORANGE
	stx COLUPF
	sleep 8
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop12

	ldx #%11100011
	stx PF2
	sleep 28
	ldx #ORANGE
	stx COLUPF
	sleep 8
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC

	ldy #2
	ldx #0
loop13
	lda #%11000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 22
	lda #ORANGE
	sta COLUPF
	sleep 6
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop13
	
	sleep 14
	lda #%10000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 10
	lda #ORANGE
	sta COLUPF
	sleep 6
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx

	ldy #6
loop14
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sta WSYNC
	inx
	dey
	bne loop14	

	ldy #3
loop15
	ldx #%00000111
	stx PF2
	sta WSYNC
	dey
	bne loop15
	
	ldx #BLACK
	stx COLUBK		
	ldx #0
	stx PF0
	stx PF1
	stx PF2
	stx CTRLPF
	rts

	;takes 64+4 WSYNCS
Sunkernel_0
	lda #8
	ldx #0
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	
	lda #30
	ldx #1
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	sta WSYNC
	sta HMOVE
	
	ldx #GREEN
	stx COLUP0
	stx COLUP1
	ldx #%00000101
	stx NUSIZ0
	stx NUSIZ1
	
	ldx #1
	stx CTRLPF
	sta WSYNC
	
	ldy #5
loop1_5
	ldx #DARKBROWN
	stx COLUPF
	ldx #%00110000
	stx PF0
	sleep 4
	ldx #LIGHTBLUE
	stx COLUBK
	sta WSYNC
	dey
	bne loop1_5
	
	ldy #3
loop2_5
	ldx #%01110000
	stx PF0
	sta WSYNC
	dey
	bne loop2_5
	
	ldy #3
loop3_5
	ldx #%11110000
	stx PF0
	ldx #%10000000
	stx PF1
	sta WSYNC
	dey
	bne loop3_5
	
	ldy #3
loop4_5a
	lda #%11000000
	sta PF1
	sta WSYNC
	inx
	dey
	bne loop4_5a
	
	ldy #3
loop4_5b
	lda #%11000000
	sta PF1
	lda #%10000000
	sta PF2
	sleep 10
	lda #YELL
	sta COLUPF
	sleep 15
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop4_5b
	
	ldy #3
loop4_5c
	lda #%11000000
	sta PF1
	lda #%11000000
	sta PF2
	sleep 10
	lda #YELL
	sta COLUPF
	sleep 15
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop4_5c
	
	ldy #3
	ldx #0
loop5_5
	lda #%11100000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11100000
	sta PF2
	sleep 13
	lda #YELL
	sta COLUPF
	sleep 10
	lda #DARKBROWN
	sta COLUPF 
	sta WSYNC
	inx 
	dey
	bne loop5_5
	
	ldy #3
loop6a_5
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11110000
	sta PF2
	sleep 6
	lda #YELL
	sta COLUPF
	sleep 16
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop6a_5
	
	ldy #3
loop6b_5
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11100000
	sta PF2
	sleep 6
	lda #YELL
	sta COLUPF
	sleep 16
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop6b_5
	
	ldy #3
loop7_5
	ldx #%11111100
	stx PF1
	ldx #%11000000	
	stx PF2
	sleep 16
	ldx #YELL
	stx COLUPF
	sleep 14
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop7_5
	
	ldy #3
loop8_5
	ldx #%11111110
	stx PF1
	ldx #%10000000
	stx PF2
	sleep 20
	ldx #YELL
	stx COLUPF
	sleep 12
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop8_5
	
	ldy #3
loop9_5a
	ldx #%11111111
	stx PF1
	lda #0
	sta PF2
	sta WSYNC
	dey
	bne loop9_5a
	
	ldy #3
loop9_5b
	ldx #%11111111
	stx PF1
	lda #0
	sta PF2
	sta WSYNC
	dey
	bne loop9_5b
	
	ldy #3
loop11_5
	sleep 24
	ldx #%00000001
	stx PF2
	sleep 17
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop11_5
	
	ldy #3
loop12_5
	ldx #DARKBLUE
	stx COLUBK
	sleep 8
	ldx #%00000011
	stx PF2
	sleep 27
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop12_5

	ldx #%00000011
	stx PF2
	sleep 28
	ldx #ORANGE
	stx COLUPF
	sleep 8
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC

	ldy #2
	ldx #0
loop13_5
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 22
	lda #ORANGE
	sta COLUPF
	sleep 4
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop13_5
	
	sleep 14
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 10
	lda #ORANGE
	sta COLUPF
	sleep 4
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx

	ldy #6
loop14_5
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sta WSYNC
	inx
	dey
	bne loop14_5

	ldy #3
loop15_5
	ldx #%00000111
	stx PF2
	sta WSYNC
	dey
	bne loop15_5
	
	ldx #BLACK
	stx COLUBK		
	ldx #0
	stx PF0
	stx PF1
	stx PF2
	stx CTRLPF
	rts
	
	;takes 64+4 WSYNCS
Sunkernel_1
	lda #8
	ldx #0
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	
	lda #30
	ldx #1
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	sta WSYNC
	sta HMOVE
	
	ldx #GREEN
	stx COLUP0
	stx COLUP1
	ldx #%00000101
	stx NUSIZ0
	stx NUSIZ1
	
	ldx #1
	stx CTRLPF
	sta WSYNC
	
	ldy #5
loop1_4
	ldx #DARKBROWN
	stx COLUPF
	ldx #%00110000
	stx PF0
	sleep 4
	ldx #LIGHTBLUE
	stx COLUBK
	sta WSYNC
	dey
	bne loop1_4
	
	ldy #3
loop2_4
	ldx #%01110000
	stx PF0
	sta WSYNC
	dey
	bne loop2_4
	
	ldy #3
loop3_4
	ldx #%11110000
	stx PF0
	ldx #%10000000
	stx PF1
	sta WSYNC
	dey
	bne loop3_4
	
	ldy #6
loop4_4a
	lda #%11000000
	sta PF1
	sta WSYNC
	inx
	dey
	bne loop4_4a
	
	ldy #3
loop4_4b
	lda #%11000000
	sta PF1
	lda #%10000000
	sta PF2
	sleep 10
	lda #YELL
	sta COLUPF
	sleep 15
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop4_4b
	
	ldy #3
	ldx #0
loop5_4
	lda #%11100000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11000000
	sta PF2
	sleep 14
	lda #YELL
	sta COLUPF
	sleep 10
	lda #DARKBROWN
	sta COLUPF 
	sta WSYNC
	inx 
	dey
	bne loop5_4
	
	ldy #3
loop6a_4
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11100000
	sta PF2
	sleep 6
	lda #YELL
	sta COLUPF
	sleep 16
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop6a_4
	
	ldy #3
loop6b_4
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11110000
	sta PF2
	sleep 6
	lda #YELL
	sta COLUPF
	sleep 16
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop6b_4
	
	ldy #3
loop7_4
	ldx #%11111100
	stx PF1
	ldx #%11100000	
	stx PF2
	sleep 16
	ldx #YELL
	stx COLUPF
	sleep 14
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop7_4
	
	ldy #3
loop8_4
	ldx #%11111110
	stx PF1
	ldx #%11000000
	stx PF2
	sleep 20
	ldx #YELL
	stx COLUPF
	sleep 12
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop8_4
	
	ldy #3
loop9_4b
	ldx #%11111111
	stx PF1
	sleep 20
	ldx #%10000000
	stx PF2
	ldx #YELL
	stx COLUPF
	sleep 10
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop9_4b
	
	ldy #3
loop9_4a
	ldx #%11111111
	stx PF1
	lda #0
	sta PF2
	sta WSYNC
	dey
	bne loop9_4a
	
	ldy #3
loop11_4
	sleep 24
	ldx #%00000001
	stx PF2
	sleep 17
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop11_4
	
	ldy #3
loop12_4
	ldx #DARKBLUE
	stx COLUBK
	sleep 8
	ldx #%00000011
	stx PF2
	sleep 27
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop12_4

	ldx #%00000011
	stx PF2
	sleep 28
	ldx #ORANGE
	stx COLUPF
	sleep 8
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC

	ldy #2
	ldx #0
loop13_4
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 22
	lda #ORANGE
	sta COLUPF
	sleep 4
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop13_4
	
	sleep 14
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 10
	lda #ORANGE
	sta COLUPF
	sleep 4
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx

	ldy #6
loop14_4
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sta WSYNC
	inx
	dey
	bne loop14_4

	ldy #3
loop15_4
	ldx #%00000111
	stx PF2
	sta WSYNC
	dey
	bne loop15_4
	
	ldx #BLACK
	stx COLUBK		
	ldx #0
	stx PF0
	stx PF1
	stx PF2
	stx CTRLPF
	rts
	
;takes 64+4 WSYNCS
Sunkernel_2
	lda #8
	ldx #0
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	
	lda #30
	ldx #1
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	sta WSYNC
	sta HMOVE
	
	ldx #GREEN
	stx COLUP0
	stx COLUP1
	ldx #%00000101
	stx NUSIZ0
	stx NUSIZ1
	
	ldx #1
	stx CTRLPF
	sta WSYNC
	
	ldy #5
loop1_3
	ldx #DARKBROWN
	stx COLUPF
	ldx #%00110000
	stx PF0
	sleep 4
	ldx #LIGHTBLUE
	stx COLUBK
	sta WSYNC
	dey
	bne loop1_3
	
	ldy #3
loop2_3
	ldx #%01110000
	stx PF0
	sta WSYNC
	dey
	bne loop2_3
	
	ldy #3
loop3_3
	ldx #%11110000
	stx PF0
	ldx #%10000000
	stx PF1
	sta WSYNC
	dey
	bne loop3_3
	
	ldy #9
loop4_3
	lda #%11000000
	sta PF1
	sta WSYNC
	inx
	dey
	bne loop4_3
	
	ldy #3
	ldx #0
loop5_3
	lda #%11100000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%10000000
	sta PF2
	sleep 15
	lda #YELL
	sta COLUPF
	sleep 10
	lda #DARKBROWN
	sta COLUPF 
	sta WSYNC
	inx 
	dey
	bne loop5_3
	
	ldy #3
loop6a_3
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11000000
	sta PF2
	sleep 6
	lda #YELL
	sta COLUPF
	sleep 16
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop6a_3
	
	ldy #3
loop6b_3
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11100000
	sta PF2
	sleep 6
	lda #YELL
	sta COLUPF
	sleep 16
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop6b_3
	
	ldy #3
loop7_3
	ldx #%11111100
	stx PF1
	ldx #%11110000	
	stx PF2
	sleep 16
	ldx #YELL
	stx COLUPF
	sleep 14
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop7_3
	
	ldy #3
loop8_3
	ldx #%11111110
	stx PF1
	ldx #%11100000
	stx PF2
	sleep 20
	ldx #YELL
	stx COLUPF
	sleep 12
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop8_3
	
	ldy #3
loop9_3
	ldx #%11111111
	stx PF1
	sleep 20
	ldx #%11000000
	stx PF2
	ldx #YELL
	stx COLUPF
	sleep 10
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop9_3
	
	ldy #3
loop10_3
	sleep 24
	ldx #%10000000
	stx PF2
	ldx #YELL
	stx COLUPF
	sleep 12
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop10_3
	
	ldy #3
loop11_3
	sleep 24
	ldx #%00000001
	stx PF2
	sleep 17
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop11_3
	
	ldy #3
loop12_3
	ldx #DARKBLUE
	stx COLUBK
	sleep 8
	ldx #%00000011
	stx PF2
	sleep 27
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop12_3

	ldx #%00000011
	stx PF2
	sleep 28
	ldx #ORANGE
	stx COLUPF
	sleep 8
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC

	ldy #2
	ldx #0
loop13_3
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 22
	lda #ORANGE
	sta COLUPF
	sleep 4
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop13_3
	
	sleep 14
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 10
	lda #ORANGE
	sta COLUPF
	sleep 4
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx

	ldy #6
loop14_3
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sta WSYNC
	inx
	dey
	bne loop14_3

	ldy #3
loop15_3
	ldx #%00000111
	stx PF2
	sta WSYNC
	dey
	bne loop15_3
	
	ldx #BLACK
	stx COLUBK		
	ldx #0
	stx PF0
	stx PF1
	stx PF2
	stx CTRLPF
	rts
	
;takes 64+4 WSYNCS
Sunkernel_3
	lda #8
	ldx #0
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	
	lda #30
	ldx #1
	;1 WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine2
	sta WSYNC
	sta HMOVE
	
	ldx #GREEN
	stx COLUP0
	stx COLUP1
	ldx #%00000101
	stx NUSIZ0
	stx NUSIZ1
	
	ldx #1
	stx CTRLPF
	sta WSYNC
	
	ldy #5
loop1_2
	ldx #DARKBROWN
	stx COLUPF
	ldx #%00110000
	stx PF0
	sleep 4
	ldx #LIGHTBLUE
	stx COLUBK
	sta WSYNC
	dey
	bne loop1_2
	
	ldy #3
loop2_2
	ldx #%01110000
	stx PF0
	sta WSYNC
	dey
	bne loop2_2
	
	ldy #3
loop3_2
	ldx #%11110000
	stx PF0
	ldx #%10000000
	stx PF1
	sta WSYNC
	dey
	bne loop3_2
	
	ldy #9
loop4_2
	lda #%11000000
	sta PF1
	sta WSYNC
	inx
	dey
	bne loop4_2
	
	ldy #3
	ldx #0
loop5_2
	lda #%11100000
	sta PF1
	lda tree_1,x
	sta GRP0
	sta WSYNC
	inx 
	dey
	bne loop5_2
	
	ldy #3
loop6a_2
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%10000000
	sta PF2
	sleep 6
	lda #YELL
	sta COLUPF
	sleep 16
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop6a_2
	
	ldy #3
loop6b_2
	lda #%11110000
	sta PF1
	lda tree_1,x
	sta GRP0
	lda #%11000000
	sta PF2
	sleep 6
	lda #YELL
	sta COLUPF
	sleep 16
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop6b_2
	
	ldy #3
loop7_2
	ldx #%11111100
	stx PF1
	ldx #%11100000	
	stx PF2
	sleep 16
	ldx #YELL
	stx COLUPF
	sleep 14
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop7_2
	
	ldy #3
loop8_2
	ldx #%11111110
	stx PF1
	ldx #%11110000
	stx PF2
	sleep 20
	ldx #YELL
	stx COLUPF
	sleep 12
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop8_2
	
	ldy #3
loop9_2
	ldx #%11111111
	stx PF1
	sleep 20
	ldx #%11100000
	stx PF2
	ldx #YELL
	stx COLUPF
	sleep 10
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop9_2
	
	ldy #3
loop10_2
	sleep 24
	ldx #%11000000
	stx PF2
	ldx #YELL
	stx COLUPF
	sleep 12
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop10_2
	
	ldy #3
loop11_2
	sleep 24
	ldx #%10000001
	stx PF2
	sleep 2
	ldx #YELL
	stx COLUPF
	sleep 10
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop11_2
	
	ldy #3
loop12_2
	ldx #DARKBLUE
	stx COLUBK
	sleep 8
	ldx #%00000011
	stx PF2
	sleep 27
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC
	dey
	bne loop12_2

	ldx #%00000011
	stx PF2
	sleep 28
	ldx #ORANGE
	stx COLUPF
	sleep 8
	ldx #DARKBROWN
	stx COLUPF
	sta WSYNC

	ldy #2
	ldx #0
loop13_2
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 22
	lda #ORANGE
	sta COLUPF
	sleep 6
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx
	dey
	bne loop13_2
	
	sleep 14
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sleep 10
	lda #ORANGE
	sta COLUPF
	sleep 6
	lda #DARKBROWN
	sta COLUPF
	sta WSYNC
	inx

	ldy #6
loop14_2
	lda #%00000011
	sta PF2
	lda tree_1,x
	sta GRP1
	sta WSYNC
	inx
	dey
	bne loop14_2

	ldy #3
loop15_2
	ldx #%00000111
	stx PF2
	sta WSYNC
	dey
	bne loop15_2
	
	ldx #BLACK
	stx COLUBK		
	ldx #0
	stx PF0
	stx PF1
	stx PF2
	stx CTRLPF
	rts
	
tree_1
	byte #%00011000
	byte #%00011000
	byte #%00111100
	byte #%00111100
	byte #%01111110
	byte #%00011000
	byte #%00011000
	byte #%00011000
	byte #%00000000
	
	ALIGN 256
	
PositionASpriteSubroutine2 ;taken from vdub_bobby

	sec            ;doing this before so that I have more time
                       ;during the next scanline.
	sta WSYNC      ;begin line 1

DivideLoop2
	sbc #15
	bcs DivideLoop2                 ;+4/5 4/9.../54
	tay                            ;+2 6
	lda FineAdjustTableEnd2,Y       ;+5 11
	nop
	nop            ;+4     15/20/etc.   - 4 free cycles!
	sta HMP0,X     ;+4     19/24/...
	sta RESP0,X    ;+4     23/28/33/38/43/48/53/58/63/68/73
	sta WSYNC
	rts            ;+6      9
	
FineAdjustTableBegin2

	byte #%01100000 ;left 6
	byte #%01010000
	byte #%01000000
	byte #%00110000
	byte #%00100000
	byte #%00010000
	byte #%00000000 ;left/right 0
	byte #%11110000
	byte #%11100000
	byte #%11010000
	byte #%11000000
	byte #%10110000
	byte #%10100000
	byte #%10010000
	byte #%10000000 ;right 8
	
FineAdjustTableEnd2 = FineAdjustTableBegin2 - 241
	
;-------------------------------------
;Enter or leave Bank 2 
;-------------------------------------
	ORG $2FAD
	RORG $1FAD
Return_Bank1_Start
	ldx $1FF4

	ORG $2FB6
	RORG $1FB6
Start_Sun_Kernel
	jsr SunKernels
	ldx $1FF4
	
	ORG $2FFA
	.word Bank2          ; NMI
	.word Bank2         ; RESET
	.word Bank2          ; IRQ
	
;-------------------------------------
	;Bank 3
;-------------------------------------
	ORG $3000
	RORG $1000
Bank3
	jmp Return_Bank1_Start
	
SoundEngine
	ldx SOUNDDELAYCOUNTER
	cpx #3
	beq next_tone
	inx
	stx SOUNDDELAYCOUNTER	
	jmp finish_sound_engine
	
next_tone
	ldx #0
	stx SOUNDDELAYCOUNTER
	ldx SOUNDTABLESELECTOR
	beq no_sound_plz
	cpx #1
	beq load_melody
	cpx #2
	beq load_reload
	cpx #3
	beq load_explosion
	cpx #4
	beq load_shot
	cpx #5
	beq load_wrongshot
	cpx #6
	beq load_nomun
	jmp finish_sound_engine
update_soundchip
	stx AUDF0
	sty AUDC0
	jmp finish_sound_engine

no_sound_plz
	ldx #0
	ldy #0
	stx AUDV0
	jmp update_soundchip
load_melody
	ldx #%00001100
	stx AUDV0
	ldx SOUNDTABLEINDEX
	lda TitleSoundTable,x
	inx
	ldy TitleSoundTable,x
	jmp finish_sound_load
load_explosion
	ldx #%00001100
	stx AUDV0
	ldx SOUNDTABLEINDEX
	lda ShotWithHitTable,x
	inx
	ldy ShotWithHitTable,x
	jmp finish_sound_load
load_shot
	ldx #%00001100
	stx AUDV0
	ldx SOUNDTABLEINDEX
	lda ShotWithoutHitTable,x
	inx
	ldy ShotWithoutHitTable,x
	jmp finish_sound_load
load_reload
	ldx #%00001100
	stx AUDV0
	ldx SOUNDTABLEINDEX
	lda ReloadTable,x
	inx
	ldy ReloadTable,x
	jmp finish_sound_load
load_nomun
	ldx #%00001100
	stx AUDV0
	ldx SOUNDTABLEINDEX
	lda NomunTable,x
	inx
	ldy NomunTable,x
	jmp finish_sound_load
load_wrongshot
	ldx #%00001100
	stx AUDV0
	ldx SOUNDTABLEINDEX
	lda WrongTargetTable,x
	inx
	ldy WrongTargetTable,x
finish_sound_load
	inx
	stx SOUNDTABLEINDEX
	tax
	cpx #$ff
	beq stop_all_sound
	cpx #$fe
	bne update_soundchip
	lda #4
	jmp repeat_all_sound
stop_all_sound
	lda #0
	sta SOUNDTABLESELECTOR
	sta SOUNDTABLEINDEX
	sta SOUNDDELAYCOUNTER
	jmp no_sound_plz
repeat_all_sound
	sta SOUNDTABLESELECTOR
	lda #4
	sta SOUNDDELAYCOUNTER
	lda #0
	sta SOUNDTABLEINDEX
	jmp SoundEngine

finish_sound_engine
	rts
	
TitleSoundTable
	byte #%00000111
	byte #%00001010
	byte #%00000111
	byte #%00001010
	byte #%00001000
	byte #%00001010
	byte #%00001000
	byte #%00001010
	byte #%00001001
	byte #%00001010
	byte #%00001001
	byte #%00001010
	byte #%00000111
	byte #%00001010
	byte #%00000111
	byte #%00001010
	byte #$ff
	
ShotWithHitTable
	byte #%00000010
	byte #%00001100
	byte #%00000110
	byte #%00001100
	byte #%00000111
	byte #%00001100
	byte #$ff
	
ShotWithoutHitTable
	byte #%00000110
	byte #%00001010
	byte #$ff
	
WrongTargetTable
	byte #%00001111
	byte #%00001010
	byte #$ff
	
NomunTable
	byte #%00000000
	byte #%00001000
	byte #$ff
	
ReloadTable
	byte #%00000110
	byte #%00000100
	byte #%00000101
	byte #%00000100
	byte #%00000100
	byte #%00000100
	byte #$ff

;-------------------------------------
;Enter or leave Bank 3 
;-------------------------------------
	ORG $3FAD
	RORG $1FAD
Return_Bank1_Start
	ldx $1FF4
	
	ORG $3fc0
	RORG $1fc0
start_sound_engine
	jsr SoundEngine
	ldx $1FF4
	
	ORG $3FFA
	.word Bank3          ; NMI
	.word Bank3         ; RESET
	.word Bank3          ; IRQ
	
;-------------------------------------
	;Bank 4
;-------------------------------------
	ORG $4000
	RORG $1000
Bank4
	jmp Return_Bank1_Start
	
	ORG $4FAD
	RORG $1FAD
Return_Bank1_Start
	ldx $1FF4
	
	ORG $4FFA
	.word Bank4          ; NMI
	.word Bank4         ; RESET
	.word Bank4          ; IRQ
	
;-------------------------------------
	;Bank 5
;-------------------------------------
	ORG $5000
	RORG $1000
Bank5
	jmp Return_Bank1_Start
	
	ORG $5FAD
	RORG $1FAD
Return_Bank1_Start
	ldx $1FF4
	
	ORG $5FFA
	.word Bank5          ; NMI
	.word Bank5         ; RESET
	.word Bank5          ; IRQ
	
;-------------------------------------
	;Bank 6
;-------------------------------------
	ORG $6000
	RORG $1000
Bank6
	jmp Return_Bank1_Start
	
	ORG $6FAD
	RORG $1FAD
Return_Bank1_Start
	ldx $1FF4
	
	ORG $6FFA
	.word Bank6          ; NMI
	.word Bank6         ; RESET
	.word Bank6          ; IRQ
	
;-------------------------------------
	;Bank 7
;-------------------------------------
	ORG $7000
	RORG $1000
Bank7
	jmp Return_Bank1_Start
	
	ORG $7FAD
	RORG $1FAD
Return_Bank1_Start
	ldx $1FF4
	
	ORG $7FFA
	.word Bank7          ; NMI
	.word Bank7         ; RESET
	.word Bank7          ; IRQ
	
;-------------------------------------
	;Bank 8
;-------------------------------------
	ORG $8000
	RORG $1000
Bank8
	jmp Return_Bank1_Start
	
	ORG $8FAD
	RORG $1FAD
Return_Bank1_Start
	ldx $1FF4
	
	ORG $8FFA
	.word Bank8          ; NMI
	.word Bank8         ; RESET
	.word Bank8          ; IRQ
	
;-------------------------------------
