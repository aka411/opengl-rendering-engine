#pragma once
#include "ecs.h"
#include "../low-level/gpu_material_system.h"
#include "../low-level/shader_manager.h"
#include "../low-level/vertex_format_manager.h"
#include "../low-level/world_vertex_buffer_management_system.h"
#include "../camera.h"
#include "../components.h"
#include "world_renderer.h"

#include "../ui/include/ui_renderer.h"

#include "../animation/animation_system.h"
#include "render_command_buffer_manager.h"
#include "object_data_buffer_manager.h"

namespace Engine
{
	//This class is the composer and conductor
	class RenderSystem
	{

	private:

		ECS::ECSEngine& m_ecsEngine; // world ecs


		ShaderManager m_shaderManager;
		VertexFormatManager m_vertexFormatManager;

		WorldRenderer m_worldRenderer;//owner
		UI::UIRenderer m_uiRenderer;//owner


		WorldVertexBufferManagementSystem& m_worldVertexBufferManagementSystem;

		AnimationSystem& m_animationSystem;


		GPUBufferManager& m_gpuBufferManager;

		RenderCommandBufferManager m_renderCommandBufferManager;
		ObjectDataBufferManager m_objectDataBufferManager;



	public:

		RenderSystem(ECS::ECSEngine& ecsEngine, WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, GPUMaterialSystem& gpuMaterialSystem, UI::UICoreSystem& uiCoreSystem, AnimationSystem& animationSystem, GPUBufferManager& gpuBufferManager);

		void render(Engine::Camera& camera);

		void renderUI();
	};

}