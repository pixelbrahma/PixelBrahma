![PixelBrahma](/Resources/Branding/Logo.png?raw=true "PixelBrahma")

2D Game Engine for Windows.

Setup Instructions:

GitHub repository link
https://github.com/pixelbrahma/PixelBrahma.git

The steps to setup the project are outlined below:

The engine only requires Windows platform, OpenGL 4.5 or 4.6 supported GPU, VulkanSDK 1.3 and Python 3 or above.

Downloading the repository

clone the repository using 
“git clone --recursive https://github.com/pixelbrahma/PixelBrahma.git”.

--recursive is used to clone all defined sub-modules in the directory.

To update sub-directories manually, use “git submodule update --init”

Go to the Scripts folder. 
Run Setup.bat.

This should install all the required prerequisites. The project needs VulkanSDK which the setup will download if its not found. The Vulkan version required is 1.3 or above. 
If the setup installation does not complete, run the VulkanSDK.exe file downloaded and install the SDK. 
Alternatively, if Vulkan SDK is installed(version 1.3 or above), set the system environment variable VULKAN_SDK path to the installed directory. Python3 is also required to run this setup.

Run Setup.bat again to ensure everything required is generated. 

Run PixelBrahma.sln.

Set the configuration in Visual Studio to Release and run the project.

The Game Engine Editor should now be launched and open.

The project has python setup scripts that installs required premake5, python and Vulkan SDK. However, in certain cases, environment variables for the Vulkan SDK variable needs to be specified correctly in the system environment variables. The VULKAN_SDK path variable also needs to be set to the correct directory location. 

If the editor still fails to compile shader binaries, the external GPU has to be set as the preferred graphics processor for the editor application to run successfully - I have done this via the NVIDIA control panel and selecting the “PBEditor” application and choosing the GPU as the preferred graphics processor. This step is needed if the chosen graphics processor does not support SPIR-V. 

The windows generate projects batch file automatically generates project files for Visual Studio 2022. If using a different Visual Studio version, run the command “vendor\premake\bin\premake5.exe vs<version>” in the terminal(Example, for vs2019: vendor\premake\bin\premake5.exe vs2019) from the project root folder.

User Manual :

All the UI panels in the editor can be resized, moved, dragged and docked. The whole editor window is a dock-space and the layout can be customized as needed. This layout is saved by ImGui and will persist if the editor is closed and launched again.

The File menu consists of the following menu items:
New - Control + N shortcut.
Open - Control + O shortcut.
SaveAs - Control + Shift + S shortcut.
Save - Control + S shortcut.
Exit - Alt + F4 shortcut.

Scenes can also be loaded from the Assets panel by dragging and dropping them into the viewport panel. 

Objects can be selected by clicking on them. They are highlighted in orange when selected. 
When selected: 
W - Shows Translation Gizmo.
E - Shows Rotation Gizmo.
R - Shows Scaling Gizmo.
Q - Stops showing any Gizmos.
Control + D - Duplicated the entity.

Properties of the selected items are shown in the properties panel. Components can be added using the add component button, and removed by pressing the “+” symbol next to each component. 
Entity name can be changed from the input field at the top of the properties panel. 
Textures can be dragged and dropped onto the “Texture” button in the sprite render component to apply the texture to the sprite. 
Clicking on the axes names(“X”, “Y”, “Z”) in the Transform component will reset the value. Clicking and sliding on these buttons will increase and decrease them depending on the direction dragged.

In the Scene Hierarchy panel, Right-Clicking on an empty space will allow addition of empty entities and Right-Clicking on an entity will allow deletion of the entity. Left-Clicking on an entity will allow selection of an entity.