# Cerberus

- First install Unreal Engine 5.0 from the Epic Games client to streamline the process of opening projects and streamlining project file generation

## **Setup Unreal Engine from source**
- **[Follow these instructions](https://www.unrealengine.com/en-US/ue4-on-github)** to gain access to the Unreal Engine source code repository on GitHub 
- Install **[Git](https://git-scm.com/downloads)** and **[Git LFS](https://git-lfs.github.com/)**
-   Install **[GitHub for Windows](https://windows.github.com/)** then clone the **[forked UnrealEngine repository](https://github.com/Cage01/UnrealEngine)**. To use Git from the command line, see the [Setting up Git](https://help.github.com/articles/set-up-git/) 
    
-   Install **Visual Studio 2022**. All desktop editions of Visual Studio 2017 can build UE4, including [Visual Studio Community 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2036&passive=false). To install the correct components for UE4 development, check the ".NET desktop development", "Desktop development with C++" and "Game Development with C++" workloads. Then in the Individual components tab search and select the "Unreal Engine Installer" and "Nuget Package Manager" optional components.
    
-   Open the Cerberus source folder in Explorer and run **Setup.bat**. This will download binary content for the engine, as well as installing prerequisites and setting up Unreal file associations. On Windows 8, a warning from SmartScreen may appear. Click "More info", then "Run anyway" to continue. It will also run through the process of creating the project files for the engine. It should take less than a minute to complete. Then finally compile the entire engine. So there should be no need to open the engine source code in an IDE
-	Right click on the Cerberus.uproject file and generate project files.
- To customize performance of the enging build. Edit this file (Unreal Engine folder location):  Engine/Saved/UnrealBuildTool/BuildConfiguration.xml and add the code below
```
<?xml version="1.0" encoding="utf-8" ?>
<Configuration xmlns="https://www.unrealengine.com/BuildConfiguration">
	<BuildConfiguration>
		<MaxParallelActions>6</MaxParallelActions>
	</BuildConfiguration>
</Configuration>
```
MaxParallelActions can be set to whatever configuration would be best for your PC. It should depend on the number of cores your CPU has available. I set mine to 6 for the time being and it improves compilation times.


**At this point you should be able run the Setup.bat file. It will prompt you to select your Unreal Engine source build location and then compile the code automatically.**
