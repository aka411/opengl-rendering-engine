#include "../include/ui_vertex_buffer_system.h"



namespace UI
{

	void UIVertexBufferSystem::createNewVertexBufferForFormat(UIVertexFormat uiVertexFormat)
	{
		GPUBufferInfo gpuBufferInfo = m_gpuBuffermanager.createMappedVertexBuffer(50 * 1024 * 1024, nullptr);//50MiB

		GPUBufferSubBumpAllocator gpuBufferSubBumpAllocator(gpuBufferInfo);

		m_formatToVertexBufferSubAllocators.emplace(uiVertexFormat,
			std::move(gpuBufferSubBumpAllocator)
		);
	}

	UIVertexBufferSystem::UIVertexBufferSystem()
	{

	}

		
	size_t UIVertexBufferSystem::uploadVertexData(UIVertexFormat uiVertexFormat, std::byte* data, size_t size)
	{

		const auto& it = m_formatToVertexBufferSubAllocators.find(uiVertexFormat);

		if (it == m_formatToVertexBufferSubAllocators.end())
		{
			createNewVertexBufferForFormat(uiVertexFormat);
		}

		GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_formatToVertexBufferSubAllocators.at(uiVertexFormat);


		AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
		GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset
	}


	GPUBufferInfo UIVertexBufferSystem::getBufferInfoForVertexFormat(UIVertexFormat uiVertexFormat)
	{
		return m_formatToVertexBufferSubAllocators.at(uiVertexFormat).getGPUBufferInfo();
	}


}