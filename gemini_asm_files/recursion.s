! Demonstrate recursion
main:
	lda #$0
        sta $0
        lda #$24
        sta $1
        jmp rec
        ba endrec
rec:
        lda $0
        sub $1
        be recdone
        lda $0
        add #$1
        sta $0
        jmp rec
recdone:
        ret
endrec:
        lda #$0
        sta $0
        lda #$25 ! here comes the stack overflow
        sta $1
        jmp rec
done:   