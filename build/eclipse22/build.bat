@echo off 

set PATH=C:/Users/mopoc/AppData/Roaming/xPacks/windows-build-tools/xpack-windows-build-tools-4.3.0-1/bin;%PATH%
set PATH=C:/Users/mopoc/AppData/Roaming/xPacks/arm-none-eabi-gcc/xpack-arm-none-eabi-gcc-11.3.1-1.1/bin;%PATH%


::echo %PATH% 


arm-none-eabi-gcc.exe -v

make -v

