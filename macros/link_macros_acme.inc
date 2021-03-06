		!macro link_decomp_under_io {
			lda $01
			pha
			lda #$34
			sta $01                         ;bank out IO
			jsr link_decomp                 ;depack
			pla
			sta $01
		}

		!macro link_load_next_raw_decomp {
			jsr link_load_next_raw
			lda $01
			pha
			lda #$34
			sta $01                         ;bank out IO
			jsr link_decomp                 ;depack
			pla
			sta $01
		}

		;load next file as a raw file (do not decomp on the fly) and jump to .addr
		!macro link_load_next_raw_jmp .arg {
			lda #>(.arg-1)
			pha
			lda #<(.arg-1)
			pha
			jmp link_load_next_raw
		}

		;load next file and decomp on the fly, then jump to .arg
		!macro link_load_next_comp_jmp .arg {
			lda #>(.arg-1)
			pha
			lda #<(.arg-1)
			pha
			jmp link_load_next_comp
		}

		;decomp previously loaded file and jump to .arg
		!macro link_decomp_jmp .arg {
			lda #>(.arg-1)
			pha
			lda #<(.arg-1)
			pha
			jmp link_decomp
		}

		!macro link_load_next_double {
			jsr link_load_next_comp
			jsr link_load_next_raw
			lda $01
			pha
			lda #$34
			sta $01
			jsr link_decomp
			pla
			sta $01
		}

		;link irq hook back to base irq
		!macro link_player_irq {
			sei
			lda #<link_player
			sta $fffe
			lda #>link_player
			sta $ffff
			lda #$ff
			sta $d012
			cli
		}

		!macro reset_drive {
			lda #BITFIRE_RESET
			jsr bitfire_send_byte_
		}

		!macro wait_floppy_busy {
			;wait until floppy is busy
.l
			bit $dd00
			bmi .l
		}

		!macro wait_floppy_idle {
			;wait until floppy is idle again
.l
			bit $dd00
			bpl .l
		}

		;request next disk side and reset filenum_counter
		!macro request_disk .arg {
			lda #.arg + $f0
			jsr bitfire_send_byte_
			pha
			pla
			pha
			pla
			;+wait_floppy_busy
			;lda #$3f
			;sta $dd02
			+wait_floppy_idle
		}

		;wait for given frame counter value
		!macro link_wait_syncpoint .arg {
.l
			lda link_syncpoint
			cmp #.arg
			bcc .l
		}

		!macro setup_sync .arg {
			lda #$00
			sta link_frame_count + 0	;avoid that the counter wraps worstcase, as we can't write atomic
			lda #<($8000 - .arg)
			sta link_frame_count + 0	;now setup counter
			lda #>($8000 - .arg)
			sta link_frame_count + 1
		}

		!macro sync {
.l
			lda link_frame_count + 1	;Counter expired when $8000 is reached
			bpl .l
		}

		!macro bus_lock {
			lda #$37			;set ATN and CLK
			sta $dd02
		}

		!macro bus_unlock .arg {
			ldx #(.arg & 3) + $c0
			stx $dd00

			lda #$3f			;release CLK
			sta $dd02

			pha
			pla
			pha
			pla
			pha
			pla
		}

		!macro set_depack_pointers .arg {
			lda #<.arg
			sta bitfire_load_addr_lo
			lda #>.arg
			sta bitfire_load_addr_hi
		}

		!macro start_music_nmi {
			ldx #<link_player
			lda #>link_player

			stx $fffa
			sta $fffb
			lda #$00
			sta $dd0e
			lda $dd0d
			lda #$c7
			sta $dd04
			lda #$4c
			sta $dd05
			lda #$81
			sta $dd0d

			lda #$ff
.l
			cmp $d012
			bne .l

			lda #$11
			sta $dd0e
		}

		!macro stop_music_nmi {
			lda #$7f
			sta $dd0d
			lda $dd0d
		}

		!macro restart_music_nmi {
			lda $dd0d
			lda #$81
			sta $dd0d
		}
