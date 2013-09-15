main:
  lda $5
  sta #$2       ! x = 5
  lda #$0       ! for(int i = 0; i < 10; i++){
  sta $0        ! i is in Stack[0]
  lda #$10    
  sta $1        ! constant 10 pushed to stack
check:
  lda $1
  sub $0
  bl out        ! 10 - i = Negative for condition to be true
luup:
  lda #$2       ! x
  add $2        ! x += 2;
  and #$0       ! x = x & i;
  ba check
out:
  lda #$2       ! X Back into ACC for examination
