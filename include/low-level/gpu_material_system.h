#pragma once
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>
#include "gpu_buffer_sub_bump_allocator.h"
#include <unordered_map>
#include "gpu_buffer_manager.h"








/*uint64_t materialBitMask       DEFINTION
*
* We need to support upto 5 texcoords so need 3 bits per texture
* TexCoord indices map to : 000 -> 0 , 001 -> 1 , 010 -> 2 ...
*
* We will use a 64 bit integer data type
*
|<---------------Presence flags (5 bits, one bit each)--------------------->|<------------------------TexCoord Indices (3 bit each,15 bits)----------------->|<------------------Unused (44 bits)------------------>|
|Albedo-Present|MR-Present|Normal-Present|Occlusion-Present|Emissive-Present|albedo-TexCoord|MR-TexCoord|Normal-TexCoord|Occlusion-TexCoord|Emissive-TexCoord|///////////////////Unused/////////////////////////////|
0			   1               2               3                 4               5-7           8-10           11-13          14-16               17-19       20                                                     63


*/






enum class MaterialType
{
	PBR_METALLIC_ROUGHNESS
};




namespace MaterialConfigMask
{
	using MaskType = uint64_t;
	// ---  Texture Presence Flags (Bits 0-4) ---
	// 
	// A single bit to check if a texture handle is valid and should be sampled.
	constexpr MaskType HAS_ALBEDO_TEX = 1u << 0;
		constexpr MaskType	HAS_MR_TEX = 1u << 1;
		constexpr MaskType	HAS_NORMAL_TEX = 1u << 2;
		constexpr MaskType	HAS_OCCLUSION_TEX = 1u << 3;
		constexpr MaskType	HAS_EMISSIVE_TEX = 1u << 4;

		// ---  Texture Coordinate Index Shifts (3 bits each )(Bits 5-19) ---

		// Base Color/Albedo TexCoord Index 
		constexpr MaskType	ALBEDO_TEXCOORD_INDEX_SHIFT = 5;
		constexpr MaskType ALBEDO_TEXCOORD_INDEX_MASK = 0b111u << ALBEDO_TEXCOORD_INDEX_SHIFT;

		// Metallic Roughness TexCoord Index 
		constexpr MaskType MR_TEXCOORD_INDEX_SHIFT = 8;
		constexpr MaskType MR_TEXCOORD_INDEX_MASK = 0b111u << MR_TEXCOORD_INDEX_SHIFT;

		// Normal TexCoord Index 
		constexpr MaskType NORMAL_TEXCOORD_INDEX_SHIFT = 11;
		constexpr MaskType NORMAL_TEXCOORD_INDEX_MASK = 0b111u << NORMAL_TEXCOORD_INDEX_SHIFT;

		// Occlusion TexCoord Index 
		constexpr MaskType OCCLUSION_TEXCOORD_INDEX_SHIFT = 14;
		constexpr MaskType	OCCLUSION_TEXCOORD_INDEX_MASK = 0b111u << OCCLUSION_TEXCOORD_INDEX_SHIFT;


		constexpr MaskType EMISSIVE_TEXCOORD_SHIFT = 17;
		constexpr MaskType EMISSIVE_TEXCOORD_MASK = 0b111u << EMISSIVE_TEXCOORD_SHIFT;
		//till bit 19
};


struct PBRMetallicRoughnessMaterial
{
	//This goes to GPU Padding is crucial

	//N = 4
	//Alignement : 4N , 4 * 4 =16
	glm::vec4 baseColorFactor{ 0 };//4N ,offset =  0
	glm::vec3 emissiveFactor{ 0 };// offset = 16

	//Alignement : N , 4
	float metallicFactor = 0.0f; //offset = 28
	float roughnessFactor = 0.0f; //offset = 32

	float padding = 0;//offset = 36

	// Alignement : 2N , 2*4 = 8 
	uint64_t albedoTextureHandle = 0; //offset = 40
	uint64_t metallicRoughnessTextureHandle = 0; //offset = 48

	uint64_t normalTextureHandle = 0; //offset = 56
	uint64_t occlusionTextureHandle = 0;// offset = 64
	uint64_t emissiveTextureHandle = 0;//offset =  72

	uint64_t materialBitMask = 0; //offset = 80

	//Total before padding2: 88 bytes
	uint64_t padding2 = 0; // Padding to make the struct size a multiple of 16 bytes
	//Total Size = 96 bytes
};

using MaterialId = uint64_t;

class GPUMaterialSystem
{

private:
	GPUBufferManager& m_gpuBufferManager;


	std::unordered_map<MaterialType, GPUBufferSubBumpAllocator> m_materialTypeToSSBO;

	MaterialId m_currentMaterialId = 0;

public:
	GPUMaterialSystem(GPUBufferManager& gpuBufferManager);
	MaterialId uploadMaterial(MaterialType materialType, std::byte* ptr, size_t size); //size in bytes

	const GPUBufferInfo getGPUBufferInfo(MaterialType materialType);

};