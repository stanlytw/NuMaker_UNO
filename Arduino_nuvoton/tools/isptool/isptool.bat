@echo off
set str=%1
set str=%str:~0,2%
%str%
cd %1
%1\isp_command_line.exe -l
%1\isp_command_line.exe -t 3
%1\isp_command_line.exe -a %2
%1\isp_command_line.exe -t 1
%1\isp_command_line.exe -j
%1\isp_command_line.exe -t 1
exit 0




