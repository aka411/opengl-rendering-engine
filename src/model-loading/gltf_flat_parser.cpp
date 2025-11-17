#include "../include/gltf_flat_parser.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

#include "../include/data_convertor.h"
#include "../include/gltf_animation_extractor.h"


namespace Engine
{








	std::vector<GLuint> GLTFFlatParser::extractImages(tinygltf::Model& tinygltfModel)
	{
		std::vector<GLuint> imageList;
		for (auto& image : tinygltfModel.images)
		{
			imageList.push_back(m_gpuTextureManager.createNewTexture(image.width, image.height, image.component, image.image.data()));
		}

		return imageList;
	}

	std::vector<GLuint> GLTFFlatParser::extractTextures(tinygltf::Model& tinygltfModel)
	{
		std::vector<GLuint> textureList;

		for (auto& texture : tinygltfModel.textures)
		{
			int sourceImageIndex = texture.source;
			if (sourceImageIndex >= 0 && sourceImageIndex < tinygltfModel.images.size())
			{


				textureList.push_back(m_imageList[sourceImageIndex]);
			}
			else
			{
				// Handle case where there is no valid source image
				textureList.push_back(0); // or some default texture ID
			}
		}


		return textureList;
	}


	std::vector<Material> GLTFFlatParser::extractMaterial(tinygltf::Model& tinygltfModel)
	{
		//first of all upload all textures to gpu and map it here
		std::vector<GLuint> textures = m_textureList;


		//second of all parse materal and create material data
		std::vector<Material> materials;
		materials.reserve(tinygltfModel.materials.size());
		for (auto& material : tinygltfModel.materials)
		{
			Material ourMaterial;
			//pbr
			//const glm::vec4 baseColorFactor = glm::vec4(material.pbrMetallicRoughness.baseColorFactor);//vec4
			const int baseColorTextureIndex = material.pbrMetallicRoughness.baseColorTexture.index;

			const float metallicFactor = material.pbrMetallicRoughness.metallicFactor;//float
			const float roughnessFactor = material.pbrMetallicRoughness.roughnessFactor;

			const int metallicRoughnessTextureIndex = material.pbrMetallicRoughness.metallicRoughnessTexture.index;

			//optional ones 

			const int normalTextureIndex = material.normalTexture.index;
			const int occlusionTextureIndex = material.occlusionTexture.index;
			const int emissiveTextureIndex = material.emissiveTexture.index;
			glm::vec3 emmissiveFactor = DataConvertor::narrowToVec3(material.emissiveFactor);

			ourMaterial.materialPBRMetallicRoughnessConstValues.emmissiveFactor = emmissiveFactor;
			ourMaterial.materialPBRMetallicRoughnessConstValues.metallicFactor = metallicFactor;
			ourMaterial.materialPBRMetallicRoughnessConstValues.roughnessFactor = roughnessFactor;
			if (baseColorTextureIndex >= 0)
			{
				ourMaterial.materialPBRMetallicRoughnessTexture.baseColourTexture = textures[baseColorTextureIndex];
				ourMaterial.materialPBRMetallicRoughnessConstValues.baseColorTexturePresent = true;
			}
			if (metallicRoughnessTextureIndex >= 0)
			{
				ourMaterial.materialPBRMetallicRoughnessTexture.metallicRoughnessTexture = textures[metallicRoughnessTextureIndex];
				ourMaterial.materialPBRMetallicRoughnessConstValues.metallicRoughnessTexturePresent = true;
			}


			if (normalTextureIndex >= 0)
			{
				ourMaterial.materialPBRMetallicRoughnessTexture.normalTexture = textures[normalTextureIndex];
				ourMaterial.materialPBRMetallicRoughnessConstValues.normalTexturePresent = true;
			}
			if (occlusionTextureIndex >= 0)
			{
				ourMaterial.materialPBRMetallicRoughnessTexture.occulsionTexture = textures[occlusionTextureIndex];
				ourMaterial.materialPBRMetallicRoughnessConstValues.occulsionTexturePresent = true;
			}
			if (emissiveTextureIndex >= 0)
			{
				ourMaterial.materialPBRMetallicRoughnessTexture.emmissiveTexture = textures[emissiveTextureIndex];
				ourMaterial.materialPBRMetallicRoughnessConstValues.emmissiveTexturePresent = true;
			}

			materials.push_back(ourMaterial);

		}

		return materials;
	}


	std::vector<EngineNode> GLTFFlatParser::getNodeList(tinygltf::Model& tinygltfModel)
	{
		std::vector<EngineNode> ourNodeLists;
		ourNodeLists.reserve(tinygltfModel.nodes.size());

		for (auto& node : tinygltfModel.nodes)
		{

			EngineNode engineNode;

			engineNode.transformation = getTransformation(node);
			engineNode.children = node.children; 
			engineNode.meshIndex = node.mesh; 

			ourNodeLists.push_back(engineNode);
		}




		return ourNodeLists;
	}






	Transformation GLTFFlatParser::getTransformation(tinygltf::Node node)
	{

		if (!node.matrix.empty())
		{
			Transformation transformation;
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


			if(!matrixDecomposingResult) std::cout << " Decomposing failed" << std::endl;

			// Populate the engine components from the decomposition
			transformation.position = translation;
			transformation.scale = scale;
			transformation.rotation = rotation;
			
			transformation.localTransformMatrix = matrix;
			return  transformation;
		}

		Transformation transformation;

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

	




	bool GLTFFlatParser::checkIfInterleaved(VertexAttributeGltfLocationMap& vertexAttributeGltfLocationMap)
	{
		assert(!vertexAttributeGltfLocationMap.vertexAttributeGltfLocationMap.empty());

		const int index = vertexAttributeGltfLocationMap.vertexAttributeGltfLocationMap.at(vertexAttributeGltfLocationMap.vertexAttributeGltfLocationMap.begin()->first).bufferViewIndex;

		for (const auto& vertexAttributeGltfLocationMap : vertexAttributeGltfLocationMap.vertexAttributeGltfLocationMap)
		{
			int bufferViewIndex = vertexAttributeGltfLocationMap.second.bufferViewIndex;

			if (index != bufferViewIndex)
			{
				return false;
			}
			
		}



		return true;
	}

	VertexAttributeGltfLocationMap GLTFFlatParser::getVertexAttributeMap(tinygltf::Primitive& primitive, tinygltf::Model& tinygltfModel)
	{

		VertexAttributeGltfLocationMap vertexAttributeGltfLocationMap;

		std::unordered_map<std::string, VertexAttributes> gltfToEngineVertexAttributeMap =
		{
			// Standard glTF Vertex Semantics (Key: glTF String, Value: Engine Enum)
			{"POSITION", Engine::VertexAttributes::POSITION},
			{"NORMAL", Engine::VertexAttributes::NORMAL},
			{"TEXCOORD_0", Engine::VertexAttributes::TEXCOORD_0}, // First UV set
			{"JOINTS_0", Engine::VertexAttributes::JOINT},        // Skinning Joint/Bone Indices
			{"WEIGHTS_0", Engine::VertexAttributes::WEIGHT},      // Skinning Weights

		
			// {"TEXCOORD_1", Engine::VertexAttributes::TEXCOORD_1}, // Second UV set
			// {"COLOR_0", Engine::VertexAttributes::COLOR},         // Vertex Colors
			// {"TANGENT", Engine::VertexAttributes::TANGENT}        // Tangent Vector
		};

		std::vector<tinygltf::Accessor> accessor = tinygltfModel.accessors;

		for (auto& attribute : primitive.attributes)
		{
			const auto& it =  gltfToEngineVertexAttributeMap.find(attribute.first);
			if (it == gltfToEngineVertexAttributeMap.end()) continue;

			const Engine::VertexAttributes vertexAttribute = gltfToEngineVertexAttributeMap[it->first];
			
			VertexAttributeInGltfIndices vertexAttributeInGltfIndices;
			vertexAttributeInGltfIndices.accessorIndex = attribute.second;
			vertexAttributeInGltfIndices.bufferViewIndex = accessor[vertexAttributeInGltfIndices.accessorIndex].bufferView;

			vertexAttributeGltfLocationMap.vertexAttributeGltfLocationMap[vertexAttribute] = vertexAttributeInGltfIndices;

		}




		return vertexAttributeGltfLocationMap;
	}

	std::vector<Primitive> GLTFFlatParser::getPrimitives(tinygltf::Model& tinygltfModel, tinygltf::Mesh& gltfMesh)
	{
		//condition for interleaved is that 
		
		//MVP VERTEX FORMAT
		//struct Vertex
		//{
		// glm::vec3 position; // 12 bytes
		// glm::vec3 normal;   // 12 bytes
		// glm::vec2 texCoord; // 8 bytes
		//}
		std::vector<Engine::VertexAttributes> defualtEngineFormat = {

			Engine::VertexAttributes::POSITION,
			Engine::VertexAttributes::NORMAL,
			Engine::VertexAttributes::TEXCOORD_0
		};

		std::vector<Engine::VertexAttributes> bonedEngineFormat = {

		Engine::VertexAttributes::POSITION,
		Engine::VertexAttributes::NORMAL,
		Engine::VertexAttributes::TEXCOORD_0,
		Engine::VertexAttributes::JOINT,
		Engine::VertexAttributes::WEIGHT
		};


		std::vector<Primitive> primitivesList;
		for (tinygltf::Primitive& gltfPrimitive : gltfMesh.primitives)
		{

			VertexAttributeGltfLocationMap vertexAttributeGltfLocationMap = getVertexAttributeMap(gltfPrimitive, tinygltfModel);

			ExtractedVertexAttributeMap extractedVertexAttributeMap;
			if (checkIfInterleaved(vertexAttributeGltfLocationMap))
			{
				extractedVertexAttributeMap = VertexAttributeRepacker::extractVertexAttributesFromInterleaved(vertexAttributeGltfLocationMap, tinygltfModel);
			}
			else
			{
				extractedVertexAttributeMap = VertexAttributeRepacker::extractVertexAttributesFromNonInterleaved(vertexAttributeGltfLocationMap, tinygltfModel);
			}


			bool isSkinned =
				gltfPrimitive.attributes.count("JOINTS_0") > 0 &&
				gltfPrimitive.attributes.count("WEIGHTS_0") > 0;

			// **2. Select the correct engine format**
			const std::vector<Engine::VertexAttributes>& currentEngineFormat =
				isSkinned ? bonedEngineFormat : defualtEngineFormat;



			InterleavedData interleavedData = VertexAttributeRepacker::interleaveAttributes(extractedVertexAttributeMap, currentEngineFormat);

			//upload to vertex buffer and update offsets correctly

			Primitive ourPrimitive;

			ourPrimitive.isSkinned = isSkinned;

			ourPrimitive.vertexOffsetInBuffer = m_bufferManagementSystem.uploadDataToVertexBuffer(interleavedData.interleavedData.data(), interleavedData.interleavedData.size());
			ourPrimitive.vertexCount = extractedVertexAttributeMap.vertexAttributeMap.at(VertexAttributes::POSITION).totalNumOfItems;
			
			ourPrimitive.stride = interleavedData.stride;

			//check if is indexed 
			if (gltfPrimitive.indices >= 0)
			{
				ourPrimitive.isIndexed = true;

				std::vector<tinygltf::Accessor>& accessors = tinygltfModel.accessors;
				std::vector<tinygltf::BufferView>& bufferViews = tinygltfModel.bufferViews;
				ourPrimitive.indexCount = accessors[gltfPrimitive.indices].count;

				size_t srcIndexOfIndexData =  bufferViews[accessors[gltfPrimitive.indices].bufferView].byteOffset + accessors[gltfPrimitive.indices].byteOffset;
				
				const std::vector<unsigned char>& buffer = tinygltfModel.buffers[0].data;
				//std::cout << "Index Bufferview Index: " << accessors[gltfPrimitive.indices].bufferView<<" accessor index :" << gltfPrimitive.indices <<" accessor offset : "<< accessors[gltfPrimitive.indices].byteOffset << std::endl;
				ourPrimitive.indexOffsetInBuffer = m_bufferManagementSystem.uploadDataToIndexBuffer(reinterpret_cast<const void*>(&buffer[srcIndexOfIndexData]), accessors[gltfPrimitive.indices].count * VertexAttributeRepacker::m_componentTypeToByteSizeMap.at(accessors[gltfPrimitive.indices].componentType));
				ourPrimitive.indexFormat = (accessors[gltfPrimitive.indices].componentType == 5125) ? IndexFormat::UINT16 : IndexFormat::UINT8;
			}
			

			ourPrimitive.materialIndex = gltfPrimitive.material;

			primitivesList.push_back(ourPrimitive);
		}




		return primitivesList;
	}






	std::vector<Mesh> GLTFFlatParser::getMeshList(tinygltf::Model& tinygltfModel)
	{
	std::vector<Mesh> m_meshList;
		for(auto& tinygltfMesh : tinygltfModel.meshes)
		{
			std::vector<Primitive> primitives = getPrimitives(tinygltfModel, tinygltfMesh);
			Mesh ourMesh;
			ourMesh.primitives = primitives;
			m_meshList.push_back(ourMesh);
		}


		return m_meshList;




	}







	Model GLTFFlatParser::parse(tinygltf::Model& tinygltfModel)
	{

		size_t rootNodeIndex = 0;

		for (auto scene : tinygltfModel.scenes)
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



		//const tinygltf::Node rootNode = (tinygltfModel.nodes.size() > 1) ? tinygltfModel.nodes[1] : tinygltfModel.nodes[0];


		Model model;

		model.rootNodeIndex = rootNodeIndex;

		m_imageList = extractImages(tinygltfModel);
		m_textureList = extractTextures(tinygltfModel);


		model.materials = extractMaterial(tinygltfModel);
		model.meshes = getMeshList(tinygltfModel);
		model.nodes = getNodeList(tinygltfModel);



		ExtractedAnimationData extractedAnimationData = GltfAnimationExtractor::extractAnimation(tinygltfModel);

		model.animationData = extractedAnimationData.animationData;
		model.animations = extractedAnimationData.animationsMap;

		BoneAnimationData boneAnimationData;

		boneAnimationData = getBoneAnimationData(tinygltfModel);
		if (boneAnimationData.isSkinned)
		{
			model.inverseBindMatrices = boneAnimationData.inverseBindMatrices;
			model.jointIndices = boneAnimationData.jointIndices;
			model.jointMatrices = boneAnimationData.jointMatrices;

			model.hasSkin = boneAnimationData.isSkinned;
	    }
		


		return model;

	}






	BoneAnimationData GLTFFlatParser::getBoneAnimationData(tinygltf::Model& tinygltfModel)
	{
		std::vector<tinygltf::Accessor>& accessors = tinygltfModel.accessors;
		std::vector<tinygltf::BufferView>& bufferViews = tinygltfModel.bufferViews;

		BoneAnimationData boneAnimationData;

		if (tinygltfModel.skins.size() == 0 /**!(tinygltfModel.skins[0].inverseBindMatrices >= 0)*/)
		{
			boneAnimationData.isSkinned = false;
			return boneAnimationData;
		}
		boneAnimationData.isSkinned = true;

		boneAnimationData.jointIndices = tinygltfModel.skins[0].joints;

		boneAnimationData.inverseBindMatrices.resize(boneAnimationData.jointIndices.size());

		boneAnimationData.jointMatrices.resize(boneAnimationData.jointIndices.size(),glm::mat4(1.0f));

	

		const std::vector<unsigned char>& buffer = tinygltfModel.buffers[0].data;

		const size_t srcIndex = accessors[tinygltfModel.skins[0].inverseBindMatrices].byteOffset + bufferViews[accessors[tinygltfModel.skins[0].inverseBindMatrices].bufferView].byteOffset;

		memcpy(boneAnimationData.inverseBindMatrices.data(), &buffer[srcIndex], bufferViews[accessors[tinygltfModel.skins[0].inverseBindMatrices].bufferView].byteLength);


		return boneAnimationData;
	}

	GLTFFlatParser::GLTFFlatParser(BufferManagementSystem& bufferManagementSystem, GPUTextureManager& gpuTextureManager) : m_bufferManagementSystem(bufferManagementSystem), m_gpuTextureManager(gpuTextureManager)
	{



	}

}