#pragma once
#include "gpu_buffer_manager.h"



class BufferManagementSystem
{

private:
	GPUBufferManager m_gpuBufferManager;

	BufferInfo m_globalVertexBufferInfo;
	BufferInfo m_globalIndexBufferInfo;

	BufferInfo m_materialUBOInfo;

	BufferInfo m_cameraUBOInfo;
	BufferInfo m_lightUBOInfo;

	size_t m_currentOffsetOfVertexBuffer = 0;
	size_t m_currentOffsetOfIndexBuffer = 0;

public:
	BufferManagementSystem();

	size_t uploadDataToVertexBuffer(void* data, size_t size);
	size_t uploadDataToIndexBuffer(void* data, size_t size);

	BufferInfo getGlobalVertexBufferInfo();
	BufferInfo getMaterialUBOInfo();

	BufferInfo getCameraUBOInfo();
	BufferInfo getLightUBOInfo();

};