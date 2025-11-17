#pragma once 


class GPUBufferSubBumpAllocator
{
private:
	const GPUBufferInfo m_gpuBufferInfo;
	size_t currentPointer = 0;
private:
	GPUBufferSubBumpAllocator(BufferInfo bufferInfo);

	GPUBufferInfo getGPUBufferInfo();

	AllocationInfo allocate(const size_t size);
	//No deallocation instead thinking of whole reset

};