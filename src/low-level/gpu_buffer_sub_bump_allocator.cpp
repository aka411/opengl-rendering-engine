
#include "gpu_buffer_sub_bump_allocator"


GPUBufferSubBumpAllocator::GPUBufferSubBumpAllocator(BufferInfo bufferInfo) : m_bufferInfo(bufferInfo)
{

}


GPUBufferInfo GPUBufferSubBumpAllocator::getGPUBufferInfo()
{
	return m_gpuBufferInfo;
}