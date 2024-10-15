@echo off

:: hr:min:sec:day:isdow:skipWeeks

set HR=-1
set MIN=53
set SEC=22
set DAY=-1
set ISWD=0
set WSKIP=0
set ALARM=0

CH340_cpp.exe COM8 setalarm %HR%:%MIN%:%SEC%:%DAY%:%ISWD%:%WSKIP%:%ALARM%
