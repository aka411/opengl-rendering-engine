#pragma once
#include "../low-level/gpu_texture_manager.h"
#include "../low-level/gpu_material_system.h"
#include "../low-level/world_vertex_buffer_management_system.h"
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "engine_loader.h"
#include "../rendering-system/render_system.h"
#include <string>
#include "../low-level/gpu_buffer_manager.h"
#include "../ui/include/core/ui_system.h"
#include "../ui/include/builder/ui_builder.h"
#include "../transformation_system.h"



class EngineCore
{
private:

	TheEngine::ECS::ECSEngine m_ecsEngine;//owner
	

	GPUBufferManager m_gpuBufferManager;

	GPUTextureManager m_gpuTextureManager;//owner

	GPUMaterialSystem m_gpuMaterialSystem;// ---> needs GPUBufferManager

	WorldVertexBufferManagementSystem m_worldVertexBufferManagementSystem;//-->needs GPUBufferManager


	EngineLoader m_engineLoader;



	/***MODEL RENDERING SYSTEMS***/


	TransformationSystem m_transformationSystem;




	/**UI SYSTEM**/

	UI::UICoreSystem m_uiCoreSystem; // needs ECSEngine, GPUBufferManager
	UI::UISystem m_uiSystem;//owner


	RenderSystem m_renderSystem;// contains both world renderer and ui renderer


	UI::UIBuilder m_uiBuilder;


public:



	EngineCore();

	void loadModel(std::string pathToFile);
	void render(Engine::Camera camera);

	void update();

	void renderUI();

	/***** UI SYSTEM *****/
	UI::UIBuilder& getUIBuilder();

	UI::UICoreSystem& getUICoreSystem();
	


};