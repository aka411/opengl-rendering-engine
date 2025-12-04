#pragma once
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "../low-level/gpu_material_system.h"
#include "../low-level/shader_manager.h"
#include "../low-level/vertex_format_manager.h"
#include "../low-level/world_vertex_buffer_management_system.h"
#include "../camera.h"
#include "../components.h"
#include "world_renderer.h"
#include <map>
#include "../ui/include/ui_renderer.h"
#include "../ui/include/ui_renderer.h"


//This class is the composer and conductor
class RenderSystem
{

private:

	TheEngine::ECS::ECSEngine& m_ecsEngine; // world ecs


	ShaderManager m_shaderManager;
	VertexFormatManager m_vertexFormatManager;

	WorldRenderer m_worldRenderer;//owner
	UI::UIRenderer m_uiRenderer;//owner


	WorldVertexBufferManagementSystem& m_worldVertexBufferManagementSystem;





public:

	RenderSystem(TheEngine::ECS::ECSEngine& ecsEngine, WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, GPUMaterialSystem& gpuMaterialSystem,UI::UICoreSystem& uiCoreSystem);

	void render(Engine::Camera& camera); 

	void renderUI();
};