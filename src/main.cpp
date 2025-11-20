

#include <string>
#include <iostream>


#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include "../glad/glad.h"


#include <filesystem>
#include <fstream>
#include <cassert>
#include "../include/camera.h"
#include "../include/engine/engine_core.h"

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

	













int main(int argc, char* args[])
{




	SDL_Window* window = init();


	 SDL_Event event;

	 bool running = true;

	
	

	 Engine::Camera camera;
	
	

	 EngineCore engineCore;









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

		 









		 //buffer swap
		 SDL_GL_SwapWindow(window);

		
	 }



	 















	SDL_Delay(20);

	//dont call renderer after this point, as the window will be destroyed
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}
