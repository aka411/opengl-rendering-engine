
#include "../../include/rendering-system/render_command_buffer_manager.h"

namespace Engine
{

	RenderCommandBufferManager::RenderCommandBufferManager(GPUBufferManager& gpuBufferManager) :
		m_gpuBufferManager(gpuBufferManager)
	{


		for (int i = 0; i < MAX_NUM_OF_FRAMES; ++i)
		{
			//1 MiB each
			m_perFrameAllocatorGroups.push_back(
				AllocatorGroup(
					GPUBufferPerFrameAllocator(m_gpuBufferManager.createMappedIBO(1024 * 1024)),
					GPUBufferPerFrameAllocator(m_gpuBufferManager.createMappedIBO(1024 * 1024))

				)
			);
	
		}

	}



	GPUBufferInfo RenderCommandBufferManager::getArrayGPUBufferForThisFrame()
	{



		return m_perFrameAllocatorGroups[m_currentFrameIndex].gpuBufferPerFrameAllocatorForArrayCommands.getGPUBufferInfo();
	}

	GPUBufferInfo RenderCommandBufferManager::getIndexedGPUBufferForThisFrame()
	{



		return m_perFrameAllocatorGroups[m_currentFrameIndex].gpuBufferPerFrameAllocatorForIndexedCommands.getGPUBufferInfo();
	}

	size_t RenderCommandBufferManager::uploadArrayCommandsForCurrentFrame(std::byte* data, const size_t size)
	{

		


		GPUBufferPerFrameAllocator& gpuBufferPerFrameAllocator =  m_perFrameAllocatorGroups[m_currentFrameIndex].gpuBufferPerFrameAllocatorForArrayCommands;


		AllocationInfo allocationInfo = gpuBufferPerFrameAllocator.allocate(size);
		GPUBufferInfo gpuBufferInfo = gpuBufferPerFrameAllocator.getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset

		
	}


	size_t RenderCommandBufferManager::uploadIndexedCommandsForCurrentFrame(std::byte* data, const size_t size)
	{





		GPUBufferPerFrameAllocator& gpuBufferPerFrameAllocator = m_perFrameAllocatorGroups[m_currentFrameIndex].gpuBufferPerFrameAllocatorForIndexedCommands;


		AllocationInfo allocationInfo = gpuBufferPerFrameAllocator.allocate(size);
		GPUBufferInfo gpuBufferInfo = gpuBufferPerFrameAllocator.getGPUBufferInfo();

		std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

		std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
		memcpy(absoluteBufferPtr, data, size);

		return allocationInfo.offset; //byte offset we might need a higher level offset





	}



	void RenderCommandBufferManager::nextFrame()
	{
		++m_currentFrameIndex;
		m_currentFrameIndex = m_currentFrameIndex % MAX_NUM_OF_FRAMES;
		m_perFrameAllocatorGroups[m_currentFrameIndex].gpuBufferPerFrameAllocatorForArrayCommands.reset();
		m_perFrameAllocatorGroups[m_currentFrameIndex].gpuBufferPerFrameAllocatorForIndexedCommands.reset();
	}

}









