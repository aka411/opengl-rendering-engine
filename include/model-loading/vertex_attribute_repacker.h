#pragma once
#include <unordered_map>
#include  <cstddef>
#include "tiny_gltf.h"
#include "engine_data_structures.h"

struct ExtractedAttribute
{
	size_t numOfBytesOfOneComponentOfItem = 0;
	size_t numOfBytesPerItem = 0;
	size_t totalNumOfItems = 0;
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

	 size_t numOfComponentsPerItem = 0;
	 size_t numOfBytesOfOneComponentOfItem = 0;// vec3(float) has 3 components and this means one component is 4 bytes 

	 size_t numOfBytesPerItem = 0; // f(vec3) = 3 * 4 = 12 bytes , size of one item
	 size_t totalNumOfItems = 0; // vec3 * n, n is the value noted here

	 size_t totalBytes = 0;
	
};

struct VertexAttributeInfo
{
	size_t bytesPerAttribute = 0;
};



struct InterleavedData
{
	std::vector<std::byte> interleavedData;
	size_t stride; //in byte
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
	static InterleavedData interleaveAttributes(ExtractedVertexAttributeMap& extractedVertexAttributeMap, std::vector<Engine::VertexAttributes> packingFormat);
	

};