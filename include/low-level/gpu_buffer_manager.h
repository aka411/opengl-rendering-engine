#pragma once

#include <vector>
#include "../../glad/glad.h"
#include "rendering_system_data_types.h"

class GPUBufferManager
{
private:


	std::vector<GLuint> m_allocatedBuffers;

public:
	~GPUBufferManager();


	GPUBufferManager();


	GPUBufferInfo createMappedVertexBuffer(const size_t size, void* data);

	GPUBufferInfo createMappedUBOBuffer(const size_t size, void* data);

	GPUBufferInfo createMappedIndexBuffer(const size_t size, void* data);

	GPUBufferInfo createMappedSSBO(const size_t size, void* data);



};