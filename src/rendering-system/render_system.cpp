#include "../../include/rendering-system/render_system.h"
#include <unordered_map>

RenderSystem::RenderSystem(TheEngine::ECS::ECSEngine& ecsEngine, WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, GPUMaterialSystem& gpuMaterialSystem, UI::UICoreSystem& uiCoreSystem)
	:
	m_vertexFormatManager(),
	m_ecsEngine(ecsEngine),
	m_worldVertexBufferManagementSystem(worldVertexBufferManagementSystem),
	m_uiRenderer(uiCoreSystem),
	m_worldRenderer(m_vertexFormatManager, worldVertexBufferManagementSystem, gpuMaterialSystem)
{






}














void RenderSystem::render(Engine::Camera& camera)
{
	

	std::unordered_map<VertexFormat, std::vector<RenderCommand>> vertexFormatToRenderCommand;

	TheEngine::ECS::Query querySingleRenderableComponent = m_ecsEngine.getQuery<EngineTransformationComponent, EngineRenderComponent>();

	TheEngine::ECS::Query queryFatRenderableComponent = m_ecsEngine.getQuery<EngineTransformationComponent, EngineFatRenderComponent>();


	

	for (auto& chunkArrayView : querySingleRenderableComponent.getChunkArrayViews())
	{
		const size_t count = chunkArrayView.getCount();

		for (size_t i = 0; i < count; ++i)
		{
			EngineTransformationComponent* engineTransformationComponentArray = chunkArrayView.getComponentArray<EngineTransformationComponent>();
			EngineRenderComponent* engineRenderComponentArray = chunkArrayView.getComponentArray<EngineRenderComponent>();

			EngineTransformationComponent engineTransformationComponent = engineTransformationComponentArray[i];
			EngineRenderComponent engineRenderComponent = engineRenderComponentArray[i];

			RenderCommand renderCommand;
			PerObjectData perObjectData;

			perObjectData.engineTransformationComponent = engineTransformationComponent;
			perObjectData.materialId = engineRenderComponent.materialId;

			renderCommand.perObjectData = perObjectData;

			renderCommand.indexCount = engineRenderComponent.indexCount;
			renderCommand.indexOffset = engineRenderComponent.indexOffset;
			renderCommand.indexType = engineRenderComponent.indexType;
			renderCommand.isIndexed = engineRenderComponent.isIndexed;
			renderCommand.vertexCount = engineRenderComponent.vertexCount;
			renderCommand.vertexOffset = engineRenderComponent.vertexOffset;


			vertexFormatToRenderCommand[engineRenderComponent.vertexFormat].push_back(renderCommand);
		}


	}



	for (auto& chunkArrayView : queryFatRenderableComponent.getChunkArrayViews())
	{
		const size_t count = chunkArrayView.getCount();

		EngineTransformationComponent* engineTransformationComponentArray = chunkArrayView.getComponentArray<EngineTransformationComponent>();
		EngineFatRenderComponent* engineFatRenderComponentArray = chunkArrayView.getComponentArray<EngineFatRenderComponent>();

		for (size_t i = 0; i < count; ++i)
		{


			const EngineTransformationComponent& engineTransformationComponent = engineTransformationComponentArray[i];
			const EngineFatRenderComponent& engineFatRenderComponent = engineFatRenderComponentArray[i];

			for (auto& engineRenderComponent : engineFatRenderComponent.engineRenderComponents)
			{
				RenderCommand renderCommand;
				PerObjectData perObjectData;

				perObjectData.engineTransformationComponent = engineTransformationComponent;
				perObjectData.materialId = engineRenderComponent.materialId;

				renderCommand.perObjectData = perObjectData;

				renderCommand.indexCount = engineRenderComponent.indexCount;
				renderCommand.indexOffset = engineRenderComponent.indexOffset;
				renderCommand.indexType = engineRenderComponent.indexType;
				renderCommand.isIndexed = engineRenderComponent.isIndexed;
				renderCommand.vertexCount = engineRenderComponent.vertexCount;
				renderCommand.vertexOffset = engineRenderComponent.vertexOffset;


				vertexFormatToRenderCommand[engineRenderComponent.vertexFormat].push_back(renderCommand);
			}


		}
	}




	


	m_worldRenderer.render(vertexFormatToRenderCommand,camera);
	
}

void RenderSystem::renderUI()
{



	m_uiRenderer.renderUI();




}
