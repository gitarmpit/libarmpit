@echo off

:: hr:min:sec:day:isdow:skipWeeks

set HR=-1
set MIN=-1
set SEC=3
set DAY=-1
set ISWD=0
set WSKIP=0
set ALARMN=1
set ALARMTYPE=1

:: every minute at 11 second mark
CH340_cpp.exe COM%1 setalarm %HR%:%MIN%:%SEC%:%DAY%:%ISWD%:%WSKIP%:%ALARMN%:%ALARMTYPE%
