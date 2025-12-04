# Opengl-Rendering-Engine

This is a simple opengl rendering system that is currently being developed.Currently I am working on a demo scene and UI system for this.
The Renderer as of now can : 
- Load simple gltf models and render them.
- move camera with mouse and keyboard keys WASD.
- Run skeletal animation and keyframe animation.(Temporarily disabled while integrating with ECS).
  
I have added the build instructions to build and run it below, the build and run instructions may seem a little verbose cause the project is **work in progress**, but if you follow it correctly it should work.

_If you have any issue or need help don't hesitate to contact me or raise an issue._
## Rendering Demo

https://github.com/user-attachments/assets/5fd0ce10-e963-4496-8abc-d5d17ad4cae4


## Build And Run Instructions
### Prerequisites
To build and run this project, you will need the following tools and dependencies installed on your system.

- **C++ Toolchain :** C++ compiler supporting the C++17 standard or newer (e.g., GCC, Clang, or MSVC).
- **Build System :** CMake (Minimum Required Version: 3.14) is used as the meta-build system to generate native build files.
  
#### 1. Clone & Navigate

````bash
git clone https://github.com/aka411/opengl-rendering-engine.git
cd opengl-rendering-engine
````
  

#### 2. Configure Path To Model To be Loaded
You need to add the filepath to the model to be loaded in `main.cpp`

```cpp
// Near line 110 in src/main.cpp
	engineCore.loadModel("PATH TO GLTF FILE");
```

#### 3. Build the Project
```bash
cmake -B build
cmake --build build
```
This will create the executable named opengl-rendering-engine.exe inside the `build/bin/` directory.

#### 4. Run the Renderer
```bash
./build/bin/opengl-rendering-engine
```
***Important:*** If you don't immediately see the model, use the mouse to look around and WASD keys to move. The model may be right next to or behind the initial camera position.
