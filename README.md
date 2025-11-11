# Opengl-Rendering-Engine

This is a simple opengl rendering system that is currently being developed.Currently I am working on a demo scene and UI system for this.
The Renderer as of now can : 
- Load simple gltf models and render them.
- move camera with mouse and keyboard keys WASD.
- Run skeletal animation and keyframe animation.
  
I have added the build instructions to build and run it below, the build and run instructions may seem a little verbose cause the project is **work in progress**, but if you follow it correctly it should work.

_If you have any issue or need help don't hesitate to contact me or raise an issue._

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

#### 2. Configure Model Path
   
Open `src/main.cpp` and locate the line for `std::string filePath`.

- Replace `"ADD PATH TO GLTF FILE"` with the actual path to your model (e.g., `"C:/Models/mymodel.gltf"`).
```cpp
// Near line 61 in src/main.cpp
std::string filePath = "YOUR/ACTUAL/PATH/TO/model.gltf";
```
**Note:** For a `.glb` (binary glTF) file, you must comment out the *LoadASCIIFromFile line* (`bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filePath);`) and uncomment the *LoadBinaryFromFile* line (`bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filePath); // for binary glTF(.glb)`).

#### 3. Configure Animation (Optional)
If your model has skeletal animation and you want it to play, find the animationSystem.animate line (around line 200 in main.cpp) and replace the third parameter with the exact name of the animation you want to play.

```cpp
// Near line 200 in src/main.cpp
animationSystem.animate(deltaTime, engineModel, "YOUR_ANIMATION_NAME");
```

#### 4. Build the Project
```bash
cmake -B build
cmake --build build
```
This will create the executable named opengl-rendering-engine.exe inside the `build/bin/` directory.

#### 5. Run the Renderer
```bash
./build/bin/opengl-rendering-engine
```
***Important:*** If you don't immediately see the model, use the mouse to look around and WASD keys to move. The model may be right next to or behind the initial camera position.
