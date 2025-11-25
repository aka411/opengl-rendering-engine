#include "../../include/low-level/world_vertex_buffer_management_system.h"




WorldVertexBufferManagementSystem::WorldVertexBufferManagementSystem(GPUBufferManager& gpuBufferManager): m_gpuBuffermanager(gpuBufferManager)
{

	
}

void WorldVertexBufferManagementSystem::createNewVertexBufferForFormat(VertexFormat vertexFormat)
{

	GPUBufferInfo gpuBufferInfo = m_gpuBuffermanager.createMappedVertexBuffer(50 * 1024 * 1024,nullptr);//50MiB

	GPUBufferSubBumpAllocator gpuBufferSubBumpAllocator(gpuBufferInfo);

	m_formatToVertexBufferSubAllocators.emplace(vertexFormat,
		std::move(gpuBufferSubBumpAllocator)
	);


}


void WorldVertexBufferManagementSystem::createNewIndexBufferForIndexType(IndexType indexType)
{

	GPUBufferInfo gpuBufferInfo = m_gpuBuffermanager.createMappedIndexBuffer(50 * 1024 * 1024, nullptr);//50MiB

	GPUBufferSubBumpAllocator gpuBufferSubBumpAllocator(gpuBufferInfo);

	m_formatToIndexBufferSubAllocators.emplace(indexType,
		std::move(gpuBufferSubBumpAllocator)
	);
	


}

size_t WorldVertexBufferManagementSystem::uploadVertexData(VertexFormat vertexFormat, std::byte* data, size_t size)
{


	const auto& it = m_formatToVertexBufferSubAllocators.find(vertexFormat);

	if (it == m_formatToVertexBufferSubAllocators.end())
	{
		createNewVertexBufferForFormat(vertexFormat);
	}

	GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_formatToVertexBufferSubAllocators.at(vertexFormat);


	AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
	GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

	std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

	std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
	memcpy(absoluteBufferPtr, data, size);

	return allocationInfo.offset; //byte offset we might need a higher level offset

}


size_t WorldVertexBufferManagementSystem::uploadIndexData(IndexType indexType, std::byte* data, size_t size)
{

	const auto& it = m_formatToIndexBufferSubAllocators.find(indexType);

	if (it == m_formatToIndexBufferSubAllocators.end())
	{
	createNewIndexBufferForIndexType(indexType);
	}


	GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_formatToIndexBufferSubAllocators.at(indexType);

	AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
	GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

	std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

	std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
	memcpy(absoluteBufferPtr, data, size);

	return allocationInfo.offset; //byte offset we might need a higher level offset

}



GPUBufferInfo WorldVertexBufferManagementSystem::getBufferInfoForVertexFormat(VertexFormat vertexFormat)
{

	return m_formatToVertexBufferSubAllocators.at(vertexFormat).getGPUBufferInfo();

}

GPUBufferInfo WorldVertexBufferManagementSystem::getBufferInfoForIndexType(IndexType indextype)
{

	return m_formatToIndexBufferSubAllocators.at(indextype).getGPUBufferInfo();

}