;Author: belial
;E-Mail: christian.ammann@phobosys.de
;
;Breakout clone for Atari 2600

	processor 6502
	include "vcs.h"
	include "macro.h"
	include "breakout.h"

	SEG
	ORG $F000

Reset

	ldx #0
	txa

Clear   dex
	txs
	pha
	bne Clear
	
	;ball initialisation
	ldx #100
	stx COLUP1
	ldx #50
	stx COLUP0
	stx PADDLECOLOR
	
	ldx #0
	stx BALLVEKTORPOINTER
	ldx #3
	stx HORPOSPADDLE
	ldx #%00000101
	stx NUSIZ0
	ldx #255
	stx COLLISIONPOINTX
	
	ldx #%11101010
	stx LIFES
	
	ldx Levels_Offset
	stx LEVELPOINTER
	ldx Levels_Offset+1
	stx LEVELPOINTER2	
	       
StartOfFrame

	; deactivate the blank pattern
	lda #0
	sta VBLANK              
	
	lda #2
	sta VSYNC

	; 3 scanlines of VSYNCH signal...

	sta WSYNC
	sta WSYNC
	sta WSYNC

	lda #0
	sta VSYNC 
	
	;check wether we have to load a new level
	ldx BLOCKSLEFT
	bne no_new_level
	jsr LoadLevel
	ldx #0
	stx BALLVEKTORX1 
	stx BALLVEKTORY1 
	stx BALLVEKTORX2 
	stx BALLVEKTORY2
	ldx #90
	stx VERTPOSBALL
	ldx #80
	stx HORPOSBALL
	ldx #1
	stx PAUSE
	jmp StartOfFrame
	
no_new_level         
	ldx LEVEL
	cpx #11
	beq Reset
	
	; 45 scanlines of vertical blank...
	; Some game logic stuff too
	
	;load one one of the two move vektors
	ldx BALLVEKTORPOINTER
	cpx #0
	bne loadVektor2
loadVektor1
	ldx BALLVEKTORY1
	stx BALLVEKTORY
	ldx BALLVEKTORX1
	stx BALLVEKTORX
	lda BALLVEKTORPOINTER
	eor #%11111111
	sta BALLVEKTORPOINTER
	jmp addTheVektor
loadVektor2
	ldx BALLVEKTORY2
	stx BALLVEKTORY
	ldx BALLVEKTORX2
	stx BALLVEKTORX
	lda BALLVEKTORPOINTER
	eor #%11111111
	sta BALLVEKTORPOINTER	 	 

addTheVektor
	sta WSYNC
	;stroe old position needed for paddle collision
	lda HORPOSBALL
	sta HORPOSBALLOLD
	lda VERTPOSBALL
	sta VERTPOSBALLOLD	
	;add move vector for ball
	lda BALLVEKTORX
	clc
	adc HORPOSBALL
	sta HORPOSBALL
	lda BALLVEKTORY
	clc
	adc VERTPOSBALL
	sta VERTPOSBALL
	sta WSYNC

	;check collision ball/paddle	
ball_paddle_collision_check	
	lda #255 ;keine kollision = 255
	sta COLLISIONPOINTX
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #1
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #2
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #1
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #3
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #1
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #4
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #2
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #5
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #3
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #6
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #4
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #6
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #5
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #5
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #6
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #4
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #6
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #3
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	;check wether these coordinates hit the paddle
	lda HORPOSBALL
	clc
	adc #6
	sta BALLPOSTEMPX	
	lda VERTPOSBALL
	sec
	sbc #2
	sta BALLPOSTEMPY	
	jsr BallPaddleCollision ;takes 1 WSYNC
	
	ldx COLLISIONPOINTX
	cpx #255
	beq everything_okay
	
	;a collision has happend
	;load old ball position
	ldx HORPOSBALLOLD
	stx HORPOSBALL
	ldx VERTPOSBALLOLD
	stx VERTPOSBALL
	
	;start sound
	ldx #2
	stx MUSICTYPE
	
	lda COLLISIONPOINTX
	asl 
	asl
	tax
	ldy PaddleBallCollisionTable,x
	sty BALLVEKTORX1
	inx
	ldy PaddleBallCollisionTable,x
	sty BALLVEKTORY1
	inx
	ldy PaddleBallCollisionTable,x
	sty BALLVEKTORX2
	inx
	ldy PaddleBallCollisionTable,x
	sty BALLVEKTORY2
	sta WSYNC
	jmp finished_ball_paddle_collision
everything_okay
	sta WSYNC
finished_ball_paddle_collision

	
;check wether the paddle has to move
	ldy COLLISIONPOINTX
	cpx #255
	bne dont_move_paddle
		
	ldy SWCHA
	
	ldx HORPOSPADDLE
	cpy #%01111111
	bne not_move_right
	inx
	inx
	stx HORPOSPADDLE
not_move_right
	sta WSYNC	
	cpy #%10111111
	bne not_move_left
	dex
	dex
	stx HORPOSPADDLE
not_move_left	
	sta WSYNC

	;check wether the paddle is too much right or left	
	ldx HORPOSPADDLE
	cpx #3
	bcc tomuchleft
	cpx #139
	bcc paddleokay
tomuchright
	ldx #139
	stx HORPOSPADDLE
	jmp paddleokay
tomuchleft
	ldx #3
	stx HORPOSPADDLE
	jmp paddleokay
dont_move_paddle
	sta WSYNC
	sta WSYNC
paddleokay
	sta WSYNC
	
	;check wether fire button was pressed
	lda INPT4
	and #%10000000
	cmp #%00000000
	bne not_fired
	lda PAUSE
	cmp #1
	bne not_fired
	jsr ResumeGame
	sta WSYNC
	jmp finished_fire_button
not_fired
	sta WSYNC
finished_fire_button	

	;check wether reset was pressed
	lda SWCHB
	and 00000001
	cmp #1
	beq no_reset_plz
	jmp Reset	
no_reset_plz

	;check wether game select was pressed
	lda SWCHB
	and #%00000010
	cmp #2
	sta WSYNC
	beq no_select_plz	
	ldx PADDLECOLOR
	inx
	stx PADDLECOLOR
	stx COLUP0
no_select_plz

	;test wether the ball is out of bounds
testrightwall	
	ldx HORPOSBALL
	cpx #150
	bcc testleftwall
	ldx #150
	stx HORPOSBALL
testleftwall
	ldx HORPOSBALL
	cpx #2
	bcs testupperwall
	ldx #2
	stx HORPOSBALL
testupperwall
	sta WSYNC
	ldx VERTPOSBALL
	cpx #240
	bcc testlowerwall
	ldx #240
	stx VERTPOSBALL
testlowerwall
	ldx VERTPOSBALL
	cpx #36
	bcs finishedoverboundtest
	ldx #36
	stx VERTPOSBALL
finishedoverboundtest
	sta WSYNC
		 	
	;collision check with walls
rightwall
	ldx HORPOSBALL
	cpx #150
	bne leftwall 
	jsr InvertLeftRight
	jmp upperwall
leftwall
	cpx #2
	bne upperwall
	jsr InvertLeftRight
upperwall
	sta WSYNC
	ldx VERTPOSBALL
	cpx #240
	bne lowerwall
	jsr InvertUpDown
	jmp ball_block_collision
lowerwall
	cpx #36
	bne ball_block_collision
	jsr PauseGame
	ldx #3
	stx MUSICTYPE

	;check wether ball meets block	
ball_block_collision			
	sta WSYNC
	;check wether the ball is in collision zone
	lda VERTPOSBALL
	sta BALLPOSTEMPY
	lda HORPOSBALL
	sta BALLPOSTEMPX
	jsr CheckForCollisionWithBlocks
	sta WSYNC

	cpx #1
	bne no_collision1
	;when yes, calculate where we have to remove a block
	;takes 2 WSYNC
	jsr DoTheCollisionUpDown
	jmp after_collision1
no_collision1
	sta WSYNC
	sta WSYNC
after_collision1
	
	lda VERTPOSBALL
	sec
	sbc #3
	sta BALLPOSTEMPY
	lda HORPOSBALL
	sec
	sbc #2
	sta BALLPOSTEMPX
	sta WSYNC	
	jsr CheckForCollisionWithBlocks
	sta WSYNC

	cpx #1
	bne no_collision2
	;when yes, calculate where we have to remove a block
	;takes 2 WSYNC
	jsr DoTheCollisionLeftRight
	jmp after_collision2
no_collision2
	sta WSYNC
	sta WSYNC
after_collision2

	lda VERTPOSBALL
	sec
	sbc #6
	sta BALLPOSTEMPY
	lda HORPOSBALL
	sta BALLPOSTEMPX
	sta WSYNC
	jsr CheckForCollisionWithBlocks
	sta WSYNC

	cpx #1
	bne no_collision3
	;when yes, calculate where we have to remove a block
	;takes 2 WSYNC
	jsr DoTheCollisionUpDown
	jmp after_collision3
no_collision3
	sta WSYNC
	sta WSYNC
after_collision3

	lda VERTPOSBALL
	sec
	sbc #3
	sta BALLPOSTEMPY
	lda HORPOSBALL
	clc
	adc #3
	sta BALLPOSTEMPX
	sta WSYNC
	jsr CheckForCollisionWithBlocks
	sta WSYNC

	cpx #1
	bne no_collision4
	;when yes, calculate where we have to remove a block
	;takes 2 WSYNC
	jsr DoTheCollisionLeftRight
	jmp after_collision4
no_collision4
	sta WSYNC
	sta WSYNC
after_collision4

	;the ball-wall collision check is finished at this pont
	
	;music stuff to do
	sta WSYNC
	ldx MUSICTYPE
	cpx #1
	beq ball_block_music
	cpx #2
	beq ball_paddle_music	
	cpx #3
	beq ball_out_music
	ldx MUSICCOUNTER
	beq turn_music_off
	dex
	stx MUSICCOUNTER
	jmp finished_music_stuff	
ball_block_music
	ldx #4
	stx AUDC0
	ldx #15
	stx AUDF0
	ldx #%00000100
	stx AUDV0	
	ldx #0
	stx MUSICTYPE
	jmp finished_music_stuff
ball_paddle_music	
	ldx #%00001001
	stx AUDC0
	ldx #15
	stx AUDF0
	ldx #%00000100
	stx AUDV0	
	ldx #0
	stx MUSICTYPE
	jmp finished_music_stuff
ball_out_music	
	ldx #1
	stx AUDC0
	ldx #23
	stx AUDF0
	ldx #%00000100
	stx AUDV0	
	ldx #0
	stx MUSICTYPE
	ldx #5
	stx MUSICCOUNTER
	jmp finished_music_stuff
turn_music_off
	ldx 0
	stx AUDV0
finished_music_stuff
	
	;horizontal pos the paddle
	ldx #0
	stx GRP0	
	lda HORPOSPADDLE
	ldx #0
	;two WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine
	
	;pos the ball 
	ldx #0
	stx GRP1
	lda HORPOSBALL
	ldx #1
	;two WSYNC is taken by the subroutine
	jsr PositionASpriteSubroutine
	
	sta WSYNC
	sta HMOVE
	
	ldx #0
lines45	sta WSYNC
	inx		;2 Zyklen
	cpx #6		;3 Zyklen
	bne lines45	;2 Zyklen 

	;228 scanlines of picture...

	ldy #7		;2 Zyklen
	ldx VERTPOSBALL	;to find the right vertical pos for
			;the ball
	
	lda #20
	sta COLUPF
	lda #%11111111
	sta PF0
	sta PF1
	sta PF2
	lda #208	;2 Zyklen	
	sta COLUBK	;3 Zyklen
	
upperborder
	lda LIFES
	sta PF1
	sleep 14	
	lda #255
	sta PF1
	
	sta WSYNC
	inx
	lda noballz1,x
	sta GRP1			
	dey		;2 Zyklen
	bne upperborder	;3 Zyklen
	
	inx
	sta WSYNC
	
	ldy #30
emptyroom
	lda #%00010000
	sta PF0
	lda #0
	sta PF1
	lda #0
	sta PF2 
	sleep 4
	lda #0
	sta PF0
	sleep 18
	inx
	lda noballz1,x
	sta GRP1
	
	lda #%10000000
	sta PF2
	sta WSYNC			
	dey		;2 Zyklen
	bne emptyroom	;3 Zyklen
	
	
	ldy #10		;2 Zyklen 
row1
	;left side
	lda #%00010000
	sta PF0
	lda $80
	sta PF1
	nop
	lda $81
	sta COLUPF
	lda $82
	sta PF2
	sleep 9
	
	;right side
	lda $83
	sta PF0
	lda $84
	sta PF1
	lda $85
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row1	;3 Zyklen
	
	ldy #10		;2 Zyklen 
row2
	;left side
	lda #%00010000
	sta PF0
	lda $86
	sta PF1
	nop
	lda $87
	sta COLUPF
	lda $88
	sta PF2
	sleep 9
	
	;right side
	lda $89
	sta PF0
	lda $8a
	sta PF1
	lda $8b
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row2	;3 Zyklen

	ldy #10		;2 Zyklen 
row3
	;left side
	lda #%00010000
	sta PF0
	lda $8c
	sta PF1
	nop
	lda $8d
	sta COLUPF
	lda $8e
	sta PF2
	sleep 9
	
	;right side
	lda $8f
	sta PF0
	lda $90
	sta PF1
	lda $91
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row3	;3 Zyklen
	
	ldy #10		;2 Zyklen 
row4
	;left side
	lda #%00010000
	sta PF0
	lda $92
	sta PF1
	nop
	lda $93
	sta COLUPF
	lda $94
	sta PF2
	sleep 9
	
	;right side
	lda $95
	sta PF0
	lda $96
	sta PF1
	lda $97
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row4	;3 Zyklen
	
	ldy #10		;2 Zyklen 
row5
	;left side
	lda #%00010000
	sta PF0
	lda $98
	sta PF1
	nop
	lda $99
	sta COLUPF
	lda $9a
	sta PF2
	sleep 9
	
	;right side
	lda $9b
	sta PF0
	lda $9c
	sta PF1
	lda $9d
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row5	;3 Zyklen
	
	ldy #10		;2 Zyklen 
row6
	;left side
	lda #%00010000
	sta PF0
	lda $9e
	sta PF1
	nop
	lda $9f
	sta COLUPF
	lda $a0
	sta PF2
	sleep 9
	
	;right side
	lda $a1
	sta PF0
	lda $a2
	sta PF1
	lda $a3
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row6	;3 Zyklen
	
	ldy #10		;2 Zyklen 
row7
	;left side
	lda #%00010000
	sta PF0
	lda $a4
	sta PF1
	nop
	lda $a5
	sta COLUPF
	lda $a6
	sta PF2
	sleep 9
	
	;right side
	lda $a7
	sta PF0
	lda $a8
	sta PF1
	lda $a9
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row7	;3 Zyklen
	
	ldy #10		;2 Zyklen 
row8
	;left side
	lda #%00010000
	sta PF0
	lda $aa
	sta PF1
	nop
	lda $ab
	sta COLUPF
	lda $ac
	sta PF2
	sleep 9
	
	;right side
	lda $ad
	sta PF0
	lda $ae
	sta PF1
	lda $af
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row8	;3 Zyklen
	
	ldy #10		;2 Zyklen 
row9
	;left side
	lda #%00010000
	sta PF0
	lda $b0
	sta PF1
	nop
	lda $b1
	sta COLUPF
	lda $b2
	sta PF2
	sleep 9
	
	;right side
	lda $b3
	sta PF0
	lda $b4
	sta PF1
	lda $b5
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row9	;3 Zyklen
	
	ldy #10		;2 Zyklen 
row10
	;left side
	lda #%00010000
	sta PF0
	lda $b6
	sta PF1
	nop
	lda $b7
	sta COLUPF
	lda $b8
	sta PF2
	sleep 9
	
	;right side
	lda $b9
	sta PF0
	lda $ba
	sta PF1
	lda $bb
	sta PF2
	inx
	lda noballz1,x
	sta GRP1
	
	lda #20
	sta COLUPF
	sta WSYNC			
	dey		;2 Zyklen
	bne row10	;3 Zyklen
	
		
	ldy #74
	lda #0
	sta PF1
emptyroom2
	lda #%00010000
	sta PF0
	lda #0
	sta PF2 
	sleep 10
	lda #0
	sta PF0
	sleep 20
	inx
	lda noballz1,x
	sta GRP1
	lda #%10000000
	sta PF2
	 
	sta WSYNC			
	dey		;2 Zyklen
	bne emptyroom2	;3 Zyklen
	
	ldy #8
	lda #0
	sta PF1
paddlearea
	lda #%00010000
	sta PF0
	lda #255
	sta GRP0
	lda #0
	sta PF2 
	sleep 5
	lda #0
	sta PF0
	sleep 20
	inx
	lda noballz1,x
	sta GRP1
	lda #%10000000
	sta PF2
	 
	sta WSYNC			
	dey		;2 Zyklen
	bne paddlearea	;3 Zyklen
	
	lda #0
	sta GRP0
			
	ldy #4		;2 Zyklen
	lda #20
	sta COLUPF
lowerborder
	lda #%00010000
	sta PF0
	lda #0
	sta PF1
	sta PF2
	sta PF0
	sleep 35
	lda #%10000000
	sta PF2
	
	sta WSYNC
	inx
	lda noballz1,x
	sta GRP1			
	dey		;2 Zyklen
	bne lowerborder	;3 Zyklen	
	
	; end of screen - enter blanking
	lda #%01000010
	sta VBLANK
	lda #0  
	sta COLUBK
	sta COLUPF

	; 30 scanlines of overscan...

	ldx #0
lines36	sta WSYNC
	inx
	cpx #36
	bne lines36
	                  
	jmp StartOfFrame

	;this is the ball graphic	
noballz1
	REPEAT 249
	byte #0
	REPEND
ballz
	byte #%00011000
	byte #%00111100
	byte #%01111110
	byte #%01111110
	byte #%01111110
	byte #%00111100
	byte #%00011000

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

CheckForCollisionWithBlocks
	ldx BALLPOSTEMPX
	cpx #13
	bcc nocollision ;if smaller 
	cpx #141
	bcs nocollision ;if greater
	ldx BALLPOSTEMPY
	cpx #112
	bcc nocollision ;if smaller
	cpx #211
	bcs nocollision ;if greater 
	ldx #1
	rts
nocollision
	ldx #0	
	rts
	
DoTheCollisionUpDown
	;look in the HorTable
	lda BALLPOSTEMPX
	sec
	sbc #13
	tax
	lda HorCheckTable,x
	tay
	
	;look in the VerTable
	lda BALLPOSTEMPY
	sec
	sbc #112
	tax
	lda VertCheckTable,x
	
	;access collision table
	sty TEMP
	clc
	adc TEMP
	sta WSYNC
	tax
	ldy GetBlockTable,x	
	lda $0,y
	sta TEMP
	inx 
	and GetBlockTable,x
	sta OLDSTONE
	sta $0,y
	
	;check wether we need to change the ball direction vector
	ldx OLDSTONE
	cpx TEMP
	beq they_are_equal1
	ldx #1
	stx MUSICTYPE
	dec BLOCKSLEFT
	sta WSYNC
	jsr InvertUpDown
	jmp they_are_equal1_end
they_are_equal1
	sta WSYNC
they_are_equal1_end		
	rts

InvertUpDown
	lda BALLVEKTORY1
	eor #%11111111
	clc
	adc #1 
	sta BALLVEKTORY1
	
	lda BALLVEKTORY2
	eor #%11111111
	clc
	adc #1 
	sta BALLVEKTORY2
	rts

InvertLeftRight
	lda BALLVEKTORX1
	eor #%11111111
	clc
	adc #1 
	sta BALLVEKTORX1
	
	lda BALLVEKTORX2
	eor #%11111111
	clc
	adc #1 
	sta BALLVEKTORX2
	rts
				
DoTheCollisionLeftRight
	;look in the HorTable
	lda BALLPOSTEMPX
	sec
	sbc #13
	tax
	lda HorCheckTable,x
	tay
	
	;look in the VerTable
	lda BALLPOSTEMPY
	sec
	sbc #112
	tax
	lda VertCheckTable,x
	
	;access collision table
	sty TEMP
	clc
	adc TEMP
	sta WSYNC
	tax
	ldy GetBlockTable,x	
	lda $0,y
	sta TEMP
	inx 
	and GetBlockTable,x
	sta OLDSTONE
	sta $0,y
	
	;check wether we need to change the ball direction vector
	ldx OLDSTONE
	cpx TEMP
	beq they_are_equal2	
	ldx #1
	stx MUSICTYPE
	dec BLOCKSLEFT
	sta WSYNC
	jsr InvertLeftRight
	jmp they_are_equal2_end
they_are_equal2
	sta WSYNC
they_are_equal2_end
	rts


BallPaddleCollision
	ldx BALLPOSTEMPY
	cpx #37
	bcs no_ballpaddlecollision
	lda BALLPOSTEMPX
	sec
	sbc HORPOSPADDLE
	cmp #0
	bcc no_ballpaddlecollision
	cmp #16
	bcs no_ballpaddlecollision
	
	sta COLLISIONPOINTX
	sta WSYNC
	rts

no_ballpaddlecollision
	sta WSYNC	 
	rts
	
PauseGame
	lda LIFES
	sec
	ror
	sec
	ror
	sta LIFES
	ldx #0
	stx BALLVEKTORX1 
	stx BALLVEKTORY1 
	stx BALLVEKTORX2 
	stx BALLVEKTORY2
	ldx #90
	stx VERTPOSBALL
	ldx #80
	stx HORPOSBALL
	ldx #1
	stx PAUSE
	rts
	
ResumeGame
	ldx LIFES
	cpx #%11111111
	beq no_resume
	
	ldx #1
	stx BALLVEKTORX1 
	stx BALLVEKTORX2 
	ldx #255
	stx BALLVEKTORY1 
	stx BALLVEKTORY2
	ldx #0
	stx PAUSE
	
no_resume
	rts
	
LoadLevel
	ldx #60
	ldy #0
load_level_loop
	lda (LEVELPOINTER),y
	sta $80,y
	iny
	dex
	bne load_level_loop
	
	lda (LEVELPOINTER),y
	sta BLOCKSLEFT
	
	lda LEVELPOINTER
	clc
	adc #61
	sta LEVELPOINTER
	bcc MSBFine
	inc LEVELPOINTER2
MSBFine
	inc LEVEL
	rts
				
PaddleBallCollisionTable
	byte #254,#1,#254,#1
	byte #254,#1,#254,#1
	
	byte #254,#1,#254,#1
	byte #254,#1,#254,#1
	
	byte #254,#2,#255,#1
	byte #254,#2,#255,#1
	
	byte #255,#2,#255,#2
	byte #255,#2,#255,#2
	
	byte #1,#2,#1,#2
	byte #1,#2,#1,#2
	
	byte #2,#2,#1,#1
	byte #2,#2,#1,#1
	
	byte #2,#1,#2,#1
	byte #2,#1,#2,#1
	
	byte #2,#1,#2,#1
	byte #2,#1,#2,#1

Levels_Offset
POS	SET Levels
	word POS	
Levels
;Level 1 starts here
POS	SET 22
	REPEAT 10
	byte #%11111111
	byte POS
	byte #%11111111
	byte #%11110000
	byte #%11111111
	byte #%10001111
POS 	SET POS+16	
	REPEND
	byte #80
;level 2 starts here
	byte #%11111111,#$44,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #%11111111,#$55,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #%00000000,#$a8,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%00000000,#$a8,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%11111111,#$3e,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #%11111111,#$3e,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #%00000000,#$a8,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%00000000,#$a8,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%11111111,#$55,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #%11111111,#$44,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #56	
;Level 3 starts here
	byte #%11110000,#$48,#%00001111,#%11110000,#%00001111,#%10000000
	byte #%00001111,#$42,#%11110000,#%00000000,#%11110000,#%10001111
	byte #%11110000,#$48,#%00001111,#%11110000,#%00001111,#%10000000
	byte #%00001111,#$83,#%11110000,#%00000000,#%11110000,#%10001111
	byte #%11110000,#$26,#%00001111,#%11110000,#%00001111,#%10000000
	byte #%00001111,#$85,#%11110000,#%00000000,#%11110000,#%10001111
	byte #%11110000,#$48,#%00001111,#%11110000,#%00001111,#%10000000
	byte #%00001111,#$83,#%11110000,#%00000000,#%11110000,#%10001111
	byte #%11110000,#$26,#%00001111,#%11110000,#%00001111,#%10000000
	byte #%00001111,#$85,#%11110000,#%00000000,#%11110000,#%10001111
	byte #40
;Level 4 starts here
	REPEAT 5
	byte #%11111111
	byte #$9e
	byte #%11111111
	byte #%11110000
	byte #%11111111
	byte #%10001111
	byte #%00000000
	byte #0
	byte #%00000000
	byte #%00000000
	byte #%00000000
	byte #%10000000	
	REPEND
	byte #40
;Level 5 starts here
	byte #%11110000,#$d8,#%00001111,#%00000000,#%11110000,#%10001111 ;4
	byte #%11110000,#$c8,#%00001111,#%00000000,#%11110000,#%10001111 ;4
	byte #%11110000,#$b8,#%00001111,#%00000000,#%11110000,#%10001111 ;4
	byte #%11110000,#$a8,#%11111111,#%11110000,#%11110000,#%10001111 ;6
	byte #%11110000,#$18,#%00001111,#%00000000,#%11110000,#%10001111 ;4
	byte #%11110000,#$28,#%00001111,#%00000000,#%11110000,#%10001111 ;4
	byte #%11110000,#$98,#%11111111,#%11110000,#%11110000,#%10001111 ;6
	byte #%11110000,#$38,#%00001111,#%00000000,#%11110000,#%10001111 ;4
	byte #%11110000,#$58,#%00001111,#%00000000,#%11110000,#%10001111 ;4
	byte #%11110000,#$48,#%00001111,#%00000000,#%11110000,#%10001111 ;4
	byte #44
;Level 6 starts here
	byte #%00000000,#$ca,#%00001111,#%00000000,#%11110000,#%10000000 ;2
	byte #%00000000,#$54,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%00000000,#$68,#%11111111,#%11110000,#%11110000,#%10000000 ;4
	byte #%00001111,#$76,#%11111111,#%11110000,#%11111111,#%10000000 ;6
	byte #%11111111,#$26,#%11110000,#%11110000,#%00001111,#%10001111 ;6
	byte #%11111111,#$68,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #%11111111,#$54,#%00000000,#%00000000,#%00001111,#%10001111 ;4
	byte #%00001111,#$68,#%11111111,#%11110000,#%11111111,#%10000000 ;6
	byte #%00000000,#$ca,#%11111111,#%11110000,#%11110000,#%10000000 ;4
	byte #%00000000,#$54,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #44
;Level 7 starts here
	byte #%00000000,#$a8,#%00001111,#%00000000,#%11110000,#%10000000 ;2
	byte #%00001111,#$89,#%11110000,#%11110000,#%00001111,#%10000000 ;4
	byte #%00000000,#$a8,#%00001111,#%00000000,#%11110000,#%10000000 ;2
	byte #%00000000,#$00,#%00000000,#%00000000,#%00000000,#%10000000 ;0
	byte #%00000000,#$75,#%00000000,#%11110000,#%00000000,#%10000000 ;1
	byte #%00000000,#$75,#%11110000,#%00000000,#%00000000,#%10000000 ;1
	byte #%00000000,#$75,#%11111111,#%11110000,#%00000000,#%10000000 ;3
	byte #%00000000,#$00,#%00000000,#%00000000,#%00000000,#%10000000 ;0
	byte #%00001111,#$46,#%00000000,#%00000000,#%00001111,#%10000000 ;2
	byte #%00000000,#$46,#%11111111,#%11110000,#%11110000,#%10000000 ;4
	byte #19
;Level 8 starts here
	REPEAT 5
	byte #%00000000
	byte #0
	byte #%00000000
	byte #%00000000
	byte #%00000000
	byte #%10000000
	byte #%11111111
	byte #$5c
	byte #%11111111
	byte #%11110000
	byte #%11111111
	byte #%10001111	
	REPEND
	byte #40
;Level 9 starts here
	byte #%11111111,#$44,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #%11110000,#$4c,#%00000000,#%00000000,#%00000000,#%10001111 ;2
	byte #%00001111,#$4a,#%00000000,#%00000000,#%00001111,#%10000000 ;2
	byte #%00000000,#$48,#%00001111,#%00000000,#%11110000,#%10000000 ;2
	byte #%00000000,#$46,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%00000000,#$46,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%00000000,#$48,#%00001111,#%00000000,#%11110000,#%10000000 ;2
	byte #%00001111,#$4a,#%00000000,#%00000000,#%00001111,#%10000000 ;2
	byte #%11110000,#$4c,#%00000000,#%00000000,#%00000000,#%10001111 ;2
	byte #%11111111,#$44,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #32	
;Level 10 starts here
	byte #%11111111,#$18,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #%00001111,#$28,#%11111111,#%11110000,#%11111111,#%10000000 ;6
	byte #%00000000,#$38,#%11111111,#%11110000,#%11110000,#%10000000 ;4
	byte #%00000000,#$48,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%00000000,#$00,#%00000000,#%00000000,#%00000000,#%10000000 ;0
	byte #%00000000,#$00,#%00000000,#%00000000,#%00000000,#%10000000 ;0
	byte #%00000000,#$48,#%11110000,#%11110000,#%00000000,#%10000000 ;2
	byte #%00000000,#$38,#%11111111,#%11110000,#%11110000,#%10000000 ;4
	byte #%00001111,#$28,#%11111111,#%11110000,#%11111111,#%10000000 ;6
	byte #%11111111,#$18,#%11111111,#%11110000,#%11111111,#%10001111 ;8
	byte #40
;level 11 begins here	
	REPEAT 61
	byte #1
	REPEND
;...
		
HorCheckTable
POS	SET 0
	REPEAT 8
	byte POS,POS,POS,POS,POS,POS,POS,POS,POS,POS
	byte POS,POS,POS,POS,POS,POS
POS	SET POS+2	
	REPEND
VertCheckTable
POS	SET 0
	REPEAT 10
	byte POS,POS,POS,POS,POS,POS,POS,POS,POS,POS
POS	SET POS+16
	REPEND
GetBlockTable
POS	SET 54	
	REPEAT 10
	byte $80+POS
	byte %00001111
	byte $80+POS
	byte %11110000
	byte $82+POS
	byte %11110000
	byte $82+POS
	byte %00001111
	byte $83+POS
	byte %00001111
	byte $84+POS
	byte %00001111
	byte $84+POS
	byte %11110000
	byte $85+POS
	byte %11110000
POS	SET POS-6 
	REPEND
	
	
	ORG $FF00

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

	ORG $FFFA

	.word Reset          ; NMI
	.word Reset          ; RESET
	.word Reset          ; IRQ

END


