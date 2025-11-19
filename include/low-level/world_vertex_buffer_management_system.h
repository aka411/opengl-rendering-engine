#pragma once
#include <unordered_map>
#include "rendering_system_data_types.h"
#include "gpu_buffer_sub_bump_allocator.h"



class WorldVertexBufferSystem
{

private:
	std::unordered_map<VertexFormat,GPUBufferSubBumpAllocator> m_formatToVertexBufferSubAllocators;
	std::unordered_map<IndexType, GPUBufferSubBumpAllocator> m_formatToIndexBufferSubAllocators;
public:

	WorldVertexBufferSystem();
	//get buffer for given vertex format
	size_t uploadVertexData(VertexFormat vertexFormat, std::byte* data, size_t size);
	size_t uploadIndexData(IndexType indexType, std::byte* data, size_t size);
};