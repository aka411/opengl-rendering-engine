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
		TheEngine::ECS::Query uiMeshQuery = m_ecsEngine.getQuery<UIRenderMeshComponent,UIRectDimensionsComponent,UIMaterialComponent>();



		for (auto& chunkArrayView : uiMeshQuery.getChunkArrayViews())
		{
			const size_t count = chunkArrayView.getCount();

			for (size_t i = 0; i < count; ++i)
			{
				UIRectDimensionsComponent* uiRectDimensionsComponentPtr = chunkArrayView.getComponentArray<UIRectDimensionsComponent>();
				UIRenderMeshComponent* uRenderMeshComponentPtr = chunkArrayView.getComponentArray<UIRenderMeshComponent>();

				//use the geometry generator to create a quad mesh based on rect bounds

				if(uiRectDimensionsComponentPtr == nullptr || uRenderMeshComponentPtr == nullptr || !uiRectDimensionsComponentPtr->isDirty)
				{
					continue; 
				}

				const float width = uiRectDimensionsComponentPtr[i].width;
				const float height = uiRectDimensionsComponentPtr[i].height;


				//Generate Quad Mesh
				GeometryGenerator::MeshData quadMeshData = GeometryGenerator::getRectangle(width, height);
				const auto bufferType = uRenderMeshComponentPtr->bufferType;
				const auto vertexFormat = uRenderMeshComponentPtr->vertexFormat;

				size_t offset = m_uiCoreSystem.getUIVertexBufferManagementSystem().uploadVertexData(vertexFormat, bufferType,quadMeshData.data.data(), quadMeshData.data.size());
			
				uRenderMeshComponentPtr->vertexBufferOffset = offset;
				uRenderMeshComponentPtr->vertexCount =0/*Not in Bytes*/;
				uRenderMeshComponentPtr->isDirty = false;

			}
		}






	}










}