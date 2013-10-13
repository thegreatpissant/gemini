!example sethi and setlo
main:
        lda #$0
        sethi #$0
        sethi #$1
	lda #$32766
        setlo #$0
	lda #$24000
        setlo #$1
        mulsl
        ldhi #$0
        ldlo #$0
        ldhi #$1
        ldlo #$1
        ldhi #$2  ! error, this does not exist in cpu.