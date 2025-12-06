#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <vector>

#include "../../include/model-loading/gltf_intermediate_mapper.h"

#include "../../include/model-loading/data_convertor.h"

#include "../../include/model-loading/vertex_attributes_repacker.h"
#include "../../include/model-loading/gltf_animation_extractor.h"
#include "../../include/components.h"
#include "../../include/low-level/vertex_format_helper.h"
#include "../../include/animation/animation_data_structures.h"
#include  <algorithm>
#include "../../include/model-loading/gltf_attribute_extractor.h"




namespace Engine::Utils
{


	TextureSourcePixelFormat tinygltfToTextureSourcePixelFormat(const int tinygltfImagecomponent)
	{



		switch (tinygltfImagecomponent)
		{
		case 1: return TextureSourcePixelFormat::R; break;
		case 2: return TextureSourcePixelFormat::RG; break;
		case 3: return TextureSourcePixelFormat::RGB; break;
		case 4: return TextureSourcePixelFormat::RGBA; break;
		default: return TextureSourcePixelFormat::UNKNOWN; break;

		}

		return TextureSourcePixelFormat::UNKNOWN;;
	}


	TextureSourceComponentType tinygltfToTextureSourceComponentType(const int pixel_type)
	{
		switch (pixel_type)
		{
		case 5121:
			return TextureSourceComponentType::UNSIGNED_BYTE;
		case 5126:
			return TextureSourceComponentType::FLOAT;
		case 5142:
			return TextureSourceComponentType::HALF_FLOAT;

			//case 5123: // GL_UNSIGNED_SHORT
			//	return TextureSourceComponentType::UNSIGNED_SHORT;

		default:
			return TextureSourceComponentType::UNKNOWN;
		}

	}

	




	TextureInternalFormat TextureSourceEngineSoourceToTextureInternalFormat(
		const TextureSourcePixelFormat pixelFormat,
		const TextureSourceComponentType componentType)
	{
		// RENDERED INTERNAL FORMAT MAPPER
		// This function maps the base pixel layout (format) and the data type (component type)
		// to a specific GPU-ready internal storage format.

		
		switch (pixelFormat)
		{
		case TextureSourcePixelFormat::R: // Single component (Red)
			switch (componentType)
			{
			case TextureSourceComponentType::UNSIGNED_BYTE:
				// Example: GL_R8 (8-bit Red)
				return TextureInternalFormat::R8;
			case TextureSourceComponentType::FLOAT:
				// Example: GL_R32F (32-bit float Red)
				return TextureInternalFormat::R32F;
			case TextureSourceComponentType::HALF_FLOAT:
				// Example: GL_R16F (16-bit float Red)
				return TextureInternalFormat::R16F;
			default:
				break;
			}
			break;

		case TextureSourcePixelFormat::RG: // Two components (Red, Green)
			switch (componentType)
			{
			case TextureSourceComponentType::UNSIGNED_BYTE:
				// Example: GL_RG8 (8-bit Red, 8-bit Green)
				return TextureInternalFormat::RG8;
			case TextureSourceComponentType::FLOAT:
				// Example: GL_RG32F (32-bit float Red, 32-bit float Green)
				return TextureInternalFormat::RG32F;
			case TextureSourceComponentType::HALF_FLOAT:
				// Example: GL_RG16F (16-bit float Red, 16-bit float Green)
				return TextureInternalFormat::RG16F;
			default:
				break;
			}
			break;

		case TextureSourcePixelFormat::RGB: // Three components (Red, Green, Blue)
			switch (componentType)
			{
			case TextureSourceComponentType::UNSIGNED_BYTE:
				// Example: GL_RGB8 (8-bit Red, Green, Blue)
				// Note:Might need to consider an sRGB format like SRGB8 if the source is for color data.
				return TextureInternalFormat::RGB8;
			case TextureSourceComponentType::FLOAT:
				// Example: GL_RGB32F
				return TextureInternalFormat::RGB32F;
			case TextureSourceComponentType::HALF_FLOAT:
				// Example: GL_RGB16F
				return TextureInternalFormat::RGB16F;
			default:
				break;
			}
			break;

		case TextureSourcePixelFormat::RGBA: // Four components (Red, Green, Blue, Alpha)
			switch (componentType)
			{
			case TextureSourceComponentType::UNSIGNED_BYTE:
				// Example: GL_RGBA8 (8-bit Red, Green, Blue, Alpha)
				return TextureInternalFormat::RGBA8;
			case TextureSourceComponentType::FLOAT:
				// Example: GL_RGBA32F
				return TextureInternalFormat::RGBA32F;
			case TextureSourceComponentType::HALF_FLOAT:
				// Example: GL_RGBA16F
				return TextureInternalFormat::RGBA16F;
			default:
				break;
			}
			break;

		case TextureSourcePixelFormat::UNKNOWN:
		default:
			// Fallthrough for unknown pixel formats or unhandled component types
			break;
		}

		// Default return for unsupported combinations
		return TextureInternalFormat::UNKNOWN;
	}







	/*-----SAMPLER------*/

	TextureFilter convertToEngineMinMagFilter(int tinygltfMinMagFilter)
	{
		switch (tinygltfMinMagFilter)
		{
		case 9728: // GL_NEAREST
			return TextureFilter::NEAREST;
		case 9729: // GL_LINEAR
			return TextureFilter::LINEAR;
		case 9984: // GL_NEAREST_MIPMAP_NEAREST
			return TextureFilter::NEAREST_MIPMAP_NEAREST;
		case 9985: // GL_LINEAR_MIPMAP_NEAREST
			return TextureFilter::LINEAR_MIPMAP_NEAREST;
		case 9986: // GL_NEAREST_MIPMAP_LINEAR
			return TextureFilter::NEAREST_MIPMAP_LINEAR;
		case 9987: // GL_LINEAR_MIPMAP_LINEAR
			return TextureFilter::LINEAR_MIPMAP_LINEAR;
		default:
			// Handle unknown/unsupported value (e.g., glTF spec violations)
			return TextureFilter::UNKNWON;
		}
	}
	TextureWrap convertToEngineWrapMode(int tinygltfWrapMode)
	{
		switch (tinygltfWrapMode)
		{
		case 10497: // GL_REPEAT
			return TextureWrap::REPEAT;
		case 33071: // GL_CLAMP_TO_EDGE
			return TextureWrap::CLAMP_TO_EDGE;
		case 33648: // GL_MIRRORED_REPEAT
			return TextureWrap::MIRRORED_REPEAT;
		default:
			// Handle unknown/unsupported value
			return TextureWrap::UNKNOWN;
		}
	}


}







namespace Engine
{






	EngineTransformationComponent GLTFIntermediateMapper::getTransformationFromGltfNode(const tinygltf::Node& node)
	{

		if (!node.matrix.empty())
		{
			EngineTransformationComponent transformation;
			glm::mat4 matrix = DataConvertor::narrowToMatrix4(node.matrix);

			// **Decomposition**
			glm::vec3 scale{ 1.0f,1.0f,1.0f };
			glm::quat rotation{ 1.0f,0.0f,0.0f, 0.0f };
			glm::vec3 translation{ 0.0f,0.0f,0.0f };
			glm::vec3 skew;
			glm::vec4 perspective;

			// Decompose the matrix into its components (T, R, S)

			bool matrixDecomposingResult = glm::decompose(
				matrix,
				scale,
				rotation,
				translation,
				skew,
				perspective
			);


			if (!matrixDecomposingResult) std::cout << " Decomposing failed" << std::endl;

			// Populate the engine components from the decomposition
			transformation.position = translation;
			transformation.scale = scale;
			transformation.rotation = rotation;

			transformation.localTransformMatrix = matrix;
			return  transformation;
		}

		EngineTransformationComponent transformation;

		if (!node.translation.empty())
		{
			transformation.position = DataConvertor::narrowToVec3(node.translation);
		}

		if (!node.scale.empty())
		{
			transformation.scale = DataConvertor::narrowToVec3(node.scale);
		}

		if (!node.rotation.empty())
		{
			transformation.rotation = DataConvertor::narrowToQuat(node.rotation);
		}



		return transformation;

	}


	
	std::vector<IntermediateImage> GLTFIntermediateMapper::extractIntermediateImages(const tinygltf::Model& tinygltfModel)
	{
		std::vector<IntermediateImage> intermediateImages;
		intermediateImages.reserve(tinygltfModel.images.size());
		for (auto& tinygltfImage : tinygltfModel.images)
		{
			IntermediateImage intermediateImage;
			intermediateImage.width = tinygltfImage.width;
			intermediateImage.height = tinygltfImage.height;
			
			intermediateImage.textureSourcePixelFormat = Engine::Utils::tinygltfToTextureSourcePixelFormat(tinygltfImage.component); // tinygltf to engine
			intermediateImage.textureSourceComponentType = Engine::Utils::tinygltfToTextureSourceComponentType(tinygltfImage.pixel_type);
			intermediateImage.internalFormat = Engine::Utils::TextureSourceEngineSoourceToTextureInternalFormat(intermediateImage.textureSourcePixelFormat, intermediateImage.textureSourceComponentType);
			const std::size_t byteCount = tinygltfImage.image.size();



			if (byteCount > 0)
			{
				intermediateImage.data.resize(byteCount);
				std::memcpy(
					intermediateImage.data.data(),   // Destination pointer (std::byte*)
					tinygltfImage.image.data(),         // Source pointer (unsigned char*)                          
					byteCount                                     // Number of bytes to copy
				);
			}

			intermediateImages.push_back(intermediateImage);
		}

		return intermediateImages;
	}

	std::vector<SamplerSetting> GLTFIntermediateMapper::getTextureSamplerSettings(const tinygltf::Model& tinygltfModel)
	{
		const std::vector<tinygltf::Sampler>& samplers = tinygltfModel.samplers;

		std::vector<SamplerSetting> samplersettings;
		samplersettings.reserve(samplers.size());

		for (const auto& tinygltfSamplerSetting : samplers)
		{
			SamplerSetting samplerSetting;

			samplerSetting.minFilter = Engine::Utils::convertToEngineMinMagFilter(tinygltfSamplerSetting.minFilter);
			samplerSetting.magFilter = Engine::Utils::convertToEngineMinMagFilter(tinygltfSamplerSetting.magFilter);

			samplerSetting.wrapS = Engine::Utils::convertToEngineWrapMode(tinygltfSamplerSetting.wrapS);
			samplerSetting.wrapT = Engine::Utils::convertToEngineWrapMode(tinygltfSamplerSetting.wrapT);
			//samplerSetting.wrapR = tinygltfSamplerSetting.
			
			samplersettings.push_back(samplerSetting);
		}



		return samplersettings;
	}

	std::vector<IntermediateTexture> GLTFIntermediateMapper::getIntermediateTextures(const tinygltf::Model& tinygltfModel)
	{

		std::vector<IntermediateTexture> intermediateTextures;
		intermediateTextures.reserve(tinygltfModel.textures.size());
		for (auto& tinygltfTexture : tinygltfModel.textures)
		{
			IntermediateTexture intermediateTexture;
			intermediateTexture.imageIndex = tinygltfTexture.source;
			intermediateTexture.samplerSettingsIndex = tinygltfTexture.sampler;
			intermediateTextures.push_back(intermediateTexture);

		}

		return intermediateTextures;

	}


	std::vector<IntermediatePBRMetallicRoughnessMaterial> GLTFIntermediateMapper::getIntermediateMaterials(const tinygltf::Model& tinygltfModel)
	{


		std::vector<IntermediatePBRMetallicRoughnessMaterial> intermediatePBRMetallicRoughnessMaterials;
		intermediatePBRMetallicRoughnessMaterials.reserve(tinygltfModel.materials.size());

		for (auto& material : tinygltfModel.materials)
		{
			IntermediatePBRMetallicRoughnessMaterial intermediatePBRMetallicRoughnessMaterial;


			const glm::vec4 baseColorFactor = DataConvertor::narrowToVec4(material.pbrMetallicRoughness.baseColorFactor);


			const float metallicFactor = material.pbrMetallicRoughness.metallicFactor;
			const float roughnessFactor = material.pbrMetallicRoughness.roughnessFactor;
			
			glm::vec3 emissiveFactor = DataConvertor::narrowToVec3(material.emissiveFactor);
			

			intermediatePBRMetallicRoughnessMaterial.baseColorFactor = baseColorFactor;
			intermediatePBRMetallicRoughnessMaterial.emissiveFactor = emissiveFactor;
			intermediatePBRMetallicRoughnessMaterial.metallicFactor = metallicFactor;
			intermediatePBRMetallicRoughnessMaterial.roughnessFactor = roughnessFactor;


			const int baseColorTextureIndex = material.pbrMetallicRoughness.baseColorTexture.index;
			const int metallicRoughnessTextureIndex = material.pbrMetallicRoughness.metallicRoughnessTexture.index;

			//optional ones 

			const int normalTextureIndex = material.normalTexture.index;
			const int occlusionTextureIndex = material.occlusionTexture.index;
			const int emissiveTextureIndex = material.emissiveTexture.index;


			const int albedoTexCoordIndex = material.pbrMetallicRoughness.baseColorTexture.texCoord;
			const int metallicRoughnessCoordIndex = material.pbrMetallicRoughness.metallicRoughnessTexture.texCoord;

			const int normalTexCoordIndex = material.normalTexture.texCoord;
			const int emissiveTexCoordIndex = material.emissiveTexture.texCoord;
			const int occlusionTexCoordIndex = material.occlusionTexture.texCoord;

			

			auto texturePresent = [](const int textureIndex) { return (textureIndex >= 0) ? true : false; };

			const bool albedoTexturePresent = texturePresent(baseColorTextureIndex);
			const bool metallicRoughnessPresent = texturePresent(metallicRoughnessTextureIndex); 
			const bool normalTexturePresent = texturePresent(normalTextureIndex); 
			const bool occulsionTexturePresent = texturePresent(occlusionTextureIndex); 
			const bool emissiveTexturePresent = texturePresent(emissiveTextureIndex);

			// Add checks for other potential textures if needed (e.g., clearcoat, transmission)

			intermediatePBRMetallicRoughnessMaterial.albedoTextureIndex = (albedoTexturePresent) ? baseColorTextureIndex : -1;
			intermediatePBRMetallicRoughnessMaterial.metallicRoughnessTextureIndex = (metallicRoughnessPresent) ? metallicRoughnessTextureIndex : -1;
			intermediatePBRMetallicRoughnessMaterial.normalTextureIndex = (normalTexturePresent) ? normalTextureIndex : -1;
			intermediatePBRMetallicRoughnessMaterial.occlusionTextureIndex = (occulsionTexturePresent) ? occlusionTextureIndex : -1; // Corrected spelling and used checked variable
			intermediatePBRMetallicRoughnessMaterial.emissiveTextureIndex = (emissiveTexturePresent) ? emissiveTextureIndex : -1;


			




			intermediatePBRMetallicRoughnessMaterial.materialBitMask = 0;

			//Is actually a uint64_t
			uint64_t& materialBitMask = intermediatePBRMetallicRoughnessMaterial.materialBitMask;

			// 1. --- Set Texture Presence Flags (Bits 0-4) ---
			// Check if the texture index is valid (>= 0) and set the corresponding bit.
			

			if (albedoTexturePresent)
			{
				materialBitMask |= MaterialConfigMask::HAS_ALBEDO_TEX;
			}
			if (metallicRoughnessPresent)
			{
				materialBitMask |= MaterialConfigMask::HAS_MR_TEX;
			}
			if (normalTexturePresent)
			{
				materialBitMask |= MaterialConfigMask::HAS_NORMAL_TEX;
			}
			if (occulsionTexturePresent)
			{ 
				materialBitMask |= MaterialConfigMask::HAS_OCCLUSION_TEX;
			}
			if (emissiveTexturePresent)
			{
				materialBitMask |= MaterialConfigMask::HAS_EMISSIVE_TEX;
			}



			// 2. --- Set Texture Coordinate Index Shifts (Bits 5-14) ---
			// Use bitwise shift to place the texture coordinate index (0 or 1 in glTF) 
			// into its reserved 2-bit field.

			materialBitMask |= (albedoTexCoordIndex << MaterialConfigMask::ALBEDO_TEXCOORD_INDEX_SHIFT);
			materialBitMask |= (metallicRoughnessCoordIndex << MaterialConfigMask::MR_TEXCOORD_INDEX_SHIFT);
			materialBitMask |= (normalTexCoordIndex << MaterialConfigMask::NORMAL_TEXCOORD_INDEX_SHIFT);
			materialBitMask |= (occlusionTexCoordIndex << MaterialConfigMask::OCCLUSION_TEXCOORD_INDEX_SHIFT);
			materialBitMask |= (emissiveTexCoordIndex << MaterialConfigMask::EMISSIVE_TEXCOORD_SHIFT);

		







			intermediatePBRMetallicRoughnessMaterials.push_back(intermediatePBRMetallicRoughnessMaterial);

		}

		return intermediatePBRMetallicRoughnessMaterials;
	}





	std::vector<EngineIntermediateNode> GLTFIntermediateMapper::getEngineIntermediateNodes(const tinygltf::Model& tinygltfModel)
	{
		std::vector<EngineIntermediateNode> engineIntermediateNodes;
		engineIntermediateNodes.reserve(tinygltfModel.nodes.size());
		for (auto& node : tinygltfModel.nodes)
		{


			EngineIntermediateNode engineIntermediateNode;

			engineIntermediateNode.transformation = getTransformationFromGltfNode(node);
			engineIntermediateNode.children = node.children;// just copy
			engineIntermediateNode.meshIndex = node.mesh;

			engineIntermediateNodes.push_back(engineIntermediateNode);

		}


		return engineIntermediateNodes;
	}

	IndexType GLTFIntermediateMapper::gltfToEngineIndexType(const int componentType)
	{
		switch (componentType)
		{
		case TINYGLTF_COMPONENT_TYPE_BYTE:
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			return IndexType::UBYTE_8;

		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		case TINYGLTF_COMPONENT_TYPE_SHORT:
			return IndexType::USHORT_16;

		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			return IndexType::UINT_32;

		default:
			return IndexType::UNKNOWN;
		}
	}

	std::vector<std::byte> GLTFIntermediateMapper::getRawByteDataFromBufferView(const tinygltf::Accessor& accessor, const tinygltf::BufferView bufferView, const tinygltf::Buffer& buffer)
	{


		const std::byte* baseBufferPtr = reinterpret_cast<const std::byte*>(buffer.data.data());


		const size_t dataAbsoluteOffset = accessor.byteOffset + bufferView.byteOffset;

		const std::byte* dataPtr = baseBufferPtr + dataAbsoluteOffset;




		const size_t componentSizeInBytes = tinygltf::GetComponentSizeInBytes(accessor.componentType);
		const size_t vectorComponentCount = tinygltf::GetNumComponentsInType(accessor.type);
		const size_t vectorSizeInBytes = componentSizeInBytes * vectorComponentCount;

		const size_t vectorCount = accessor.count;
	
		const size_t totalStreamSizeInBytes = vectorCount * vectorSizeInBytes;

		const size_t stride = bufferView.byteStride;

		std::vector<std::byte> rawByteData(totalStreamSizeInBytes);

		if (stride == 0)
		{

			memcpy(rawByteData.data(), dataPtr, totalStreamSizeInBytes);
		}
		else
		{
			std::byte* destPtr = rawByteData.data();

			for (size_t i = 0; i < vectorCount; ++i)
			{
				// Calculate source address: Start + (element index * stride)
				const std::byte* srcElementPtr = dataPtr + (i * stride);

				// Copy 'elementSize' bytes from the source to the destination
				std::memcpy(destPtr, srcElementPtr, vectorSizeInBytes);

				// Advance the destination pointer by the size of the element (not the stride)
				destPtr += vectorSizeInBytes;
			}
		}
		return rawByteData;
	}


	
	
	ExtractedAttributeData GLTFIntermediateMapper::getVertexDataFromAccessors(const std::vector<AttributeExtractionResult>& attributeExtractionResults, const tinygltf::Model& tinygltfModel)
	{
		//These two loops could be merged but for clarity kept separate

	
		const std::vector<tinygltf::Accessor>& accessors = tinygltfModel.accessors;
		const std::vector<tinygltf::BufferView>& bufferViews = tinygltfModel.bufferViews;

		std::vector<VertexAttributeInfo> vertexAttributeInfos;
		vertexAttributeInfos.reserve(attributeExtractionResults.size());
		for(const auto& attributeExtractionResult : attributeExtractionResults)
		{
			vertexAttributeInfos.push_back(attributeExtractionResult.vertexAttributeInfo);
	
		}
	

		//get individual data
		VertexFormat vertexFormat = VertexFormatHelper::createVertexFormatFromAttributeInfos(vertexAttributeInfos);
		ExtractedAttributeData extractedAttributeData;

		extractedAttributeData.vertexAttributeInfos.reserve(attributeExtractionResults.size());
		extractedAttributeData.vertexAttributeRawDatas.reserve(attributeExtractionResults.size());
	
		for (const auto& attributeExtractionResult : attributeExtractionResults)
		{

			const size_t accessorIndex = attributeExtractionResult.accessorIndex;


			const tinygltf::Accessor& accessor = accessors[accessorIndex];
			const tinygltf::BufferView& bufferView = bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = tinygltfModel.buffers[bufferView.buffer];

			extractedAttributeData.vertexAttributeInfos.push_back(attributeExtractionResult.vertexAttributeInfo);
			extractedAttributeData.vertexAttributeRawDatas.push_back (getRawByteDataFromBufferView(accessor, bufferView, buffer));
		

		}
		extractedAttributeData.vertexFormat = vertexFormat;

		return extractedAttributeData;
	}

	




	MeshRelatedData GLTFIntermediateMapper::getMeshRelatedData(const tinygltf::Model& tinygltfModel)
	{

		std::vector<IntermediateMesh> intermediateMeshs;
		std::vector<IndexData> indexDatas;
		std::vector<VertexData> vertexDatas;

		

		//gltf accessor index to our indice vector index
		std::unordered_map<int, int> gltfIndiceAccessorIndexToOurIndexData; // what if different accessor access same bufferview like with different offset use of index data?
		std::unordered_map<std::uint32_t, int> gltfVertexAttributesAccessorHashToOurVertexData;

		intermediateMeshs.reserve(tinygltfModel.meshes.size());

		const std::vector<tinygltf::Accessor>& gltfAccessors = tinygltfModel.accessors;

		for (const auto& tinygltfMesh : tinygltfModel.meshes)
		{
			IntermediateMesh intermediateMesh;
			std::vector<IntermediatePrimitive> intermediatePrimitives;
			intermediatePrimitives.reserve(tinygltfMesh.primitives.size()); // might not process every primtive if attributes are not as expected 
			for (const auto& tinygltfPrimitive : tinygltfMesh.primitives)
			{
				IntermediatePrimitive intermediatePrimitive;


				const std::vector<AttributeExtractionResult> attributeExtractionResults = GLTFAttributeExtractor::getEngineAttributesToAccessorIndex(tinygltfPrimitive, gltfAccessors);//getGLTFAttributeAccessorMappedToEngineAttribute(tinygltfPrimitive);

				intermediatePrimitive.materialId = tinygltfPrimitive.material;


				std::vector<int> orderedAccessors;
				std::transform(attributeExtractionResults.begin(),
					attributeExtractionResults.end(),
					std::back_inserter(orderedAccessors),
					[](const auto& attributeExtractionResult)
					{
						return attributeExtractionResult.accessorIndex;
					});
				std::sort(orderedAccessors.begin(), orderedAccessors.end());

				
				std::uint32_t attributeAccessorHash = simpleArrayHash<int>(orderedAccessors); /// for reusing


				//get vertex data 
				const auto& it = gltfVertexAttributesAccessorHashToOurVertexData.find(attributeAccessorHash);
				if (it == gltfVertexAttributesAccessorHashToOurVertexData.end())
				{


					//extract then store
	
					const ExtractedAttributeData extractedAttributeData = getVertexDataFromAccessors(attributeExtractionResults, tinygltfModel);

					VertexData vertexData = VertexAttributeRepacker::interleaveVertexAttributes(extractedAttributeData);
		
				

					gltfVertexAttributesAccessorHashToOurVertexData[attributeAccessorHash] = vertexDatas.size();
					intermediatePrimitive.vertexDataId = vertexDatas.size();
					
					vertexDatas.push_back(vertexData);
				}
				else
				{
					//reuse vertex data
					intermediatePrimitive.vertexDataId = it->second;

				}


				//get get indice data
				if (tinygltfPrimitive.indices >= 0)
				{
					intermediatePrimitive.isIndexed = true;
					const auto& IndexIterator = gltfIndiceAccessorIndexToOurIndexData.find(tinygltfPrimitive.indices);
					if (IndexIterator == gltfIndiceAccessorIndexToOurIndexData.end())
					{



						//extract 
			
						const tinygltf::Accessor& accessor = tinygltfModel.accessors[tinygltfPrimitive.indices];
						const tinygltf::BufferView& bufferView = tinygltfModel.bufferViews[accessor.bufferView];
						const tinygltf::Buffer& buffer = tinygltfModel.buffers[bufferView.buffer];

						IndexData indexData;
						indexData.indexType = gltfToEngineIndexType(accessor.componentType);
						assert(indexData.indexType != IndexType::UNKNOWN);
						indexData.indexCount = accessor.count;
						indexData.data = getRawByteDataFromBufferView(accessor, bufferView, buffer);//also send whole model

						//store
						gltfIndiceAccessorIndexToOurIndexData[tinygltfPrimitive.indices] = indexDatas.size();

						intermediatePrimitive.indexId = indexDatas.size();

						indexDatas.push_back(indexData);


					}
					else
					{
						//reuse from book keep
						intermediatePrimitive.indexId = IndexIterator->second;

					}





				}




				intermediatePrimitives.push_back(intermediatePrimitive);


			}

			//get primitives 
			intermediateMesh.intermediatePrimitives = intermediatePrimitives;
			intermediateMeshs.push_back(intermediateMesh);
		}


		MeshRelatedData meshRelatedData;
		meshRelatedData.indexDatas = indexDatas;
		meshRelatedData.intermediateMeshs = intermediateMeshs;
		meshRelatedData.intermediateVertexDatas = vertexDatas;

		return meshRelatedData;
	}










	EngineIntermediateModel GLTFIntermediateMapper::convertGltfToEngineIntermediateModel(const tinygltf::Model& tinygltfModel)
	{
		size_t rootNodeIndex = 0;

		for (auto& scene : tinygltfModel.scenes)
		{
			for (auto nodeIndex : scene.nodes)
			{
				if (nodeIndex >= 0)
				{
					rootNodeIndex = nodeIndex;
					break;
				}
			}
		}



		EngineIntermediateModel engineIntermediateModel;

		 
		engineIntermediateModel.samplerSettings = getTextureSamplerSettings(tinygltfModel);
		engineIntermediateModel.intermediateImage = extractIntermediateImages(tinygltfModel);
		engineIntermediateModel.samplerSettings = getTextureSamplerSettings(tinygltfModel);
		engineIntermediateModel.intermediateTextures = getIntermediateTextures(tinygltfModel);
		engineIntermediateModel.intermdediatePBRMetallicRoughnessMaterials = getIntermediateMaterials(tinygltfModel);

		engineIntermediateModel.engineIntermediateNode = getEngineIntermediateNodes(tinygltfModel);


		MeshRelatedData meshRelatedData = getMeshRelatedData(tinygltfModel);


		engineIntermediateModel.intermediateMeshs = meshRelatedData.intermediateMeshs;

		engineIntermediateModel.indiceDatas = meshRelatedData.indexDatas;
		engineIntermediateModel.vertexDatas = meshRelatedData.intermediateVertexDatas;
		
		engineIntermediateModel.rootNodeIndex = tinygltfModel.scenes[0].nodes[0];// very fragile




		/**Animation Area***/


		ExtractedAnimation extractedAnimation = GltfAnimationExtractor::extractAnimation(tinygltfModel);



		BoneAnimationData boneAnimationData = GltfAnimationExtractor::getBoneAnimationData(tinygltfModel);



		engineIntermediateModel.hasAnimations = extractedAnimation.hasAnimations;

		engineIntermediateModel.boneAnimationData = boneAnimationData;
		engineIntermediateModel.animationData = extractedAnimation.animationData;


		engineIntermediateModel.animationsMap = extractedAnimation.animationsMap;

		return engineIntermediateModel;

	}







	GLTFIntermediateMapper::GLTFIntermediateMapper()
	{

	}



}