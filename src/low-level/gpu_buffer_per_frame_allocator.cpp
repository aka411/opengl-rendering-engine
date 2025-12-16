#include "../../include/low-level/gpu_buffer_per_frame_allocator.h"



GPUBufferPerFrameAllocator::GPUBufferPerFrameAllocator(GPUBufferInfo gpuBufferInfo) : 
	m_gpuBufferInfo(gpuBufferInfo)
{




}

GPUBufferInfo GPUBufferPerFrameAllocator::getGPUBufferInfo()
{
	return m_gpuBufferInfo;
}




AllocationInfo GPUBufferPerFrameAllocator::allocate(const size_t size)
{

	if (size + m_currentPointer > m_gpuBufferInfo.size)
	{

		m_currentPointer = 0; //reset for now
	}

	AllocationInfo allocationInfo;

	allocationInfo.offset = m_currentPointer;
	m_currentPointer += size;

	return allocationInfo;

}




void GPUBufferPerFrameAllocator::reset()
{
	m_currentPointer = 0;
}