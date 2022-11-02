@echo off
set str=%1
set str=%str:~0,2%
%str%
cd %1
%1\NuLink_M460 -C 
%1\NuLink_M460 -E ALL
%1\NuLink_M460 -W APROM %2
%1\NuLink_M460 -V APROM %2
%1\NuLink_M460 -S
exit 0
