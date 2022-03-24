Set sh = CreateObject("WScript.Shell")
Set shortcut = sh.CreateShortcut("C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector_shortcut.lnk")
shortcut.TargetPath = """C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe"""
shortcut.Arguments = "-fileassociations"
shortcut.Save