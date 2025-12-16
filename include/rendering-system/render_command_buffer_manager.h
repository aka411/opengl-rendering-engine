#pragma once
#include <vector>
#include "../low-level/gpu_buffer_manager.h"
#include "../low-level/gpu_buffer_per_frame_allocator.h"

#include <cstddef>

namespace Engine
{

	class RenderCommandBufferManager
	{
	private:

		struct AllocatorGroup
		{
			GPUBufferPerFrameAllocator gpuBufferPerFrameAllocatorForArrayCommands;
			GPUBufferPerFrameAllocator gpuBufferPerFrameAllocatorForIndexedCommands;
		};


		const int MAX_NUM_OF_FRAMES = 2;
		GPUBufferManager& m_gpuBufferManager;

		std::vector<AllocatorGroup> m_perFrameAllocatorGroups;

		unsigned int m_currentFrameIndex = 0;

	public:
		RenderCommandBufferManager(GPUBufferManager& gpuBufferManager);

		GPUBufferInfo getArrayGPUBufferForThisFrame();
		GPUBufferInfo getIndexedGPUBufferForThisFrame();

		size_t uploadArrayCommandsForCurrentFrame(std::byte* data, const size_t size);
		size_t uploadIndexedCommandsForCurrentFrame(std::byte* data, const size_t size);

		//To be only called by rendersystem at start
		void nextFrame();

	};

}