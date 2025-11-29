#pragma once
#include "ui_component.h"
#include "../../include/low-level/gpu_buffer_manager.h"
#include "../../include/low-level/gpu_buffer_sub_bump_allocator.h"

namespace UI
{


	class UIVertexBufferSystem
	{

	private:

		GPUBufferManager m_gpuBuffermanager;

		std::unordered_map<UIVertexFormat, GPUBufferSubBumpAllocator> m_formatToVertexBufferSubAllocators;


		void createNewVertexBufferForFormat(UIVertexFormat uiVertexFormat);


	public:

		UIVertexBufferSystem();

		//get buffer for given vertex format
		size_t uploadVertexData(UIVertexFormat uiVertexFormat, std::byte* data, size_t size);


		GPUBufferInfo getBufferInfoForVertexFormat(UIVertexFormat uiVertexFormat);



	};


}