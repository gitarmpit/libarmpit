@echo off

:: hr:min:sec:day:isdow:skipWeeks

:: DAYS: 1 = Mon, 2 = Tue, 3 = Wed, 4 = Thu, 5 = Fri, 6 = Sat, 7 = Sun

set HR=10
set MIN=0
set SEC=0
set DAY=4
set ISWD=1
set WSKIP=0
set ALARMN=0
set ALARMTYPE=1

CH340_cpp.exe COM8 setalarm %HR%:%MIN%:%SEC%:%DAY%:%ISWD%:%WSKIP%:%ALARMN%:%ALARMTYPE%