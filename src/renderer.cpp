
#include "../include/renderer.h"
#include <iostream>
#include <glm/glm.hpp>
#include "../include/shader_code.h"

Renderer::Renderer(BufferManagementSystem& bufferManagementSystem, GPUTextureManager& gpuTextureManager, Engine::Camera& camera) : m_bufferManagementSystem(bufferManagementSystem), m_gpuTextureManager(gpuTextureManager), m_camera(camera)
{



	m_defaultShaderProgram = m_shaderManager.compileShader(defaultVertexShaderCode, defaultFragmentShaderCode);

	m_bonedShaderProgram = m_shaderManager.compileShader(bonedVertexShaderCode, defaultFragmentShaderCode);



}


void Renderer::setVirtualViewport(int width, int height)
{


}

void Renderer::setPhysicalViewport(int width, int height)
{
	m_viewPort.height = width;
	m_viewPort.width = height;


	m_camera.setPerspectiveProjection(60.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);

}



void Renderer::renderFrame(Engine::Model& engineModel)
{
	m_hierarchyTraverser.setModel(engineModel);

	glViewport(0, 0, m_viewPort.width, m_viewPort.height);



	//glBindVertexArray(m_vertexFormatManager.getDefaultVAO());
	
	//glBindBuffer(GL_ARRAY_BUFFER, m_bufferManagementSystem.getGlobalVertexBufferInfo().bufferHandle);
	//glBindBufferBase(GL_ARRAY_BUFFER, m_bufferManagementSystem.getGlobalVertexBufferInfo().bufferHandle, 0); 

	//glBindBufferBase(GL_ARRAY_BUFFER, 0, m_bufferManagementSystem.getGlobalVertexBufferInfo().bufferHandle);

	


	glClearColor(0.2, 0.4, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_CULL_FACE);

	CameraShaderData cameraShaderData;

	cameraShaderData.projection = m_camera.getProjectionMatrix();
	cameraShaderData.view = m_camera.getViewMatrix();

	BufferInfo cameraUBOInfo = m_bufferManagementSystem.getCameraUBOInfo();
	assert(cameraUBOInfo.mappedPtr != nullptr);
	memcpy(cameraUBOInfo.mappedPtr, &cameraShaderData, sizeof(cameraShaderData));

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, cameraUBOInfo.bufferHandle, 0, sizeof(cameraShaderData));
	

	while (!m_hierarchyTraverser.isTraversalComplete())
	{
		const int meshIndex = m_hierarchyTraverser.getCurrentNode()->meshIndex;
		if(meshIndex < 0)
		{
			m_hierarchyTraverser.traverse();
			continue;
		}
		for (auto& primitive : engineModel.meshes[meshIndex].primitives)
		{

			

			glProgramUniformMatrix4fv
			(
				m_defaultShaderProgram,
				5, // location
				1, // count
				GL_FALSE,
				 // Model matrix
				&m_hierarchyTraverser.getCurrentNode()->transformation.worldTransformMatrix[0][0]
			);


			//material related uniforms to be set here


			BufferInfo materialUBOInfo = m_bufferManagementSystem.getMaterialUBOInfo();
			assert(materialUBOInfo.mappedPtr != nullptr);
			assert(primitive.materialIndex < engineModel.materials.size());
			const Engine::Material& material = engineModel.materials[primitive.materialIndex];


			memcpy(materialUBOInfo.mappedPtr, &material.materialPBRMetallicRoughnessConstValues, sizeof(material.materialPBRMetallicRoughnessConstValues));
			GLuint bindingPoint = 5; 
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, materialUBOInfo.bufferHandle);

			Engine::MaterialPBRMetallicRoughnessConstValues materialPBRMetallicRoughnessConstValues = material.materialPBRMetallicRoughnessConstValues;
			
			if (materialPBRMetallicRoughnessConstValues.baseColorTexturePresent)
			{
				//
				glBindSampler(6, m_gpuTextureManager.getSampler());
				glBindTextureUnit(6, material.materialPBRMetallicRoughnessTexture.baseColourTexture);
			}
			if (materialPBRMetallicRoughnessConstValues.metallicRoughnessTexturePresent)
			{
				//
				glBindSampler(7, m_gpuTextureManager.getSampler());
				glBindTextureUnit(7, material.materialPBRMetallicRoughnessTexture.metallicRoughnessTexture);
			}

			if (materialPBRMetallicRoughnessConstValues.normalTexturePresent)
			{
				//
				glBindSampler(8, m_gpuTextureManager.getSampler());
				glBindTextureUnit(8, material.materialPBRMetallicRoughnessTexture.normalTexture);
			}
			if (materialPBRMetallicRoughnessConstValues.occulsionTexturePresent)
			{
				//
				glBindSampler(9, m_gpuTextureManager.getSampler());
				glBindTextureUnit(9, material.materialPBRMetallicRoughnessTexture.occulsionTexture);
			}
			if (materialPBRMetallicRoughnessConstValues.emmissiveTexturePresent)
			{
				//
				glBindSampler(10, m_gpuTextureManager.getSampler());
				glBindTextureUnit(10, material.materialPBRMetallicRoughnessTexture.emmissiveTexture);
			}

			if (primitive.isSkinned)
			{
				glUseProgram(m_bonedShaderProgram);
				glBindVertexArray(m_vertexFormatManager.getBonedVAO());
				//continue;

				BufferInfo boneUBOInfo = m_bufferManagementSystem.getBoneUBOBufferInfo();
				assert(boneUBOInfo.mappedPtr != nullptr);

				memcpy(boneUBOInfo.mappedPtr, engineModel.jointMatrices.data(), sizeof(engineModel.jointMatrices[0]) * engineModel.jointMatrices.size());
				GLuint boneUBOBindingPoint = 1; 
				glBindBufferBase(GL_UNIFORM_BUFFER, boneUBOBindingPoint, boneUBOInfo.bufferHandle);

			}
			else
			{
				glUseProgram(m_defaultShaderProgram);
				glBindVertexArray(m_vertexFormatManager.getDefaultVAO());
			}

			if (primitive.isIndexed)
			{
				

				auto indexFormat = (primitive.indexFormat == Engine::IndexFormat::UINT8) ?  GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferManagementSystem.getGlobalIndexBufferInfo().bufferHandle);
				glBindVertexBuffer(0, m_bufferManagementSystem.getGlobalVertexBufferInfo().bufferHandle, primitive.vertexOffsetInBuffer, primitive.stride);
				//glDrawElementsBaseVertex(GL_TRIANGLES, primitive.indexCount, indexFormat, (const void*)primitive.indexOffsetInBuffer, 0);
				glDrawElements(GL_TRIANGLES, primitive.indexCount, indexFormat, (const void*)primitive.indexOffsetInBuffer);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, primitive.vertexOffsetInBuffer, primitive.vertexCount);
			}
		}

		m_hierarchyTraverser.traverse();

	}
	
	//std::cout << "Frame Complete" << std::endl;

}
