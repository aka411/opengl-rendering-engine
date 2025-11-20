#pragma once
#include "../low-level/gpu_texture_manager.h"
#include "../low-level/gpu_material_system.h"
#include "../low-level/world_vertex_buffer_management_system.h"
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "engine_loader.h"

class EngineCore
{
private:

	TheEngine::ECS::ECSEngine m_ecsEngine;

	GPUBufferManager m_gpuBufferManager;

	GPUTextureManager m_gpuTextureManager;
	GPUMaterialSystem m_gpuMaterialSystem;// ---> needsGPUBuffermanager

	WorldVertexBufferSystem m_worldVertexBufferSystem;

	EngineLoader engineLoader;

public:
	EngineCore();

};