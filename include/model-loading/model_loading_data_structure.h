#pragma once
#include <vector>
#include "../components.h"
#include "../low-level/rendering_system_data_types.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstddef>
#include <cstdint>


struct VertexData
{

	VertexLayout vertexLayout;
	VertexFormat vertexFormat;
	size_t vertexCount = 0;

	//Interleaved Only
	std::vector<std::byte> data;
};

struct IndexData
{
	IndexType indexType = IndexType::UNKNOWN;
	size_t indexCount = 0;
	std::vector<std::byte> data;
};

struct IntermediatePrimitive
{
	int vertexDataId = -1; //needs to be converted to offset


	bool isIndexed = false;
	int indexId = -1; //need to be converted into offset


	int materialId = -1;//
};


struct IntermediateMesh
{
	std::vector<IntermediatePrimitive> intermediatePrimitives;
};


//index data is gonna be a bitch

struct EngineIntermediateNode
{
	EngineTransformationComponent transformation;
	int meshIndex = -1;//index to intermediateMeshs
	std::vector<int> children;
};



struct IntermediateImage
{
	size_t width = 0;
	size_t height = 0;
	
	TextureSourcePixelFormat textureSourcePixelFormat = TextureSourcePixelFormat::RGBA;
	TextureSourceComponentType textureSourceComponentType = TextureSourceComponentType::UNSIGNED_BYTE;
	TextureInternalFormat internalFormat = TextureInternalFormat::RGBA8;


	std::vector<std::byte> data;
};


struct IntermediateTexture
{
	int imageIndex = -1;
	int samplerSettingsIndex = -1;
};




struct IntermediatePBRMetallicRoughnessMaterial
{



	glm::vec4 baseColorFactor{}; // 4 * 4 = 16 bytes
	glm::vec3 emissiveFactor{};//optional // 3 * 4 = 12 bytes

	float metallicFactor = 0.0f; // 4 bytes
	float roughnessFactor = 0.0f; // 4 bytes


	int32_t albedoTextureIndex = -1;
	int32_t metallicRoughnessTextureIndex = -1;

	int32_t normalTextureIndex = -1;
	int32_t occlusionTextureIndex = -1;
	int32_t emissiveTextureIndex = -1;


	uint64_t configMask = 0; // 8 bytes

};



struct EngineIntermediateModel
{
	std::vector<VertexData> vertexDatas;
	std::vector<IndexData> indiceDatas;
	std::vector<IntermediateMesh> intermediateMeshs;

	std::vector<EngineIntermediateNode> engineIntermediateNode;//independantly extractable

	std::vector<IntermediatePBRMetallicRoughnessMaterial> intermdediatePBRMetallicRoughnessMaterials; // PBR // use unordered map //independantly gettable
	std::vector<IntermediateTexture> intermediateTextures;//use unordered map
	std::vector<IntermediateImage> intermediateImage;
	std::vector<SamplerSetting> samplerSettings;

	int rootNodeIndex = -1;





	/**CPU DATA**/

	//BoneAnimationData boneAnimationData;




};