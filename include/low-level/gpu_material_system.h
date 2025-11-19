#pragma once
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>
#include "gpu_buffer_sub_bump_allocator.h"

enum class MaterialType
{
	PBR_METALLIC_ROUGHNESS
};


struct PBRMetallicRoughnessMaterial
{
	//This goes to GPU Padding is crucial

	glm::vec4 baseColorFactor{}; // 4 * 4 = 16 bytes
	glm::vec3 emissiveFactor{};//optional // 3 * 4 = 12 bytes

	float metallicFactor = 0.0f; // 4 bytes
	float roughnessFactor = 0.0f; // 4 bytes


	uint64_t albedoTextureHandle = 0;// 8 bytes
	uint64_t metallicRoughnessTextureHandle = 0; // 8 bytes

	uint64_t normalTextureHandle = 0; // 8 bytes
	uint64_t occulsionTextureHandle = 0;
	uint64_t emissiveTextureHandle = 0;


};

using MaterialId = uint64_t;

class GPUMaterialSystem
{

private:

	std::unordered_map<MaterialType,GPUBufferSubBumpAllocator> m_materialTypeToSSBO;
	
	MaterialId m_currentMaterialId = 0;

public:
	GPUMaterialSystem();
	MaterialId uploadMaterial(MaterialType materialType, std::byte* ptr, size_t size); //size in bytes

	const GPUBufferInfo getGPUBufferInfo(MaterialType materialType);

}