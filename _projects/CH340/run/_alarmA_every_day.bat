@echo off

:: hr:min:sec:day:isdow:skipWeeks

set HR=19
set MIN=45
set SEC=11
set DAY=-1
set ISWD=0
set WSKIP=0
set ALARMN=0
set ALARMTYPE=1

CH340_cpp.exe COM7 setalarm %HR%:%MIN%:%SEC%:%DAY%:%ISWD%:%WSKIP%:%ALARMN%:%ALARMTYPE%
