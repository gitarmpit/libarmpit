:: this is the 64 bit version but this dll only works with ttf files, 
:: it generates wrong characters for .fon / .fft files
set PATH=C:\msys64\mingw64\bin;%PATH%
gcc -I freetype/include fontconvert.c  libfreetype-6.dll -o fc
