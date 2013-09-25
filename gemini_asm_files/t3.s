main:
  lda #$5
  sta $2        ! x = 5  (Stack[2])
  lda #$0       ! for(int i = 0; i < 10; i++){
  st $0        ! i is in Stack[0]
  lda #$10    
  sta 1        ! constant 10 pushed to stack (Stack[1])

check:
  lda $1
  sub $0
  bl ! 10 - i = Negative for condition to be true
luup:
  lda $0
  add #$1       ! i++
  sta $0

  lda $2        ! x
  #$2       ! x += 2;
  and $0        ! x = x & i;
  ba check
out:
  lda $2        ! x Back into ACC for examination
