# OpenGLTestModel
## Project Description:
This program utilizes OpenGL to create a 3d model that is controllable by the user. Once in first person mode, the user can see objects in the environment. Additionally, in first person mode the camera follows the model, in both movement and rotation.
## How to Run:
Dowload all files in directory and run the file RUNME.exe, a window will open and the program will be playable.
### Controls:
* WASD - Used for movement of the model.
* C - Changes perspective between fly cam and dynamic first person. On start niether will be activated, after the first use of this input it will swap between the two. 
* FV - Moves camera forward and backward while in fly cam mode.
* Arrow Keys - controls where the camera looks during fly cam mode


## Main project files are:
* main.cpp
  * This file initalizes the window, and shader. It responds to keyboard input listed in [Controls](#Controls). Additionally, it reshapes the viewport if the window is resized.
* Model.cpp
  * This file contains the class of Model and how it is rendered and updated.
* Shader.cpp
  * This file contains all the logic of shaders and allows them to be loaded from specific files.
* tiny_obj_loader.cpp
  * This file handles loading objects into the environment from verticies.
