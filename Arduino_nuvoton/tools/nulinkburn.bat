@echo off
set str=%1
set str=%str:~0,2%
%str%
cd %1
%1\NuLink -update
%1\NuLink -e all
%1\NuLink -w aprom %2
%1\NuLink -reset
exit 0
