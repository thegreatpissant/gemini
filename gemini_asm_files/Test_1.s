main:
  lda #$0 ! x 
  sta $0
  lda #$0 ! g
  sta $1
  lda #$7 ! h
  sta $2
  lda #$10 ! j
  sta $3
  lda #$15 ! k
  sta $4
  lda #$15 ! l
  sta $5
  lda #$10 ! y
  sta $6
  lda #$0 ! i
  sta $7
for:
  lda $7
  sub $6
  be forend
  lda $7 ! i
  mul $6 ! * y
  add $0 ! += x
  sta $0 ! x = acc
  lda $2 ! h
  and $3 ! & j
  and $7 ! & i
  or  $4 ! | k
  sta $1
  nota
  sta $1
  lda $7  ! load i
  add #$1
  sta $7
  ba for
forend:
  lda $0
  and #$10
  sub $6
  bl end
  lda #$0
  sta $0
end:
