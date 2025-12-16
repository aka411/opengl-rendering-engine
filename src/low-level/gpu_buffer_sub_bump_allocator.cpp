

#include "../../include/low-level/rendering_system_data_types.h"
#include "../../include/low-level/gpu_buffer_sub_bump_allocator.h"
#include <iostream>
#include <cassert>


GPUBufferSubBumpAllocator::GPUBufferSubBumpAllocator(GPUBufferInfo gpuBufferInfo) : m_gpuBufferInfo(gpuBufferInfo)
{




}




AllocationInfo GPUBufferSubBumpAllocator::allocate(const size_t size)
{


	if (size + m_currentPointer > m_gpuBufferInfo.size)
	{
		std::cout << " RUN OUT OF MEMORY IN GPU BUFFER SUB BUMP ALLOCATOR " << std::endl;
		assert(0);
		m_currentPointer = 0; //reset for now
	}

	AllocationInfo allocationInfo;

	allocationInfo.offset = m_currentPointer;
	m_currentPointer += size;

	return allocationInfo;

}



GPUBufferInfo GPUBufferSubBumpAllocator::getGPUBufferInfo()
{
	return m_gpuBufferInfo;
}