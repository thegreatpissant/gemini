main:
  lda #$0
  sta $0    ! x = 0
  lda #$4
  sta $1    ! y = 4
  lda #$1
  sta $2   ! z = 1
fbeg:
  lda $0
  sub #$13
  be rehash
  bg rehash
  lda $1   !y
  add $0  ! += x
  sub $2  ! -= z
  sta $1    ! y = x +y - z
  sub #$9
  be rehash
  ba out
  lda $0
  add #$1
  sta $0

rehash:
  lda $0
  and $2
  or $1
  sta $1
out:
  lda $1

