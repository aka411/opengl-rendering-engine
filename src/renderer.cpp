
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

	glUseProgram(m_defaultShaderProgram);

	glBindVertexArray(m_vertexFormatManager.getDefaultVAO());
	
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferManagementSystem.getGlobalVertexBufferInfo().bufferHandle);
	//glBindBufferBase(GL_ARRAY_BUFFER, m_bufferManagementSystem.getGlobalVertexBufferInfo().bufferHandle, 0); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferManagementSystem.getGlobalVertexBufferInfo().bufferHandle);


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

			glVertexAttribPointer(
				0,       // Attribute index (0, 1, 2, etc.)
				3,         // Component count (3 for vec3, 2 for vec2)
				GL_FLOAT,          // Data type (GL_FLOAT, GL_UNSIGNED_SHORT, etc.)
				GL_FALSE,                   // Normalized (GL_FALSE)
				primitive.postionStride,                     // Dynamic Stride (0 or >0 for interleaved)
				(const void*)primitive.positionOffsetInBuffer    // Dynamic Offset in bytes from VBO start
			);


			glVertexAttribPointer(
				1,       // Attribute index (0, 1, 2, etc.)
				3,         // Component count (3 for vec3, 2 for vec2)
				GL_FLOAT,          // Data type (GL_FLOAT, GL_UNSIGNED_SHORT, etc.)
				GL_FALSE,                   // Normalized (GL_FALSE)
				primitive.normalStride,                     // Dynamic Stride (0 or >0 for interleaved)
				(const void*)primitive.normalOffsetInBuffer    // Dynamic Offset in bytes from VBO start
			);


			glVertexAttribPointer(
				2,       // Attribute index (0, 1, 2, etc.)
				2,         // Component count (3 for vec3, 2 for vec2)
				GL_FLOAT,          // Data type (GL_FLOAT, GL_UNSIGNED_SHORT, etc.)
				GL_FALSE,                   // Normalized (GL_FALSE)
				primitive.texCoordStride,                     // Dynamic Stride (0 or >0 for interleaved)
				(const void*)primitive.texCoordOffsetInBuffer    // Dynamic Offset in bytes from VBO start
			);
			

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
			GLuint bindingPoint = 5; // The desired binding point
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





			if (primitive.isIndexed)
			{
				

				auto indexFormat = (primitive.indexFormat == Engine::IndexFormat::UNSIGNED_INT) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
				glDrawElements(GL_TRIANGLES, primitive.indexCount, indexFormat, (const void*)primitive.indexOffsetInBuffer);
				

			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, primitive.vertexCount);
			}
		}

		m_hierarchyTraverser.traverse();

	}
	
	//std::cout << "Frame Complete" << std::endl;

}
