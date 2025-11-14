#include "../include/buffer_management_system.h"
#include <cassert>
#include <cstdint>
#include <cstring>

BufferManagementSystem::BufferManagementSystem()
{
	/*** VERTEX AND INDEX BUFFER AREA ***/
	m_globalVertexBufferInfo = m_gpuBufferManager.createMappedVertexBuffer(1024 * 1024 * 100, nullptr);// 100 MiB
	m_globalIndexBufferInfo = m_gpuBufferManager.createMappedIndexBuffer(1024 * 1024 * 10, nullptr);// 10 MiB




	/*** UBO AREA ***/

	m_materialUBOInfo = m_gpuBufferManager.createMappedUBOBuffer(1024 * 20,nullptr);//20 KiB


	m_cameraUBOInfo = m_gpuBufferManager.createMappedUBOBuffer(65 * 2 , nullptr);// 130 bytes,2 extra bytes just for safety , actually only 128 bytes needed

	m_boneUBOInfo = m_gpuBufferManager.createMappedUBOBuffer((100 * 64)+20, nullptr);// 6420 bytees, good for 100 mat4

	m_lightUBOInfo = m_gpuBufferManager.createMappedUBOBuffer(100, nullptr); // 100 bytes currently


}



size_t BufferManagementSystem::uploadDataToVertexBuffer(const void* data, size_t size/*in bytes*/)
{
	if(m_currentOffsetOfVertexBuffer + size > m_globalVertexBufferInfo.totalSize)
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


	const std::uintptr_t vertexbufferOffsetAddress = reinterpret_cast<std::uintptr_t>(m_globalVertexBufferInfo.mappedPtr) + m_currentOffsetOfVertexBuffer;

	memcpy(reinterpret_cast<void*>(vertexbufferOffsetAddress), data, size);

	const size_t returnOffset = m_currentOffsetOfVertexBuffer;
	m_currentOffsetOfVertexBuffer += size;

	return returnOffset;//in byte stride

}

size_t BufferManagementSystem::uploadDataToIndexBuffer(const void* data, size_t size)
{

	if (m_currentOffsetOfIndexBuffer + size > m_globalIndexBufferInfo.totalSize)
	{
		// not enough space
		// in a real system, we would handle this more gracefully
		assert(false);
		return 0;
	}

	if (data == nullptr || size == 0)
	{
		// invalid data
		assert(false);
		return 0;
	}


	const std::uintptr_t indexbufferOffsetAddress = reinterpret_cast<std::uintptr_t>(m_globalIndexBufferInfo.mappedPtr) + m_currentOffsetOfIndexBuffer;

	memcpy(reinterpret_cast<void*>(indexbufferOffsetAddress), data, size);

	const size_t returnOffset = m_currentOffsetOfIndexBuffer;
	m_currentOffsetOfIndexBuffer += size;

	return returnOffset;

	
}

BufferInfo BufferManagementSystem::getGlobalVertexBufferInfo()
{
	return m_globalVertexBufferInfo;
}

BufferInfo BufferManagementSystem::getGlobalIndexBufferInfo()
{
	return m_globalIndexBufferInfo;
}

BufferInfo BufferManagementSystem::getMaterialUBOInfo()
{


	return m_materialUBOInfo;
}



BufferInfo BufferManagementSystem::getCameraUBOInfo()
{
	return m_cameraUBOInfo;
}

BufferInfo BufferManagementSystem::getBoneUBOBufferInfo()
{
	return m_boneUBOInfo;
}

BufferInfo BufferManagementSystem::getLightUBOInfo()
{
	return m_lightUBOInfo;
}


