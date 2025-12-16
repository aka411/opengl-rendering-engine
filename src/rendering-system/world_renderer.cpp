#include "../../include/rendering-system/world_renderer.h"
#include "../../include/low-level/vertex_format_helper.h"
#include "../../glad/glad.h"
#include <unordered_map>
#include <iostream>




namespace Engine
{

	WorldRenderer::WorldRenderer(VertexFormatManager& vertexFormatManager, WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, GPUMaterialSystem& gpuMaterialSystem, AnimationSystem& animationSystem, RenderCommandBufferManager& renderCommandBufferManager,
		ObjectDataBufferManager& objectDataBufferManager):

		m_worldVertexBufferManagementSystem(worldVertexBufferManagementSystem),
		m_vertexFormatManager(vertexFormatManager),
		m_shaderManager(),
		m_gpuMaterialSystem(gpuMaterialSystem),
		m_animationSystem(animationSystem),
		m_renderCommandBufferManager(renderCommandBufferManager),
		m_objectDataBufferManager(objectDataBufferManager)
	{



		m_cameraBufferInfo = m_gpuBufferManager.createMappedUBOBuffer(1024 * 10, nullptr);//10 KiB
	


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



	void WorldRenderer::startFrame(Engine::Camera& camera)
	{


		//glFlush();

		glClearColor(0.2, 0.4, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		glDisable(GL_BLEND);
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CCW);
		//glCullFace(GL_BACK);

				//material ssbo





		//camera ubo 
		struct CameraData
		{
			glm::mat4  projection;
			glm::mat4  view;
		};




		CameraData cameraData;
		cameraData.projection = camera.getProjectionMatrix();
		cameraData.view = camera.getViewMatrix();



		memcpy(m_cameraBufferInfo.mappedPtr, &cameraData, sizeof(CameraData));

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_cameraBufferInfo.bufferHandle, 0, sizeof(CameraData));

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_objectDataBufferManager.getGPUBufferForThisFrame().bufferHandle);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_animationSystem.getSkeletalAnimationManager().getJointMatrixSSBO().bufferHandle);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_gpuMaterialSystem.getGPUBufferInfo(MaterialType::PBR_METALLIC_ROUGHNESS).bufferHandle);
		
	}


	void WorldRenderer::IndirectDrawArray(const VertexFormat vertexFormat, const size_t byteOffset, const size_t count)
	{


		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_renderCommandBufferManager.getArrayGPUBufferForThisFrame().bufferHandle);


			//set up vao
			
			GLuint vao = m_vertexFormatManager.getVAO(vertexFormat);

			glBindVertexArray(vao);

			//set up shader 
			GLuint shaderProgram = m_shaderManager.getShaderProgramForVertexFormat(vertexFormat);
			glUseProgram(shaderProgram);


			//set up vertex buffer

			GPUBufferInfo vertexBufferInfo = m_worldVertexBufferManagementSystem.getBufferInfoForVertexFormat(vertexFormat);
			glBindVertexBuffer(0, vertexBufferInfo.bufferHandle, 0, VertexFormatHelper::getSizeOfVertexForFormatInBytes(vertexFormat));


			glMultiDrawArraysIndirect(GL_TRIANGLES, (void*)byteOffset, count, 0);

	}



	void WorldRenderer::IndirectDrawIndexed(const VertexFormat vertexFormat, const IndexType indexType, const size_t byteOffset, const size_t count)
	{

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_renderCommandBufferManager.getIndexedGPUBufferForThisFrame().bufferHandle);

		//set up vao
	
		GLuint vao = m_vertexFormatManager.getVAO(vertexFormat);

		glBindVertexArray(vao);

		//set up shader 
		GLuint shaderProgram = m_shaderManager.getShaderProgramForVertexFormat(vertexFormat);
		glUseProgram(shaderProgram);


		//set up vertex buffer

		GPUBufferInfo vertexBufferInfo = m_worldVertexBufferManagementSystem.getBufferInfoForVertexFormat(vertexFormat);
		glBindVertexBuffer(0, vertexBufferInfo.bufferHandle, 0, VertexFormatHelper::getSizeOfVertexForFormatInBytes(vertexFormat));

		GPUBufferInfo indexBufferInfo = m_worldVertexBufferManagementSystem.getBufferInfoForIndexType(indexType);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferInfo.bufferHandle);

		glMultiDrawElementsIndirect(GL_TRIANGLES, IndexTypeToGLType(indexType), (void*)byteOffset, count, 0);

	}



	void WorldRenderer::endFrame()
	{

		m_renderCommandBufferManager.nextFrame();
		m_objectDataBufferManager.nextFrame();

	}








}



