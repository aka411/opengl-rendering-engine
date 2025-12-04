#include "../../include/rendering-system/world_renderer.h"
#include "../../include/low-level/vertex_format_helper.h"
#include "../../glad/glad.h"
#include <unordered_map>

WorldRenderer::WorldRenderer(VertexFormatManager& vertexFormatManager, WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, GPUMaterialSystem& gpuMaterialSystem)
	: m_worldVertexBufferManagementSystem(worldVertexBufferManagementSystem),
	  m_vertexFormatManager(vertexFormatManager),
	  m_shaderManager(),
	  m_gpuMaterialSystem(gpuMaterialSystem)
{



	m_cameraBufferInfo = m_gpuBufferManager.createMappedUBOBuffer(1024 *10,nullptr);//10 KiB
	m_objectBufferInfo = m_gpuBufferManager.createMappedUBOBuffer(1024 * 1024*3,nullptr);//10 KiB


}


GLenum IndexTypeToGLType(IndexType type)
{
	switch (type)
	{
	case IndexType::UBYTE_8:
		return GL_UNSIGNED_BYTE;

	case IndexType::USHORT_16:
		return GL_UNSIGNED_SHORT;

	case IndexType::UINT_32:
		return GL_UNSIGNED_INT;

	case IndexType::UNKNOWN:
	default:
		// non-standard GLenum value to indicate failure.
		return 0;
	}
}

void WorldRenderer::render(std::unordered_map<VertexFormat, std::vector<RenderCommand>>& vertexFormatToRenderCommands, Engine::Camera& camera)
{
	
	//set up 
	glClearColor(0.2, 0.4, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	//glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);


	//material ssbo
	GPUBufferInfo materialBufferIndo = m_gpuMaterialSystem.getGPUBufferInfo(MaterialType::PBR_METALLIC_ROUGHNESS);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0, materialBufferIndo.bufferHandle);
	
	//camera ubo 
	struct CameraData
	{
		glm::mat4  projection;
		glm::mat4  view;
	};

	struct PerObjectDataUBOData
	{
		glm::mat4  model;
		
		uint32_t materialId; // no 64 bit int in shader opengl unless extension is used
		float padding[3];
	};


	CameraData cameraData;
	cameraData.projection = camera.getProjectionMatrix();
	cameraData.view = camera.getViewMatrix();


	
	memcpy(m_cameraBufferInfo.mappedPtr, &cameraData, sizeof(CameraData));
	
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_cameraBufferInfo.bufferHandle, 0, sizeof(CameraData));



	for (const auto& vertexFormatToRenderCommand : vertexFormatToRenderCommands)
	{

		
		//set up vao
		VertexFormat vertexFormat = vertexFormatToRenderCommand.first;
		GLuint vao = m_vertexFormatManager.getVAO(vertexFormat);
		glBindVertexArray(vao);

		//set up shader 
		GLuint shaderProgram = m_shaderManager.getShaderProgramForVertexFormat(vertexFormat);
		glUseProgram(shaderProgram);
		

		//set up vertex buffer

		GPUBufferInfo vertexBufferInfo = m_worldVertexBufferManagementSystem.getBufferInfoForVertexFormat(vertexFormat);
		glBindVertexBuffer(0, vertexBufferInfo.bufferHandle,0, VertexFormatHelper::getSizeOfVertexForFormatInBytes(vertexFormat));
		
		size_t objectSize = sizeof(PerObjectDataUBOData); // 80 bytes
		size_t currentOffset = 0; // Initialize offset

		for (const auto& renderCommand : vertexFormatToRenderCommand.second)
		{



			PerObjectDataUBOData perObjectDataUBOData;
			perObjectDataUBOData.model = renderCommand.perObjectData.engineTransformationComponent.worldTransformMatrix;
			perObjectDataUBOData.materialId = renderCommand.perObjectData.materialId;
		
			void* objectWritePtr = (char*)m_objectBufferInfo.mappedPtr + currentOffset;

			memcpy(objectWritePtr, &perObjectDataUBOData, objectSize);

			
			glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_objectBufferInfo.bufferHandle, currentOffset, sizeof(PerObjectDataUBOData));
			currentOffset += objectSize;

			

			//  Upload the data per object 
			

			if (renderCommand.isIndexed)
			{


				GLenum glIndexFormat = IndexTypeToGLType(renderCommand.indexType);
				GPUBufferInfo indexBufferInfo = m_worldVertexBufferManagementSystem.getBufferInfoForIndexType(renderCommand.indexType);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferInfo.bufferHandle);
				assert(renderCommand.indexType != IndexType::UNKNOWN);
				assert(renderCommand.vertexOffset% VertexFormatHelper::getSizeOfVertexForFormatInBytes(vertexFormat) == 0);
				assert(renderCommand.indexOffset % (renderCommand.indexType == IndexType::USHORT_16 ? 2 : (renderCommand.indexType == IndexType::UINT_32 ? 4 : 1)) == 0);
				const size_t vertexOffset = ((renderCommand.vertexOffset) / VertexFormatHelper::getSizeOfVertexForFormatInBytes(vertexFormat));
				glDrawElementsBaseVertex(GL_TRIANGLES, renderCommand.indexCount, glIndexFormat, (const void*)renderCommand.indexOffset,vertexOffset);
				
			}
			else
			{
				glDrawArrays(GL_TRIANGLES,renderCommand.vertexOffset / VertexFormatHelper::getSizeOfVertexForFormatInBytes(vertexFormat), renderCommand.vertexCount);
				
			}




		}
		
	}














}