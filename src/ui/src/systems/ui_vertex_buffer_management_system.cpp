#include "../../../../include/ui/include/systems/ui_vertex_buffer_management_system.h"
#include "../../../../include/low-level/world_vertex_buffer_management_system.h"
#include <cstring>



namespace UI
{



	GPUBufferSubBumpAllocator UIVertexBufferManagementSystem::createNewSSBOAllocator()
	{
		GPUBufferInfo gpuBufferInfo = m_gpuBufferManager.createMappedSSBO(50 * 1024 * 1024, nullptr);//50MiB

		return GPUBufferSubBumpAllocator (gpuBufferInfo);
		
	}


	UIVertexBufferManagementSystem::UIVertexBufferManagementSystem(GPUBufferManager& gpuBufferManager)
		: m_gpuBufferManager(gpuBufferManager)
	{



	}


	size_t UIVertexBufferManagementSystem::uploadVertexData(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType, std::byte* data, const size_t size)
	{

		//TODO : Use SSBO Only

		auto& bufferTypeMap = m_formatToNormalVertexBufferSubAllocators[uiVertexFormat];

		// 2. Check if the specific BufferType allocator exists within the inner map.
		if (bufferTypeMap.find(uiBufferType) == bufferTypeMap.end())
		{


			bufferTypeMap.emplace(uiBufferType, createNewSSBOAllocator());
			
		}





		GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = bufferTypeMap.at(uiBufferType);


		AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
		GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset
	}



	size_t UIVertexBufferManagementSystem::uploadTextVertexData(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType, std::byte* data, const size_t size)
	{



		//TODO : Use SSBO Only

		auto& bufferTypeMap = m_formatToTextVertexBufferSubAllocators[uiVertexFormat];

		// 2. Check if the specific BufferType allocator exists within the inner map.
		if (bufferTypeMap.find(uiBufferType) == bufferTypeMap.end())
		{


			bufferTypeMap.emplace(uiBufferType, createNewSSBOAllocator());

		}





		GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = bufferTypeMap.at(uiBufferType);


		AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
		GPUBufferInfo gpuBufferInfo = gpuBufferSubBumpAllocator.getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset






	}



	GPUBufferInfo UIVertexBufferManagementSystem::getBufferInfoForVertexFormat(const UIVertexFormat uiVertexFormat, UIBufferType uiBufferType)
	{
		return m_formatToNormalVertexBufferSubAllocators.at(uiVertexFormat).at(uiBufferType).getGPUBufferInfo();
	
	}



	GPUBufferInfo UIVertexBufferManagementSystem::getBufferInfoForTextWithVertexFormat(const UIVertexFormat uiVertexFormat, const UIBufferType uiBufferType)
	{

		return m_formatToTextVertexBufferSubAllocators.at(uiVertexFormat).at(uiBufferType).getGPUBufferInfo();

	}


}