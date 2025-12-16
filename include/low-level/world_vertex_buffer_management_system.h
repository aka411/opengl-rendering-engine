#pragma once
#include <unordered_map>
#include "rendering_system_data_types.h"
#include "gpu_buffer_sub_bump_allocator.h"
#include "gpu_buffer_manager.h"


class WorldVertexBufferManagementSystem
{


private:

	GPUBufferManager m_gpuBuffermanager;

	std::unordered_map<VertexFormat, GPUBufferSubBumpAllocator> m_formatToVertexBufferSubAllocators;
	std::unordered_map<IndexType, GPUBufferSubBumpAllocator> m_formatToIndexBufferSubAllocators;

	void createNewVertexBufferForFormat(const VertexFormat vertexFormat);
	void createNewIndexBufferForIndexType(const IndexType indexType);

public:

	WorldVertexBufferManagementSystem(GPUBufferManager& gpuBufferManager);

	//get buffer for given vertex format
	size_t uploadVertexData(const VertexFormat vertexFormat, std::byte* data, const size_t size);
	size_t uploadIndexData(const IndexType indexType, std::byte* data, const size_t size);

	GPUBufferInfo getBufferInfoForVertexFormat(const VertexFormat vertexFormat);
	GPUBufferInfo getBufferInfoForIndexType(const IndexType indextype);


};