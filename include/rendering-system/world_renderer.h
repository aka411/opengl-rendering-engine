#pragma once
#include "../../include/low-level/rendering_system_data_types.h"
#include "../low-level/world_vertex_buffer_management_system.h"
#include "../low-level/shader_manager.h"
#include "../low-level/vertex_format_manager.h"
#include "../components.h"
#include "../camera.h"
#include "../animation/animation_system.h"

#include "world_renderer_data_structures.h"
#include "render_command_buffer_manager.h"
#include "object_data_buffer_manager.h"

namespace Engine
{

	class WorldRenderer
	{

	private:

		WorldVertexBufferManagementSystem& m_worldVertexBufferManagementSystem;
		VertexFormatManager& m_vertexFormatManager;
		ShaderManager m_shaderManager;

		GPUMaterialSystem& m_gpuMaterialSystem;

		GPUBufferManager m_gpuBufferManager;

		GPUBufferInfo m_cameraBufferInfo;


		AnimationSystem& m_animationSystem;


		RenderCommandBufferManager& m_renderCommandBufferManager;
		ObjectDataBufferManager& m_objectDataBufferManager;

	public:

		WorldRenderer(VertexFormatManager& vertexFormatManager, WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, GPUMaterialSystem& gpuMaterialSystem, AnimationSystem& animationSystem, RenderCommandBufferManager& m_renderCommandBufferManager,
		ObjectDataBufferManager& m_objectDataBufferManager);


		void startFrame(Engine::Camera& camera);

		//void render(std::unordered_map<VertexFormat, std::vector<RenderCommand>>& vertexFormatToRenderCommands);
		
		void IndirectDrawArray(const VertexFormat vertexFormat, const size_t byteOffset, const size_t count);
		void IndirectDrawIndexed(const VertexFormat vertexFormat, const IndexType indexType, const size_t byteOffset, const size_t count);
		

		void endFrame();



	};

}