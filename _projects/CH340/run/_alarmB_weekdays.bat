@echo off

:: hr:min:sec:day:isdow:skipWeeks

:: DAYS: 1 = Mon, 2 = Tue, 3 = Wed, 4 = Thu, 5 = Fri, 6 = Sat, 7 = Sun

:: WSKIP = 2  : every 3 weeks

set HR=13
set MIN=25
set SEC=0
set DAY=4
set ISWD=1
set WSKIP=0
set ALARM=1

CH340_cpp.exe COM8 setalarm %HR%:%MIN%:%SEC%:%DAY%:%ISWD%:%WSKIP%:%ALARM%
