@echo off

:: hr:min:sec:day:isdow:skipWeeks

set HR=9
set MIN=39
set SEC=11
set DAY=-1
set ISWD=0
set WSKIP=0
set ALARM=0

CH340_cpp.exe COM8 setalarm %HR%:%MIN%:%SEC%:%DAY%:%ISWD%:%WSKIP%:%ALARM%
