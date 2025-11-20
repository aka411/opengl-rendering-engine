
#include <stdexcept>
#include "../../include/low-level/gpu_buffer_manager.h"





GPUBufferManager::~GPUBufferManager()
{
	glDeleteBuffers(m_allocatedBuffers.size(), m_allocatedBuffers.data());
}

GPUBufferManager::GPUBufferManager()
{
	


}

GPUBufferInfo GPUBufferManager::createMappedVertexBuffer(const size_t size, void* data)
{
	GLuint VBO;



	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glBufferStorage(GL_ARRAY_BUFFER, size, data, flags);

	void* mappedVBOData = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, flags);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_allocatedBuffers.push_back(VBO);

	GPUBufferInfo gpuBufferInfo;

	gpuBufferInfo.bufferHandle = VBO;
	gpuBufferInfo.size = size;
	gpuBufferInfo.mappedPtr = mappedVBOData;


	return gpuBufferInfo;
}

GPUBufferInfo GPUBufferManager::createMappedUBOBuffer(const size_t size, void* data)
{
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);



	
	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glBufferStorage(GL_UNIFORM_BUFFER, size, NULL, flags);
	void* mappedUBOData = glMapBufferRange(GL_UNIFORM_BUFFER, 0, size, flags);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_allocatedBuffers.push_back(UBO);

	GPUBufferInfo gpuBufferInfo;

	gpuBufferInfo.bufferHandle = UBO;
	gpuBufferInfo.size = size;
	gpuBufferInfo.mappedPtr = mappedUBOData;


	return gpuBufferInfo;
}

GPUBufferInfo GPUBufferManager::createMappedIndexBuffer(const size_t size, void* data)
{
	GLuint EBO;



	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, size, data, flags);

	void* mappedVBOData = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, size, flags);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_allocatedBuffers.push_back(EBO);

	GPUBufferInfo gpuBufferInfo;

	gpuBufferInfo.bufferHandle = EBO;
	gpuBufferInfo.size = size;
	gpuBufferInfo.mappedPtr = mappedVBOData;


	return gpuBufferInfo;
}



GPUBufferInfo GPUBufferManager::createMappedSSBO(const size_t size, void* data)
{
	GLuint SSBO;



	glGenBuffers(1, &SSBO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glBufferStorage(GL_SHADER_STORAGE_BUFFER, size, data, flags);

	void* mappedVBOData = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, size, flags);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	m_allocatedBuffers.push_back(SSBO);

	GPUBufferInfo gpuBufferInfo;

	gpuBufferInfo.bufferHandle = SSBO;
	gpuBufferInfo.size = size;
	gpuBufferInfo.mappedPtr = mappedVBOData;


	return gpuBufferInfo;
}

