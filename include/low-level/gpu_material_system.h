#pragma once

enum class MaterialType
{
	PBR_METALLIC_ROUGHNESS
};


struct PBRMetallicRoughnessMaterial
{
	//This goes to GPU Padding is crucial

	glm::vec4 baseColorFactor{}; // 4 * 4 = 16 bytes
	glm::vec3 emmissiveFactor{};//optional // 3 * 4 = 12 bytes

	float metallicFactor = 0.0f; // 4 bytes
	float roughnessFactor = 0.0f; // 4 bytes


	uint64_t albedoTextureHandle = 0;// 8 bytes
	uint64_t metallicRoughnessTextureHandle = 0; // 8 bytes

	uint64_t normalTextureHandle = 0; // 8 bytes
	uint64_t occulsionTextureHandle = 0;
	uint64_t emmissiveTextureHandle = 0;


};

using size_t MaterialId;

class GPUMaterialSyste
{

private:

	std::unorderedMap<MaterialType,GPUBufferSubBumpAllocator> m_materialTypeToSSBO;

public:

	MaterialId uploadMaterial(MaterialType materialType, std::byte* ptr, size_t size); //size in bytes
}