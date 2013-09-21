main:
  lda #$1
  sta $0     ! 1 into Stack[0]
  sta $255   ! 1 into Stack[255]

  add $0     ! 2 in ACC

  lda $0
  sub $0     ! 0 in ACC

  lda $0
  and $255   ! 1 & 1 is 1 in ACC

  lda #$0
  or $0      ! 1 | 0 is 1 in ACC
 
  lda #$0
  nota       ! !0 in ACC

  nop
