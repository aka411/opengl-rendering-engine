#pragma once 
#include "rendering_system_data_types.h"

class GPUBufferPerFrameAllocator
{


private:


	const GPUBufferInfo m_gpuBufferInfo;
	size_t m_currentPointer = 0;

public:
	GPUBufferPerFrameAllocator(GPUBufferInfo gpuBufferInfo);


	GPUBufferInfo getGPUBufferInfo();

	AllocationInfo allocate(const size_t size);

	void reset();

};