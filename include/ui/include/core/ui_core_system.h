#pragma once
#include "../systems/ui_vertex_buffer_management_system.h"
#include "../text/ui_font_manager.h"
#include "../../../low-level/gpu_buffer_manager.h"
#include "ecs.h"
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

	    ECS::ECSEngine& m_ecsEngine;//not owner

	public:

		UICoreSystem(ECS::ECSEngine& ecsEngine, GPUBufferManager& gpuBufferManager);



		ECS::ECSEngine& getECSEngine();

		UIVertexBufferManagementSystem& getUIVertexBufferManagementSystem();

		UIFontManager& getUIFontManager();

		UIShaderManager& getUIShaderManager();

	};

}