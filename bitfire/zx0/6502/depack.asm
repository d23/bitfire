!cpu 6510
		* = $1000

		sei
		lda #$35
		sta $01
		lda $d011
		bpl *-3
		lda #$0b
		sta $d011

		ldx #$00
		lda #$20
-
		sta $0400,x
		sta $0500,x
		sta $0600,x
		sta $0700,x
		dex
		bne -

		jsr .timer_start

		lda #$00
		sta <.lz_dst + 0
		lda #$a0
		sta <.lz_dst + 1

		ldx #<data_start
		lda #>data_start

		jsr depack

		jsr .timer_stop

		lda #$1b
		sta $d011
		jmp *

.timer_start
                lda #$00
                sta $dc0e
                lda #$40
                sta $dc0f
                lda #$ff
                sta $dc04
                sta $dc05
                sta $dc06
                sta $dc07
                lda #$41
                sta $dc0f
                lda #$01
                sta $dc0e
                rts
.timer_stop
                lda #$00
                sta $dc0e
                lda #$40
                sta $dc0f

		ldy #$00
-
		lda .cycles,y
		sta $0400,y
		iny
		cpy #$08
		bne -
                lda $dc04
		pha
                lda $dc05
		pha
                lda $dc06
		pha
                lda $dc07
		jsr .print_hex
		pla
		jsr .print_hex
		pla
		jsr .print_hex
		pla
.print_hex
		eor #$ff
		pha
		lsr
		lsr
		lsr
		lsr
		tax
		lda .hextab,x
		sta $0400,y
		iny
		pla
		ldx #$0f
		sbx #$00
		lda .hextab,x
		sta $0400,y
		iny
		rts
.cycles
		!scr "cycles: "
.hextab
		!scr "0123456789abcdef"

!align 255,0
depack
!src "dzx0.asm"
!warn "depacker size: ", * - depack

data_start
!bin "testfile.zx0"