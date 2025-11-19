#include "low-level/gpu_material_system.h"




MaterialId GPUMaterialSystem::uploadMaterial(MaterialType materialType, std::byte* ptr, size_t size) //size in bytes
{
	

	//upload the and  give out material id
	GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_materialTypeToSSBO.at(materialType);


	AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
	GPUBufferInfo gpuBufferInfo  = gpuBufferSubBumpAllocator.getGPUBufferInfo();
	std::byte* bufferBasePtr = gpuBufferInfo.mappedPtr;

	std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
	memcpy(absoluteBufferPtr, ptr, size);

	return m_currentMaterialId++;
}


const GPUBufferInfo GPUMaterialSystem::getGPUBufferInfo(MaterialType materialType)
{


	GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_materialTypeToSSBO.at(materialType);

	return gpuBufferSubBumpAllocator.getGPUBufferInfo();
}
