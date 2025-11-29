#include "../include/vertex_buffer_upload_manager.h"
#include "../include/ui_component.h"





VertexBufferUploadManager::VertexBufferUploadManager(TheEngine::ECS::ECSEngine& ecsEngine, UI::UIVertexBufferSystem& uiVertexBufferSystem) : m_ecsEngine(ecsEngine), m_uiVertexBufferSystem(uiVertexBufferSystem)
{

	//get all renderable data ierteate and upload



}


void VertexBufferUploadManager::uploadAllVertexBuffers()
{


	TheEngine::ECS::Query uiRenderMeshComponentQuery = m_ecsEngine.getQuery<UI::UIRenderMeshComponent>();

	

	//glFinish();

	for (auto& chunkArrayView : uiRenderMeshComponentQuery.getChunkArrayViews())
	{
		const size_t count = chunkArrayView.getCount();

		for (size_t i = 0; i < count; ++i)
		{
			UI::UIRenderMeshComponent* uiRenderMeshComponentPtr = chunkArrayView.getComponentArray<UI::UIRenderMeshComponent>();

			if( uiRenderMeshComponentPtr == nullptr || !uiRenderMeshComponentPtr[i].isDirty )
			{
				continue; //skip if not dirty
			}

			const size_t offset = m_uiVertexBufferSystem.uploadVertexData(
				uiRenderMeshComponentPtr[i].vertexFormat,
				uiRenderMeshComponentPtr[i].data.data(),
				uiRenderMeshComponentPtr[i].data.size()
			);

			uiRenderMeshComponentPtr[i].vertexBufferOffset = offset;
			uiRenderMeshComponentPtr[i].isDirty = false;
			//reset vector
			uiRenderMeshComponentPtr[i].data.resize(0);

		}
	}


}
