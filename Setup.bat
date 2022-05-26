@echo off

rem ## Run post-checkout hook
git config core.hooksPath .githooks
git checkout

rem ## Unreal Engine path is unknown, select path and setup environment
set ProjectConfig=.uepath

if not exist %ProjectConfig% (
	echo You will be prompted to select your Unreal Engine Source Directory.
	echo ==========
	pause
	goto :LogPath
)

rem ## Unreal Engine path has already been set, just compile code
if exist %ProjectConfig% (
	goto :SetupEnvironment
)


:LogPath
rem ## Open Folder dialog to select Unreal Engine path
Title Folder Selection
call:FolderSelection "%SourcePath%", SourcePath, "Select Folder"
echo %SourcePath%>%ProjectConfig%

:SetupEnvironment
set ProjectPath=%~dp0
set /p UnrealPath=<%ProjectConfig%

echo Engine Path - %UnrealPath%
rem ## Generate project files
if not exist "%UnrealPath%\UE5.sln" (
	echo Generating UE5 project files...
	call %UnrealPath%\Setup.bat
	call %UnrealPath%\GenerateProjectFiles.bat
)

rem ## Build Engine source code
echo Compiling Unreal Engine source code...
call %UnrealPath%\Engine\Build\BatchFiles\RunUAT.bat BuildEditor
exit/B 0


Rem ---------------------------------------------------------------------------------------------------------
:FolderSelection <SelectedPath> <folder> <Description>
SetLocal enableextensions disabledelayedexpansion & set "folder=%~1"
set "dialog=powershell -sta "Add-Type -AssemblyName System.windows.forms^
|Out-Null;$f=New-Object System.Windows.Forms.FolderBrowserDialog;$f.SelectedPath='%~1';$f.Description='%~3';^
$f.ShowNewFolderButton=$false;$f.ShowDialog();$f.SelectedPath""
for /F "delims=" %%I in ('%dialog%') do set "res=%%I"



EndLocal & (if "%res%" EQU "" (set "%2=%folder%") else (set "%2=%res%"))
exit/B 0
Rem ---------------------------------------------------------------------------------------------------------
