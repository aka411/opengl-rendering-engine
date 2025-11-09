

#include <string>
#include <iostream>


#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include "../glad/glad.h"

#include "../include/transformation_system.h"
#include "../include/renderer.h"
#include "../include/gltf_flat_parser.h"
#include "../include/skin_animation_system.h"
#include "../include/bone_animation_system.h"


SDL_Window* init()
{
	SDL_Init(SDL_INIT_VIDEO);
	// Before creating the window

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_Window* window = SDL_CreateWindow("Opengl Rendering Engine(openGL 4.6)",700, 700, SDL_WINDOW_OPENGL);
	assert(window != nullptr);
	SDL_GLContext  gl_context = SDL_GL_CreateContext(window);
	assert(gl_context != nullptr);
	SDL_GL_MakeCurrent(window, gl_context);






	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		SDL_Log("Failed to initialize GLAD");
		assert(false);//stop here
		return nullptr;
	}

	return window;
}

	


tinygltf::Model loadModel()
{

tinygltf::Model model;
tinygltf::TinyGLTF loader;
std::string err;
std::string warn;
std::string filePath = "ADD PATH TO GLTF FILE";



bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filePath);
//bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filePath); // for binary glTF(.glb)

if (!warn.empty())
{
	printf("Warn: %s\n", warn.c_str());
}

if (!err.empty())
{
	printf("Err: %s\n", err.c_str());
	assert(0);
}

if (!ret)
{
	printf("Failed to parse glTF\n");
	assert(0);
}

std::cout << "Loaded GLTF successfully" << std::endl;
return model;

}










int main(int argc, char* args[])
{



	tinygltf::Model model = loadModel();

	SDL_Window* window = init();

	GPUBufferManager gpuBufferManager;
	GPUTextureManager gpuTextureManager;

	BufferManagementSystem bufferManagementSystem;

	Engine::GLTFFlatParser gltfFlatParser(bufferManagementSystem, gpuTextureManager);

	Engine::Model engineModel = gltfFlatParser.parse(model);


	 SDL_Event event;


	 bool running = true;

	 TransformationSystem g_transfromationSystem;
	 
	 

	 Engine::Camera camera;
	 Renderer renderer(bufferManagementSystem, gpuTextureManager,camera);

	 renderer.setPhysicalViewport(700, 700);
	 SkinAnimationSystem animationSystem;
	 BoneAnimationSystem boneAnimationSystem;
	 std::cout << "Going to start Rendering" << std::endl;


	 float deltaTime = 0.0f;
	 float lastFrameTime = SDL_GetTicks() / 1000.0f;
	 float currentFrameTime = lastFrameTime;

	 while (running)
	 {
		 currentFrameTime = SDL_GetTicks() / (1000.0f); // retuns time in milliseconds converted to seconds
		 deltaTime = currentFrameTime - lastFrameTime;
		 lastFrameTime = currentFrameTime;
		
		 while (SDL_PollEvent(&event))
		 {
			 
			 if (event.type == SDL_EVENT_QUIT)
			 {
				 running = false;
			 }


			 if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
			 {
				 running = false;
			 }

			 if (  event.window.type == SDL_EVENT_WINDOW_RESIZED)
			 {
				 // Note: in SDL3, data1/data2 are the new width/height
				 int width = event.window.data1;
				 int height = event.window.data2;

				 renderer.setPhysicalViewport(width, height);

			 }



			 if (event.key.scancode == SDL_SCANCODE_A)
			 {
				 camera.move(-1.0, 0.0, 0.0);
			
			 }

			 if (event.key.scancode == SDL_SCANCODE_D)
			 {
				 camera.move(1.0, 0.0, 0.0);

			 }
			 if (event.key.scancode == SDL_SCANCODE_W)
			 {
				 camera.move(0.0, 0.0, 1.0);
			 }
			 if (event.key.scancode == SDL_SCANCODE_S)
			 {
				 camera.move(0.0, 0.0, -1.0);
			 }


			 if (event.type == SDL_EVENT_MOUSE_MOTION )
			 {
				 camera.rotate(event.motion.xrel, event.motion.yrel);
				

			 }

		 }

		 animationSystem.animate(deltaTime, engineModel, "PUT ANIMATION NAME HERE");

		 g_transfromationSystem.updateWorldTransforms(engineModel);
		 boneAnimationSystem.animate(engineModel);
		 // per frame loop here


		 renderer.renderFrame(engineModel);






		 //buffer swap
		 SDL_GL_SwapWindow(window);

		
	 }



	 















	SDL_Delay(20);

	//dont call renderer after this point, as the window will be destroyed
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}
