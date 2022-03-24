# Cerberus

**Setup Unreal Engine from source**
- **[Follow these instructions](https://www.unrealengine.com/en-US/ue4-on-github)** to gain access to the Unreal Engine source code repository on GitHub 
- Install **[Git](https://git-scm.com/downloads)** and **[Git LFS](https://git-lfs.github.com/)**
-   Install **[GitHub for Windows](https://windows.github.com/)** then **[fork and clone our repository](https://guides.github.com/activities/forking/)**. To use Git from the command line, see the [Setting up Git](https://help.github.com/articles/set-up-git/) and [Fork a Repo](https://help.github.com/articles/fork-a-repo/) articles.
- Once the repository is cloned. Switch your branch to **ue5-main**
    
-   Install **Visual Studio 2022**. All desktop editions of Visual Studio 2017 can build UE4, including [Visual Studio Community 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2036&passive=false). To install the correct components for UE4 development, check the ".NET desktop development", "Desktop development with C++" and "Game Development with C++" workloads. Then in the Individual components tab search and select the "Unreal Engine Installer" and "Nuget Package Manager" optional components.
    
-   Open your source folder in Explorer and run **Setup.bat**. This will download binary content for the engine, as well as installing prerequisites and setting up Unreal file associations. On Windows 8, a warning from SmartScreen may appear. Click "More info", then "Run anyway" to continue.
    
    A clean download of the engine binaries is currently 3-4gb, which may take some time to complete. Subsequent checkouts only require incremental downloads and will be much quicker.
    
-   Run **GenerateProjectFiles.bat** to create project files for the engine. It should take less than a minute to complete.
    
-   Load the project into Visual Studio by double-clicking on the **UE4.sln** file. Set your solution configuration to **Development Editor** and your solution platform to **Win64**, then right click on the **UE4** target and select **Build**. It may take anywhere between 10 and 40 minutes to finish compiling, depending on your system specs.
- Edit this file (Unreal Engine folder location):  Engine/Saved/UnrealBuildTool/BuildConfiguration.xml and add the code below
```
<?xml version="1.0" encoding="utf-8" ?>
<Configuration xmlns="https://www.unrealengine.com/BuildConfiguration">
	<BuildConfiguration>
		<MaxParallelActions>6</MaxParallelActions>
	</BuildConfiguration>
</Configuration>
```
MaxParallelActions can be set to whatever configuration would be best for your PC. It should depend on the number of cores your CPU has available. I set mine to 6 for the time being and it improves compilation times.

**Setup project files**
*TODO - I will create a number of scripts in the future to do a lot of this automatically*
- Now we must generate the visual studio project files in order to compile and run the project. To do that the first step is to download and install Unreal Engine 5 from the Epic Games client.

**First try**
- Run `setup.bat` in the root of the project folder. This should hopefully generate all the project files. But you may still have to select the version of the engine to generate with (see below) and set the default application to run .uproject files with (also see below)

**If above didn't work try**
- Navigate to the new install location of the engine likely at `C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe` and create a shortcut. 
- Right click on the new shortcut and select "Properties" inside the "Target" text box add  **-fileassociations** to the end. The whole line should look something like `"C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" -fileassociations`
- We will use this shortcut to associate the .uproject file in the project directory to the Engine code
- Right click on the .uproject file and set the default app to the shortcut that was just created and edited.
- Right click on the .uproject file once again and select **"Generate Project Files"**. It will likely open a small window with a dropdown to select which engine to generate the files with. Be sure to select the version that you have cloned and compiled through GitHub.
- Once complete open the project .sln in Visual Studio or Rider and compile the project.


**At this point you should be able to compile the code in your preferred IDE, and run the program to start up unreal engine and its editor**
