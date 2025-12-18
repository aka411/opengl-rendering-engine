#include "../../include/engine/engine_core.h"


namespace Engine
{

	EngineCore::EngineCore() :
		m_ecsEngine(m_nullFatalErrorHandler),
		m_gpuBufferManager(),
		m_gpuTextureManager(),

		m_gpuMaterialSystem(m_gpuBufferManager),
		m_worldVertexBufferManagementSystem(m_gpuBufferManager),
		m_animationSystem(m_ecsEngine, m_gpuBufferManager),

		m_engineLoader(m_ecsEngine, m_gpuTextureManager,
		m_gpuMaterialSystem, m_worldVertexBufferManagementSystem, m_animationSystem),

		m_transformationSystem(m_ecsEngine),

		
		m_uiCoreSystem(m_ecsEngine, m_gpuBufferManager),

		m_renderSystem(m_ecsEngine, m_worldVertexBufferManagementSystem, m_gpuMaterialSystem, m_uiCoreSystem, m_animationSystem, m_gpuBufferManager),
		m_uiSystem(m_uiCoreSystem),
		m_uiBuilder(m_uiCoreSystem)
	


	{

		m_ecsEngine.registerComponent<EngineRenderComponent>();
		m_ecsEngine.registerComponent<EngineFatRenderComponent>();
		m_ecsEngine.registerComponent<EngineChildrenComponent>();
		m_ecsEngine.registerComponent<EngineTransformationComponent>();

		m_ecsEngine.registerComponent<AnimationComponent>();
		m_ecsEngine.registerComponent<BoneAnimationComponent>();
		m_ecsEngine.registerComponent<BoneJointMatrixIdComponent>();
		m_ecsEngine.registerComponent<AnimationStateComponent>();

		m_ecsEngine.registerComponent<RootEntityComponent>();

	}

	ECS::ECSEngine& EngineCore::getECSEngine()
	{

		return m_ecsEngine;
	}

	ECS::EntityId EngineCore::loadModel(std::string pathToFile)
	{

		ECS::EntityId rootEntityId = m_engineLoader.createRootEntity(pathToFile);


		return rootEntityId;
	}




	void EngineCore::update(const float deltaTime)
	{
		//need to look into how to order properly though
		m_transformationSystem.updateTransformationsForAllEntities();
		m_uiSystem.update();
		m_animationSystem.animateAll(deltaTime);

	}

	void EngineCore::render(Engine::Camera camera)
	{

		m_renderSystem.render(camera);

	}


	void EngineCore::renderUI()
	{
		m_renderSystem.renderUI();

	}

	/***********UI SYSTEM********************/

		/***** UI SYSTEM *****/
	UI::UIBuilder& EngineCore::getUIBuilder()
	{
		return m_uiBuilder;
	}



	UI::UICoreSystem& EngineCore::getUICoreSystem()
	{

		return m_uiCoreSystem;
	}

}