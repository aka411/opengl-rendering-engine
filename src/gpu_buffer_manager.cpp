
#include <stdexcept>

#include "../include/gpu_buffer_manager.h"



GPUBufferManager::~GPUBufferManager()
{
	glDeleteBuffers(m_allocatedBuffers.size(), m_allocatedBuffers.data());
}

GPUBufferManager::GPUBufferManager()
{
	


}

BufferInfo GPUBufferManager::createMappedVertexBuffer(const size_t size, void* data)
{
	GLuint VBO;



	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glBufferStorage(GL_ARRAY_BUFFER, size, data, flags);

	void* mappedVBOData = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, flags);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_allocatedBuffers.push_back(VBO);

	BufferInfo bufferInfo;

	bufferInfo.bufferHandle = VBO;
	bufferInfo.totalSize = size;
	bufferInfo.mappedPtr = mappedVBOData;


	return bufferInfo;
}

BufferInfo GPUBufferManager::createMappedUBOBuffer(const size_t size, void* data)
{
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);



	
	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glBufferStorage(GL_UNIFORM_BUFFER, size, NULL, flags);
	void* mappedUBOData = glMapBufferRange(GL_UNIFORM_BUFFER, 0, size, flags);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_allocatedBuffers.push_back(UBO);

	BufferInfo bufferInfo;

	bufferInfo.bufferHandle = UBO;
	bufferInfo.totalSize = size;
	bufferInfo.mappedPtr = mappedUBOData;


	return bufferInfo;
}

BufferInfo GPUBufferManager::createMappedIndexBuffer(const size_t size, void* data)
{
	GLuint EBO;



	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, size, data, flags);

	void* mappedVBOData = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, flags);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_allocatedBuffers.push_back(EBO);

	BufferInfo bufferInfo;

	bufferInfo.bufferHandle = EBO;
	bufferInfo.totalSize = size;
	bufferInfo.mappedPtr = mappedVBOData;


	return bufferInfo;
}





