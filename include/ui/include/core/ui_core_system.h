#pragma once
#include "../systems/ui_vertex_buffer_management_system.h"
#include "../text/ui_font_manager.h"
#include "../../../low-level/gpu_buffer_manager.h"
#include "../../../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "ui_shader_manager.h"

namespace UI
{
	class UICoreSystem
	{
	private:
	
		//owner
		UIVertexBufferManagementSystem m_uiVertexBufferManagementSystem;//requires GPUBufferManager

		UIFontManager m_uiFontManager; // owner
		UIShaderManager m_uiShaderManager;//owner

	    TheEngine::ECS::ECSEngine& m_ecsEngine;//not owner

	public:

		UICoreSystem(TheEngine::ECS::ECSEngine& ecsEngine, GPUBufferManager& gpuBufferManager);



		TheEngine::ECS::ECSEngine& getECSEngine();

		UIVertexBufferManagementSystem& getUIVertexBufferManagementSystem();

		UIFontManager& getUIFontManager();

		UIShaderManager& getUIShaderManager();

	};

}