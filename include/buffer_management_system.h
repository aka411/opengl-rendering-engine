#pragma once
#include "gpu_buffer_manager.h"
#include <cstdint>



class BufferManagementSystem
{

private:
	GPUBufferManager m_gpuBufferManager;

	BufferInfo m_globalVertexBufferInfo;
	BufferInfo m_globalIndexBufferInfo;

	BufferInfo m_materialUBOInfo;
	BufferInfo m_boneUBOInfo;
	BufferInfo m_cameraUBOInfo;

	BufferInfo m_lightUBOInfo;

	size_t m_currentOffsetOfVertexBuffer = 0;
	size_t m_currentOffsetOfIndexBuffer = 0;

public:
	BufferManagementSystem();

	size_t uploadDataToVertexBuffer(const void* data, size_t size);
	size_t uploadDataToIndexBuffer(const void* data, size_t size);

	BufferInfo getGlobalVertexBufferInfo();
	BufferInfo getGlobalIndexBufferInfo();

	BufferInfo getMaterialUBOInfo();

	BufferInfo getCameraUBOInfo();
	BufferInfo getBoneUBOBufferInfo();
	BufferInfo getLightUBOInfo();

};