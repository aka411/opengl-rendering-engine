#pragma once
#include "../../include/low-level/rendering_system_data_types.h"
#include "../low-level/world_vertex_buffer_management_system.h"
#include "../low-level/shader_manager.h"
#include "../low-level/vertex_format_manager.h"
#include "../components.h"
#include "../camera.h"

class WorldRenderer
{

private:

	WorldVertexBufferManagementSystem& m_worldVertexBufferManagementSystem;
	VertexFormatManager& m_vertexFormatManager;
	ShaderManager m_shaderManager;

	GPUMaterialSystem& m_gpuMaterialSystem;

	GPUBufferManager m_gpuBufferManager;

	GPUBufferInfo m_cameraBufferInfo;
	GPUBufferInfo m_objectBufferInfo;


public:

	WorldRenderer(VertexFormatManager& m_vertexFormatManager,WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, GPUMaterialSystem& gpuMaterialSystem);

	void render(std::unordered_map<VertexFormat, std::vector<RenderCommand>>& vertexFormatToRenderCommands, Engine::Camera& camera);


};