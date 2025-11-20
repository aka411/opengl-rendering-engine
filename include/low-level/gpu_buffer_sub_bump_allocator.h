#pragma once 
#include "rendering_system_data_types.h"

class GPUBufferSubBumpAllocator
{
private:
	const GPUBufferInfo m_gpuBufferInfo;
	size_t m_currentPointer = 0;
public:
	GPUBufferSubBumpAllocator(GPUBufferInfo gpuBufferInfo);

	GPUBufferInfo getGPUBufferInfo();

	AllocationInfo allocate(const size_t size);
	//No deallocation instead thinking of whole reset

};