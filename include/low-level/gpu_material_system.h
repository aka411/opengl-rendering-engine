#pragma once
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>
#include "gpu_buffer_sub_bump_allocator.h"
#include <unordered_map>
#include "gpu_buffer_manager.h"

enum class MaterialType
{
	PBR_METALLIC_ROUGHNESS
};

enum PBR_CONFIG_BITS : uint32_t
{
    // ---  Texture Presence Flags (Bits 0-4) ---
    // A single bit to check if a texture handle is valid and should be sampled.
    HAS_ALBEDO_TEX = 1u << 0, // Base Color Texture
    HAS_MR_TEX = 1u << 1, // Metallic Roughness Texture
    HAS_NORMAL_TEX = 1u << 2, // Normal Texture
    HAS_OCCLUSION_TEX = 1u << 3, // Occlusion Texture
    HAS_EMISSIVE_TEX = 1u << 4, // Emissive Texture

    // ---  Texture Coordinate Index Shifts (Bits 5-14) ---
    // The starting bit for a 2-bit field (0, 1, or 2, reserved for future)
    // The value (0 or 1) will be stored at this starting position.

    // Base Color/Albedo TexCoord Index (e.g., bits 5 & 6)
    ALBEDO_TEXCOORD_SHIFT = 5,

    // Metallic Roughness TexCoord Index (e.g., bits 7 & 8)
    MR_TEXCOORD_SHIFT = 7,

    // Normal TexCoord Index (e.g., bits 9 & 10)
    NORMAL_TEXCOORD_SHIFT = 9,

    // Occlusion TexCoord Index (e.g., bits 11 & 12)
    OCCLUSION_TEXCOORD_SHIFT = 11,

    // Emissive TexCoord Index (e.g., bits 13 & 14)
    EMISSIVE_TEXCOORD_SHIFT = 13,

    // ---  Optional Vertex Attribute Flags (Bits 15+) ---
    // A single bit to check if the mesh primitive contains the vertex attribute.
    HAS_VERTEX_COLOR = 1u << 15, // COLOR_0 attribute presence
    HAS_TANGENTS = 1u << 16, // TANGENT attribute presence (needed for normal mapping)

    // ---  Alpha Mode Flags (Bits 17-18) ---
    // Use a 2-bit field to store the glTF alpha mode (0=OPAQUE, 1=MASK, 2=BLEND)
    ALPHA_MODE_SHIFT = 17,

    // ---  Other Flags (Bit 19+) ---
    IS_DOUBLESIDED = 1u << 19,
    IS_UNLIT = 1u << 20

  
};


struct PBRMetallicRoughnessMaterial
{
	//This goes to GPU Padding is crucial

	glm::vec4 baseColorFactor{0}; // 4 * 4 = 16 bytes
	glm::vec3 emissiveFactor{0};//optional // 3 * 4 = 12 bytes


	float metallicFactor = 0.0f; // 4 bytes
	float roughnessFactor = 0.0f; // 4 bytes

    float padding1 = 0;                     // Offset 36, Size 4.
    float padding2 = 0;                     // Offset 40, Size 4.
    float padding3 = 0;

	uint64_t albedoTextureHandle =0;// 8 bytes
	uint64_t metallicRoughnessTextureHandle = 0; // 8 bytes

	uint64_t normalTextureHandle = 0; // 8 bytes
	uint64_t occlusionTextureHandle = 0;
	uint64_t emissiveTextureHandle = 0;

	uint64_t configMask = 0; // 8 bytes

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