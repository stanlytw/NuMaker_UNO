@echo off
set str=%1
set str=%str:~0,2%
%str%
cd %1
%1\NuLink_M460_M2L31.exe -C 
%1\NuLink_M460_M2L31.exe -E ALL -Q
%1\NuLink_M460_M2L31.exe -W APROM %2
%1\NuLink_M460_M2L31.exe -V APROM %2
%1\NuLink_M460_M2L31.exe -S
exit 0
