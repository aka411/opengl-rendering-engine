#include "../../../../include/ui/include/systems/ui_mesh_system.h"
#include "../../../../include/ui/include/utils/geometry_generator.h"
#include "../../../../include/ui/include/core/ui_component.h"



namespace UI
{
	UIMeshSystem::UIMeshSystem(UICoreSystem& uiCoreSystem)
		: m_uiCoreSystem(uiCoreSystem), m_ecsEngine(uiCoreSystem.getECSEngine())
	{


	}



   //TODO : Need to change this
	void UIMeshSystem::generateUIMeshesForDirty()
	{
		ECS::Query uiMeshQuery = m_ecsEngine.getQuery<UIRenderMeshComponent,UIRectDimensionsComponent,UIMaterialComponent>();



		for (auto& chunkArrayView : uiMeshQuery.getChunkArrayViews())
		{
			const size_t count = chunkArrayView.getCount();


			UIRectDimensionsComponent* uiRectDimensionsComponentPtr = chunkArrayView.getComponentArray<UIRectDimensionsComponent>();
			UIRenderMeshComponent* uRenderMeshComponentPtr = chunkArrayView.getComponentArray<UIRenderMeshComponent>();
			UIMaterialComponent* uiMaterialComponentPtr = chunkArrayView.getComponentArray<UIMaterialComponent>();

			//use the geometry generator to create a quad mesh based on rect bounds

			if (uiRectDimensionsComponentPtr == nullptr || uRenderMeshComponentPtr == nullptr || !uiRectDimensionsComponentPtr->isDirty)
			{
				continue;
			}

			for (size_t i = 0; i < count; ++i)
			{
			

				const float width = uiRectDimensionsComponentPtr[i].width;
				const float height = uiRectDimensionsComponentPtr[i].height;



				GeometryGenerator::MeshData quadMeshData = GeometryGenerator::getColouredRectangle(width, height, uiMaterialComponentPtr[i].color);

				const auto bufferType = uRenderMeshComponentPtr[i].bufferType;
				const auto vertexFormat = uRenderMeshComponentPtr[i].vertexFormat;

				size_t offset = m_uiCoreSystem.getUIVertexBufferManagementSystem().uploadVertexData(vertexFormat, bufferType,quadMeshData.data.data(), quadMeshData.data.size());
			
				uRenderMeshComponentPtr[i].vertexBufferOffset = offset;
				uRenderMeshComponentPtr[i].vertexCount = quadMeshData.numOfVertex/*Not in Bytes*/;
				uRenderMeshComponentPtr[i].isDirty = false;

			}
		}






	}










}