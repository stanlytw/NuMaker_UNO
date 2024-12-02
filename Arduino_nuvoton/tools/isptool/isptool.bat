@echo off
set str=%1
set str=%str:~0,2%
%str%
cd %1
%1\isp_command_line.exe -a %2
exit 0
