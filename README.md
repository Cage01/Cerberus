# Cerberus

**Setup Unreal Engine from source**
- **[Follow these instructions](https://www.unrealengine.com/en-US/ue4-on-github)** to gain access to the Unreal Engine source code repository on GitHub 
- Install **[Git](https://git-scm.com/downloads)** and **[Git LFS](https://git-lfs.github.com/)**
-   Install **[GitHub for Windows](https://windows.github.com/)** then **[fork and clone our repository](https://guides.github.com/activities/forking/)**. To use Git from the command line, see the [Setting up Git](https://help.github.com/articles/set-up-git/) and [Fork a Repo](https://help.github.com/articles/fork-a-repo/) articles.
    
    If you'd prefer not to use Git, you can get the source with the 'Download ZIP' button on the right. The built-in Windows zip utility will mark the contents of zip files downloaded from the Internet as unsafe to execute, so right-click the zip file and select 'Properties...' and 'Unblock' before decompressing it. Third-party zip utilities don't normally do this.
    
-   Install **Visual Studio 2022**. All desktop editions of Visual Studio 2017 can build UE4, including [Visual Studio Community 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2036&passive=false). To install the correct components for UE4 development, check the ".NET desktop development", "Desktop development with C++" and "Game Development with C++" workloads. Then in the Individual components tab search and select the "Unreal Engine Installer" and "Nuget Package Manager" optional components.
    
-   Open your source folder in Explorer and run **Setup.bat**. This will download binary content for the engine, as well as installing prerequisites and setting up Unreal file associations. On Windows 8, a warning from SmartScreen may appear. Click "More info", then "Run anyway" to continue.
    
    A clean download of the engine binaries is currently 3-4gb, which may take some time to complete. Subsequent checkouts only require incremental downloads and will be much quicker.
    
-   Run **GenerateProjectFiles.bat** to create project files for the engine. It should take less than a minute to complete.
    
-   Load the project into Visual Studio by double-clicking on the **UE4.sln** file. Set your solution configuration to **Development Editor** and your solution platform to **Win64**, then right click on the **UE4** target and select **Build**. It may take anywhere between 10 and 40 minutes to finish compiling, depending on your system specs.

**Setup Project files**
