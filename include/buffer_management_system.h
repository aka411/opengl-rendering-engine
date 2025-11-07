#pragma once
#include "gpu_buffer_manager.h"



class BufferManagementSystem
{

private:
	GPUBufferManager m_gpuBufferManager;

	BufferInfo m_globalVertexBufferInfo;
	BufferInfo m_materialUBOInfo;

	BufferInfo m_cameraUBOInfo;
	BufferInfo m_lightUBOInfo;

	size_t currentOffsetOfVertexBuffer = 0;

public:
	BufferManagementSystem();
	size_t uploadDataToVertexBuffer(void* data, size_t size);

	BufferInfo getGlobalVertexBufferInfo();
	BufferInfo getMaterialUBOInfo();

	BufferInfo getCameraUBOInfo();
	BufferInfo getLightUBOInfo();

};