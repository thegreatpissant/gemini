int main(void){
   int x = 0;
   int y = 4;
   int z = 1;
   for(; x <= 13; x++){  // Possible without ble or bge
     y = x + y - z;
     if(y == 9)
      goto rehash;
    goto out;
  }
rehash:
  y = x & z | y;
out:
  // Place y into the ACC to examine
}
