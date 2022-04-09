@echo off
echo You will be prompted to select your Unreal Engine Source Directory.
echo ==========
pause

rem ## Open Folder dialog to select Unreal Engine path
Title Folder Selection
call:FolderSelection "%SourcePath%", SourcePath, "Select Folder"

rem ## Update path in the buildsrouce script and run it
set $source=buildsource.bat
set $Dest=buildsource.bat

set "search=NULL"

for /f "delims=" %%a in ('powershell -c "(get-content '%$source%') | foreach-object {$_ -replace '%Search%', '%SourcePath%'} | set-content '%$dest%'"') do echo %%a

call "buildsource.bat"
EXIT

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
