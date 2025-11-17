#pragma once

#include <vector>
#include "../glad/glad.h"


class GPUBufferManager
{
private:


	std::vector<GLuint> m_allocatedBuffers;

public:
	~GPUBufferManager();


	GPUBufferManager();


	BufferInfo createMappedVertexBuffer(const size_t size, void* data);

	BufferInfo createMappedUBOBuffer(const size_t size, void* data);

	BufferInfo createMappedIndexBuffer(const size_t size, void* data);


};