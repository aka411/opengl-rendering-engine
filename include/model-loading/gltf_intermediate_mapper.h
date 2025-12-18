#pragma once 

#include "tiny_gltf.h"
#include "model_loading_data_structure.h"
#include "gltf_attribute_extractor.h"


namespace Engine
{



	class GLTFIntermediateMapper
	{


	private:

		template<typename DataType>
		std::uint32_t simpleArrayHash(const std::vector<DataType>& array);



		EngineTransformationComponent getTransformationFromGltfNode(const tinygltf::Node& node); // helper function


		//std::vector<IntermediatePrimitive> getIntermediatePrimitive(tinygltf::Model& tinygltfModel, tinygltf::Mesh& gltfMesh);//helper


		std::vector<SamplerSetting> getTextureSamplerSettings(const tinygltf::Model& tinygltfModel);
		std::vector<IntermediateTexture> getIntermediateTextures(const tinygltf::Model& tinygltfModel);
		std::vector<IntermediateImage> extractIntermediateImages(const tinygltf::Model& tinygltfModel);
		std::vector<IntermediatePBRMetallicRoughnessMaterial> getIntermediateMaterials(const tinygltf::Model& tinygltfModel);

		std::vector<EngineIntermediateNode> getEngineIntermediateNodes(const tinygltf::Model& tinygltfModel);

		IndexType gltfToEngineIndexType(const int componentType);

	    std::vector<std::byte> getRawByteDataFromBufferView(const tinygltf::Accessor& accessor, const tinygltf::BufferView bufferView, const tinygltf::Buffer& buffer);


		std::map<VertexAttributeType, int> getGLTFAttributeAccessorMappedToEngineAttribute(const tinygltf::Primitive& tinygltfPrimitive);


		ExtractedAttributeData getVertexDataFromAccessors(const std::vector<AttributeExtractionResult>& attributeExtractionResults, const tinygltf::Model& tinygltfModel);
	
		
		MeshRelatedData getMeshRelatedData(const tinygltf::Model& tinygltfModel);



	



	public:
		GLTFIntermediateMapper();
		
		~GLTFIntermediateMapper() = default;


		EngineIntermediateModel convertGltfToEngineIntermediateModel(const tinygltf::Model& tinygltfModel);
	};
	template<typename DataType>
	std::uint32_t GLTFIntermediateMapper::simpleArrayHash(const std::vector<DataType>& array)
	{
		// A common prime for initialization (the FNV prime is often used, 2166136261)
		std::uint32_t hash = 2166136261U;

		for (const DataType& in : array)
		{
			// 1. XOR the current byte with the hash
			hash ^= static_cast<std::uint32_t>(in);

			// 2. Multiply the result by a large prime number 
			//    (this spreads the bits around, a key step for good distribution)
			//    (The FNV prime is 16777619)
			hash *= 16777619U;
		}

		return hash;
	}

}