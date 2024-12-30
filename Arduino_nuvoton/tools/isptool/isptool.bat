@echo off
set str=%1
set str=%str:~0,2%
%str%
cd %1
%1\isp_command_line.exe -l
sleep
%1\isp_command_line.exe -a %2
sleep 
%1\isp_command_line.exe -j
sleep_q 
exit 0




