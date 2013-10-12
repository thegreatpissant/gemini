// Test 1 - Test basic operation

int main(void){
  int x = 0;
  int g = 0x0, h = 0x7, j = 0xa, k = 0xf, l = 0xf;
  int y = 10;
  for(int i = 0; i < y; i++){
    x += y * i;
    g = h & j & i | k;
    g = !g;
  }
  if(x & 0xa > y){
    x = 0;
  }
}

