#include "../../include/engine/engine_core.h"




EngineCore::EngineCore(): 
	m_ecsEngine(),
	m_gpuBufferManager(),
	m_gpuTextureManager(),

	m_gpuMaterialSystem(m_gpuBufferManager),
	m_worldVertexBufferManagementSystem(m_gpuBufferManager), 

	m_engineLoader(m_ecsEngine,m_gpuTextureManager,
	m_gpuMaterialSystem,m_worldVertexBufferManagementSystem),

	m_renderSystem(m_ecsEngine, m_uiECSEngine, m_worldVertexBufferManagementSystem, m_gpuMaterialSystem)
{



}


void EngineCore::loadModel(std::string pathToFile)
{

	m_ecsEngine.registerComponent<EngineRenderComponent>();
	m_ecsEngine.registerComponent<EngineFatRenderComponent>();
	m_ecsEngine.registerComponent<EngineChildrenComponent>();
	m_ecsEngine.registerComponent<EngineTransformationComponent>();
	m_ecsEngine.registerComponent<RootEntityComponent>();

	TheEngine::ECS::EntityId rootEntityId = m_engineLoader.createRootEntity(pathToFile);

}

void EngineCore::render(Engine::Camera camera)
{

	m_renderSystem.render(camera);

}