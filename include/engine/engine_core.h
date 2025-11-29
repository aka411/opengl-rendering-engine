#pragma once
#include "../low-level/gpu_texture_manager.h"
#include "../low-level/gpu_material_system.h"
#include "../low-level/world_vertex_buffer_management_system.h"
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "engine_loader.h"
#include "../rendering-system/render_system.h"
#include <string>

class EngineCore
{
private:

	TheEngine::ECS::ECSEngine m_ecsEngine;
	TheEngine::ECS::ECSEngine m_uiECSEngine;

	GPUBufferManager m_gpuBufferManager;

	GPUTextureManager m_gpuTextureManager;
	GPUMaterialSystem m_gpuMaterialSystem;// ---> needsGPUBuffermanager

	WorldVertexBufferManagementSystem m_worldVertexBufferManagementSystem;

	EngineLoader m_engineLoader;





	RenderSystem m_renderSystem;





public:
	EngineCore();

	void loadModel(std::string pathToFile);
	void render(Engine::Camera camera);


	void renderUI();

	//UI 
	//getUIBuilder();

	


};