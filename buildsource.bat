@echo off

set ProjectPath=%~dp0
set UnrealPath="NULL"

echo Engine Path - %UnrealPath%
chdir %UnrealPath%
rem ## Generate project files
echo Running UE5 Build Tool...
if not exist "%ProjectPath%Cerberus.sln" start /wait Engine\Binaries\Win64\UnrealVersionSelector-Win64-Shipping.exe /quiet

rem ## Build Engine source code
call Engine\Build\BatchFiles\RunUAT.bat BuildEditor
pause
exit
