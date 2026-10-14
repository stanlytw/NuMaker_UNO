echo "This is LDR bat file. GO"
@echo off
set str=%1
set str=%str:~0,2%
%str%
cd %1
%1\NuLink_M460_M2L31.exe -C 
%1\NuLink_M460_M2L31.exe -E ALL -Q
%1\NuLink_M460_M2L31.exe -W LDROM %2
%1\NuLink_M460_M2L31.exe -W CFG 0xFFFFFF7F, 0xFFFFFFFF, 0xFFFF5A5A, 0xFFFFFFFF
%1\NuLink_M460_M2L31.exe -W APROM %3
%1\NuLink_M460_M2L31.exe -V APROM %3
%1\NuLink_M460_M2L31.exe -S
exit 0
exit 0
