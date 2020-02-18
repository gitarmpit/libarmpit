:: this is the 64 bit version but this dll only works with ttf files, 
:: it generates wrong characters for .fon / .fft files
gcc -I freetype/include fontconvert.c  freetype.dll  -o fc
