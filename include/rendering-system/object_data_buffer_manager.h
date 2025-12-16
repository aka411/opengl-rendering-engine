#pragma once
#include "../low-level/gpu_buffer_manager.h"
#include "../low-level/gpu_buffer_per_frame_allocator.h"

#include  <cstddef>


namespace Engine
{
	class ObjectDataBufferManager
	{

	private:

		const int MAX_NUM_OF_FRAMES = 2;

		GPUBufferManager& m_gpuBufferManager;

		std::vector<GPUBufferPerFrameAllocator> m_gpuBufferPerFrameAllocators;

		unsigned int m_currentFrameIndex = 0;

	public:

		ObjectDataBufferManager(GPUBufferManager& gpuBufferManager);


		GPUBufferInfo getGPUBufferForThisFrame();
		size_t uploadDataForCurrentFrame(std::byte* data, const size_t size);

		//To be only called by rendersystem at start
		void nextFrame();
	};

}