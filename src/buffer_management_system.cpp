#include "../include/buffer_management_system.h"
#include <cassert>

BufferManagementSystem::BufferManagementSystem()
{
	m_globalVertexBufferInfo = m_gpuBufferManager.createMappedVertexBuffer(1024 * 1024 * 100, nullptr);// 100 MiB

	m_materialUBOInfo = m_gpuBufferManager.createMappedUBOBuffer(1024 * 20,nullptr);//20 KiB


	m_cameraUBOInfo = m_gpuBufferManager.createMappedUBOBuffer(65 * 2 , nullptr);// 130 bytes,2 extra bytes just for safety , actually only 128 bytes needed

	m_lightUBOInfo = m_gpuBufferManager.createMappedUBOBuffer(100, nullptr); // 100 bytes currently


}



size_t BufferManagementSystem::uploadDataToVertexBuffer(void* data, size_t size/*in bytes*/)
{
	if(currentOffsetOfVertexBuffer + size > m_globalVertexBufferInfo.totalSize)
	{
		// not enough space
		// in a real system, we would handle this more gracefully
		assert(false);
		return 0;
	}

	if(data == nullptr || size == 0)
	{
		// invalid data
		assert(false);
		return 0;
	}


	const std::uintptr_t vertexbufferOffsetAddress = reinterpret_cast<std::uintptr_t>(m_globalVertexBufferInfo.mappedPtr) + currentOffsetOfVertexBuffer;

	memcpy(reinterpret_cast<void*>(vertexbufferOffsetAddress), data, size);

	const size_t returnOffset = currentOffsetOfVertexBuffer;
	currentOffsetOfVertexBuffer += size;

	return returnOffset;//in byte stride

}

BufferInfo BufferManagementSystem::getGlobalVertexBufferInfo()
{
	return m_globalVertexBufferInfo;
}

BufferInfo BufferManagementSystem::getMaterialUBOInfo()
{


	return m_materialUBOInfo;
}



BufferInfo BufferManagementSystem::getCameraUBOInfo()
{
	return m_cameraUBOInfo;
}

BufferInfo BufferManagementSystem::getLightUBOInfo()
{
	return m_lightUBOInfo;
}


