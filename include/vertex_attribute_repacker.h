#pragma once
#include <unordered_map>
#include  <cstddef>
#include "tiny_gltf.h"
#include "engine_data_structures.h"

struct ExtractedAttribute
{
	size_t numOfBytePerComponent = 0;
	size_t totalNumOfComponents = 0;
	std::vector<std::byte> rawData;
};

struct ExtractedVertexAttributeMap
{
	std::unordered_map<Engine::VertexAttributes, ExtractedAttribute> vertexAttributeMap;
};

struct VertexAttributeInGltfIndices
{
	int accessorIndex = -1;
	int bufferViewIndex = -1;

};
struct VertexAttributeGltfLocationMap
{
	std::unordered_map < Engine::VertexAttributes, VertexAttributeInGltfIndices> vertexAttributeGltfLocationMap;
};


struct BufferByteMetrics
{
	 size_t absoluteBufferOffset = 0;
	 size_t byteStride = 0;
	 size_t numOfBytePerComponent = 0;
	 size_t totalBytes = 0;
	 size_t totalNumOfComponents = 0;//component here means a single attribute and not a component of the attribute
};

struct VertexAttributeInfo
{
	size_t bytesPerAttribute = 0;
};


class VertexAttributeRepacker
{
private:



	static BufferByteMetrics getBufferByteMetrics(tinygltf::Accessor&  accessor, tinygltf::BufferView& bufferView);
	static ExtractedAttribute extractAttributeFromInterleaved(tinygltf::Buffer& buffer,const BufferByteMetrics& bufferByteMetrics);
	static ExtractedAttribute extractAttributeFromNonInterleaved(tinygltf::Buffer& buffer, const BufferByteMetrics& bufferByteMetrics);
public:
	//from accessor
	static const std::unordered_map<int, int> m_typeToNumOfComponentsMap;

	//from accessor
	static const std::unordered_map<int, int> m_componentTypeToByteSizeMap;

	//engine Format
	static const std::unordered_map<Engine::VertexAttributes, int> m_attributeOffsetMap;


	static ExtractedVertexAttributeMap extractVertexAttributesFromInterleaved(VertexAttributeGltfLocationMap vertexAttributeGltfLocationMap, tinygltf::Model& tinygltfModel);
	static ExtractedVertexAttributeMap extractVertexAttributesFromNonInterleaved(VertexAttributeGltfLocationMap vertexAttributeGltfLocationMap, tinygltf::Model& tinygltfModel);
	static std::vector<std::byte> interleaveAttributes(ExtractedVertexAttributeMap& extractedVertexAttributeMap, std::vector<Engine::VertexAttributes> packingFormat);
	

};