#include "../include/ui_mesh_generation_system.h"
#include "../include/ui_component.h"
#include "../include/geometry_generator.h"


namespace UI
{
	UIMeshGenerationSystem::UIMeshGenerationSystem(TheEngine::ECS::ECSEngine* ecsEngine)
	{
		m_ecsEngine = ecsEngine;

	}

	void UIMeshGenerationSystem::generateUIMeshes()
	{
		TheEngine::ECS::Query uiMeshQuery = m_ecsEngine->getQuery<UI::UIRenderMeshComponent,UI::UIRectDimensionsComponent>();



		for (auto& chunkArrayView : uiMeshQuery.getChunkArrayViews())
		{
			const size_t count = chunkArrayView.getCount();

			for (size_t i = 0; i < count; ++i)
			{
				UI::UIRectDimensionsComponent* uiRectDimensionsComponentPtr = chunkArrayView.getComponentArray<UI::UIRectDimensionsComponent>();
				UI::UIRenderMeshComponent* uRenderMeshComponentPtr = chunkArrayView.getComponentArray<UI::UIRenderMeshComponent>();

				//use the geometry generator to create a quad mesh based on rect bounds

				if(uiRectDimensionsComponentPtr == nullptr || uRenderMeshComponentPtr == nullptr)
				{
					continue; 
				}

				const float width = uiRectDimensionsComponentPtr[i].width;
				const float height = uiRectDimensionsComponentPtr[i].height;

				//Generate Quad Mesh
				GeometryGenerator::MeshData quadMeshData = GeometryGenerator::getRectangle(width, height);

				uRenderMeshComponentPtr[i].data = std::move(quadMeshData.data);
				uRenderMeshComponentPtr[i].vertexCount = quadMeshData.numOfVertex;

				uRenderMeshComponentPtr[i].isDirty = true; //for signaling upload system

			}
		}


	}

}