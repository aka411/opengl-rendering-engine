#include "../../include/low-level/world_vertex_buffer_management_system.h"



WorldVertexBufferSystem::WorldVertexBufferSystem()
{


}



size_t WorldVertexBufferSystem::uploadVertexData(VertexFormat vertexFormat, std::byte* data, size_t size)
{

	GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_formatToVertexBufferSubAllocators.at(vertexFormat);


	AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
	GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

	std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

	std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
	memcpy(absoluteBufferPtr, data, size);

	return allocationInfo.offset; //byte offset we might need a higher level offset

}


size_t WorldVertexBufferSystem::uploadIndexData(IndexType indexType, std::byte* data, size_t size)
{

	GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_formatToIndexBufferSubAllocators.at(indexType);


	AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
	GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

	std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

	std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
	memcpy(absoluteBufferPtr, data, size);

	return allocationInfo.offset; //byte offset we might need a higher level offset

}
