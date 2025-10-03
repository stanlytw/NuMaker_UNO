@echo off
set str=%1
set str=%str:~0,2%
%str%
cd %1
%1\isp_command_line.exe -o USB 2 -l
%1\isp_command_line.exe -o USB 0 -t 3
%1\isp_command_line.exe -o USB 0 -a %2
%1\isp_command_line.exe -o USB 0 -t 1
%1\isp_command_line.exe -o USB 0 -j
%1\isp_command_line.exe -o USB 2 -t 1
exit 0




