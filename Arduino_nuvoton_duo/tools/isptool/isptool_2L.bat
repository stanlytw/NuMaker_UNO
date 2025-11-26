@echo off
set str=%1
set str=%str:~0,2%
isp_command_line.exe -o USB -l
pause




