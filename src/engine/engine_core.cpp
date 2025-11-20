#include "../../include/engine/engine_core.h"



EngineCore::EngineCore(): m_ecsEngine(), m_gpuBufferManager(), m_gpuTextureManager(), m_gpuMaterialSystem(m_gpuBufferManager), m_worldVertexBufferSystem(m_gpuBufferManager), engineLoader(m_ecsEngine,m_gpuTextureManager,m_gpuMaterialSystem,m_worldVertexBufferSystem)
{
	m_ecsEngine.registerComponent<EngineRenderComponent>();
	m_ecsEngine.registerComponent<EngineFatRenderComponent>();
	m_ecsEngine.registerComponent<EngineChildrenComponent>();
	m_ecsEngine.registerComponent<EngineTransformationComponent>();
	m_ecsEngine.registerComponent<RootEntityComponent>();

	TheEngine::ECS::EntityId rootEntityId = engineLoader.createRootEntity("PATH TO FILE");


}