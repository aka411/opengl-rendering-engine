

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
#include "../include/ui/include/systems/performance_monitor_system.h"




void GLAPIENTRY DebugMessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	
	if (type == GL_DEBUG_TYPE_OTHER || type == GL_DEBUG_TYPE_PERFORMANCE)
		return;

	
	fprintf(stderr, "GL Debug Message:\n");
	fprintf(stderr, "  Source: 0x%x\n", source);
	fprintf(stderr, "  Type: 0x%x\n", type);
	fprintf(stderr, "  ID: %d\n", id);
	fprintf(stderr, "  Severity: 0x%x\n", severity);
	fprintf(stderr, "  Message: %s\n", message);
	fprintf(stderr, "------------------------------------------------\n");

	// Crash the program if it's a high-severity error
	if (severity == GL_DEBUG_SEVERITY_HIGH)
	{
		
		// __debugbreak(); // Windows-specific
	}
}

SDL_Window* init()
{
	SDL_Init(SDL_INIT_VIDEO);
	// Before creating the window

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_Window* window = SDL_CreateWindow("Opengl Rendering Engine(openGL 4.6)",1000, 700, SDL_WINDOW_OPENGL);
	assert(window != nullptr);
	SDL_GLContext  gl_context = SDL_GL_CreateContext(window);
	assert(gl_context != nullptr);
	SDL_GL_MakeCurrent(window, gl_context);


	gladLoadGL();



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

	using namespace Engine;


	SDL_Window* window = init();


	 SDL_Event event;

	 bool running = true;

	 GLint flags;
	 glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	 if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	 {
		 
		 glEnable(GL_DEBUG_OUTPUT);


		 glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);


		 glDebugMessageCallback(DebugMessageCallback, NULL);


		 glDebugMessageControl(
			 GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
			 0, NULL, GL_FALSE
		 );

		 fprintf(stdout, "OpenGL Debug Output successfully enabled.\n");
	 }
	

	 Engine::Camera camera;
	
	 camera.setPerspectiveProjection(60.0f, static_cast<float>(1000) / static_cast<float>(700), 0.01f, 2000.0f);
	 EngineCore engineCore;

	 
	 UI::UIElement uiWindow = engineCore.getUIBuilder()
		 .createUIWindow()
		 .withPosition({ 0,0,0 })
		 .withRectDimensions(700, 700)
		 .build();

	 UI::UIElement panel1 = engineCore.getUIBuilder()
		 .createUIPanel()
		 .withColour({ 23.0 / 255.0, 23.0 / 255.0, 23.0 / 255.0,0.6 })
		 .withPosition({ 0,0,0 })
		 .withRectDimensions(200, 100)
		 .build();



	 UI::UIElement fpsGraph = engineCore.getUIBuilder().createUIGraph()
		 .withColour({ 0.0,1.0,0.0,0.8 })
		 .withPosition({ 0,0,1 })
		 .withRectDimensions(200,100)
		 .build();
	 



	 // Base path to the model
	 const char* modelPath = "Path TO GLTF File";
	 ECS::ECSEngine& ecsEngine = engineCore.getECSEngine();


	 ECS::EntityId rootEntity = engineCore.loadModel(modelPath);





	 uiWindow.addChild(panel1);
	 panel1.addChild(fpsGraph);
	

	 


	 PerformanceMonitorSystem performanceMonitorSystem(engineCore.getUICoreSystem());

	 performanceMonitorSystem.setUp(fpsGraph);


	

	 float deltaTime = 0.0f;
	 float lastFrameTime = SDL_GetTicks() / 1000.0f;
	 float currentFrameTime = lastFrameTime;
	 float accumulator = 0;
	 float frameNumber = 1;
	 performanceMonitorSystem.UpdateFPSMeter(0);



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

		 frameNumber++;
		
		 accumulator += deltaTime;
		 if (accumulator > 1/30.0)
		 {
			
			 performanceMonitorSystem.UpdateFPSMeter(accumulator/ frameNumber);
			 accumulator = 0;
			 frameNumber =1;
		
		

		 }



		 engineCore.update(deltaTime);
		 engineCore.render(camera);
		 engineCore.renderUI();


		 //buffer swap
		 SDL_GL_SwapWindow(window);
	
	 }



	 















	SDL_Delay(20);

	//dont call renderer after this point, as the window will be destroyed
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}
