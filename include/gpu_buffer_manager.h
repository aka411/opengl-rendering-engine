#pragma once

#include <vector>
#include "../glad/glad.h"

struct BufferInfo
{
	GLuint bufferHandle;
	size_t totalSize = 0;

	void* mappedPtr = nullptr;

};
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