@echo off

:: hr:min:sec:day:isdow:skipWeeks

set HR=0
set MIN=0
set SEC=0
set DAY=0
set ISWD=0
set WSKIP=0
set ALARM=%2

CH340_cpp.exe COM%1 setalarm %HR%:%MIN%:%SEC%:%DAY%:%ISWD%:%WSKIP%:%ALARM%:0
