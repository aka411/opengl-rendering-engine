#include "../../include/low-level/gpu_material_system.h"



GPUMaterialSystem::GPUMaterialSystem(GPUBufferManager& gpuBufferManager) : m_gpuBufferManager(gpuBufferManager)
{
	//create PBR SSBO
	GPUBufferInfo gpuBufferInfo = m_gpuBufferManager.createMappedSSBO(10 * 1024 * 1024, nullptr); //10 MiB

	GPUBufferSubBumpAllocator gpuBufferSubBumpAllocator(gpuBufferInfo);
	m_materialTypeToSSBO.emplace(
		MaterialType::PBR_METALLIC_ROUGHNESS,
		std::move(gpuBufferSubBumpAllocator));
}

MaterialId GPUMaterialSystem::uploadMaterial(MaterialType materialType, std::byte* ptr, size_t size) //size in bytes
{
	

	//upload the and  give out material id
	GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_materialTypeToSSBO.at(materialType);


	AllocationInfo allocationInfo = gpuBufferSubBumpAllocator.allocate(size);
	GPUBufferInfo gpuBufferInfo  = gpuBufferSubBumpAllocator.getGPUBufferInfo();
	std::byte* bufferBasePtr = reinterpret_cast<std::byte*>(gpuBufferInfo.mappedPtr);

	std::byte* absoluteBufferPtr = bufferBasePtr + allocationInfo.offset;
	memcpy(absoluteBufferPtr, ptr, size);

	
	return m_currentMaterialId++;
}


const GPUBufferInfo GPUMaterialSystem::getGPUBufferInfo(MaterialType materialType)
{


	GPUBufferSubBumpAllocator& gpuBufferSubBumpAllocator = m_materialTypeToSSBO.at(materialType);

	return gpuBufferSubBumpAllocator.getGPUBufferInfo();
}
