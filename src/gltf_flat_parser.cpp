#include "../include/gltf_flat_parser.h"
#define TINYGLTF_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "../include/tiny_gltf.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

namespace Engine
{


	glm::mat4 GLTFFlatParser::convertDoubleToFloatMatrix4(const std::vector<double>& tinygltfMatrix4Data)
	{
		// tinygltf matrices are typically stored in column-major order (like OpenGL/glm)
		// and are 16 doubles long.

		glm::mat4 result{};
		//assert(result.length() == tinygltfMatrix4Data.size());
		for (int i = 0; i < tinygltfMatrix4Data.size(); ++i)
		{
			// Cast the double to float for storage in glm::mat4
			result[i / 4][i % 4] = static_cast<float>(tinygltfMatrix4Data[i]);


		}
		return result;
	}



	glm::vec4 GLTFFlatParser::convertDoubleToFloatVec4(const std::vector<double>& tinygltfVec4Data)
	{

		glm::vec4 result{};
		//assert(result.length() == tinygltfVec4Data.size());
		for (int i = 0; i < tinygltfVec4Data.size(); ++i)
		{
			// Cast the double to float for storage in glm::mat4
			result[i] = static_cast<float>(tinygltfVec4Data[i]);
		}
		return result;
	}

	glm::vec3 GLTFFlatParser::convertDoubleToFloatVec3(const std::vector<double>& tinygltfVec3Data)
	{

		glm::vec3 result{};
		//assert(result.length() == tinygltfVec3Data.size());
		for (int i = 0; i < tinygltfVec3Data.size(); ++i)
		{
			// Cast the double to float for storage in glm::mat4
			result[i] = static_cast<float>(tinygltfVec3Data[i]);
		}
		return result;
	}

	glm::quat GLTFFlatParser::convertDoubleToFloatQuat(const std::vector<double>& tinygltfQuatData)
	{
		glm::quat result{};
		//assert(result.length() == tinygltfQuatData.size());
		for (int i = 0; i < tinygltfQuatData.size(); ++i)
		{
			// Cast the double to float for storage in glm::mat4
			result[i] = static_cast<float>(tinygltfQuatData[i]);
		}
		return result;
	}





	std::vector<GLuint> GLTFFlatParser::parseImages(tinygltf::Model& tinygltfModel)
	{
		std::vector<GLuint> imageList;
		for (auto& image : tinygltfModel.images)
		{
			imageList.push_back(m_gpuTextureManager.createNewTexture(image.width, image.height, image.component, image.image.data()));
		}

		return imageList;
	}

	std::vector<GLuint> GLTFFlatParser::parseTextures(tinygltf::Model& tinygltfModel)
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
			glm::vec4 emmissiveFactor = convertDoubleToFloatVec4(material.emissiveFactor);


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

	AnimationPathType GLTFFlatParser::getAnimationType(std::string& animationPath)
	{

		if (animationPath == "translation")
		{
			return AnimationPathType::TRANSLATION;
		}
		if (animationPath == "scale")
		{
			return AnimationPathType::SCALE;
		}
		if (animationPath == "rotation")
		{
			return AnimationPathType::ROTATION;
		}

		return AnimationPathType::UNKNOWN;
	}



	AnimationInterpolationMode GLTFFlatParser::getAnimationInterpolationMode(std::string& tinygltfAnimationInterpolationMode)
	{

		if (tinygltfAnimationInterpolationMode == "LINEAR")
		{
			return AnimationInterpolationMode::LINEAR;
		}
		if (tinygltfAnimationInterpolationMode == "STEP")
		{
			return AnimationInterpolationMode::STEP;
		}
		if (tinygltfAnimationInterpolationMode == "CUBICSPLINE")
		{
			return AnimationInterpolationMode::CUBICSPLINE;
		}

		return AnimationInterpolationMode::UNKNOWN;
	}




	void GLTFFlatParser::getAnimation(tinygltf::Model& tinygltfModel)
	{  //gltf Sampler accessor Index , Engine Sampler Index


		AnimationData animationData;
		/*COVERS EVERY ANIMATION*/
		std::unordered_map<int, int> inputIndexMap;
		std::unordered_map<int, int> outputIndexMap;


		std::vector< AnimationTimeInput> inputList;
		std::vector< AnimationOutputValue> outputList;



		for (auto& animation : tinygltfModel.animations)
		{
			m_animationMap[animation.name] = Animation{}; //create empty animation entry
			float maxTime = 0.0f;

			std::unordered_map<int, int> samplerIndexMap; /*PER ANIMATION MAP*/

	


			std::vector<AnimationChannel> animationChannelList;
			animationChannelList.reserve(animation.channels.size());

			//CREATE SAMPLER VECTOR
			std::vector<AnimationSampler> animationSamplerList;
			animationSamplerList.reserve(animation.samplers.size());

			for (auto& channel : animation.channels)
			{

				AnimationChannel animationChannel;

				animationChannel.targetNodeIndex = channel.target_node;
				animationChannel.pathType = getAnimationType(channel.target_path);

				assert(animationChannel.pathType != AnimationPathType::UNKNOWN);

				assert(channel.sampler < animation.samplers.size());

				const int gltfSamplerIndex = channel.sampler;
				//using map to find 
				if (samplerIndexMap.find(gltfSamplerIndex) == samplerIndexMap.end())
				{
					//need new sampler
					AnimationSampler animationSampler;
					animationSampler.interpolationMode = getAnimationInterpolationMode(animation.samplers[gltfSamplerIndex].interpolation);





					//For input
					const int gltfInputIndex = animation.samplers[gltfSamplerIndex].input;
					if (inputIndexMap.find(gltfInputIndex) == inputIndexMap.end())
					{
						//need to create new input
						tinygltf::Accessor& inputAccessor = tinygltfModel.accessors[gltfInputIndex];
						tinygltf::BufferView& inputBufferView = tinygltfModel.bufferViews[inputAccessor.bufferView];
						tinygltf::Buffer& inputBuffer = tinygltfModel.buffers[inputBufferView.buffer];
						size_t inputByteOffset = inputBufferView.byteOffset + inputAccessor.byteOffset;
						size_t inputCount = inputAccessor.count;

						std::vector<float> inputKeyframes;
						inputKeyframes.resize(inputCount);

						memcpy(inputKeyframes.data(), inputBuffer.data.data() + inputByteOffset, inputCount * sizeof(float));

						AnimationTimeInput animationTimeInput;
						animationTimeInput.inputKeyframes = inputKeyframes;

						animationSampler.inputIndex = static_cast<int>(inputList.size());//new input index

						inputList.push_back(animationTimeInput);

						inputIndexMap[gltfInputIndex] = animationSampler.inputIndex;


						if(inputKeyframes.back() > maxTime)
						{
							maxTime = inputKeyframes.back();
						}


					}
					else
					{
						const int engineInputIndex = inputIndexMap.find(gltfInputIndex)->second;
						animationSampler.inputIndex = engineInputIndex;

						//update max time
						if(inputList[engineInputIndex].inputKeyframes.back() > maxTime)
						{
							maxTime = inputList[engineInputIndex].inputKeyframes.back();
						}
					}





					//For output
					const int gltfOutputIndex = animation.samplers[gltfSamplerIndex].output;
					if (outputIndexMap.find(gltfOutputIndex) == outputIndexMap.end())
					{
						//need to create new output
						tinygltf::Accessor& outputAccessor = tinygltfModel.accessors[gltfOutputIndex];
						tinygltf::BufferView& outputBufferView = tinygltfModel.bufferViews[outputAccessor.bufferView];
						tinygltf::Buffer& outputBuffer = tinygltfModel.buffers[outputBufferView.buffer];
						size_t outputByteOffset = outputBufferView.byteOffset + outputAccessor.byteOffset;
						size_t outputCount = outputAccessor.count;
						size_t typeCount = 0;
						if (animationChannel.pathType == AnimationPathType::TRANSLATION || animationChannel.pathType == AnimationPathType::SCALE)
						{
							typeCount = 3;
						}
						else if (animationChannel.pathType == AnimationPathType::ROTATION)
						{
							typeCount = 4;
						}

						std::vector<float> outputValues;
						outputValues.resize(outputCount * typeCount);

						memcpy(outputValues.data(), outputBuffer.data.data() + outputByteOffset, outputCount * typeCount * sizeof(float));

						AnimationOutputValue animationOutputValue;
						animationOutputValue.outputValues = outputValues;

						animationSampler.outputIndex = static_cast<int>(outputList.size());//new output index

						outputList.push_back(animationOutputValue);

						outputIndexMap[gltfOutputIndex] = animationSampler.outputIndex;



					}
					else
					{
						const int engineOutputIndex = outputIndexMap.find(gltfOutputIndex)->second;
						animationSampler.outputIndex = engineOutputIndex;
				
					}


					animationChannel.samplerIndex = static_cast<int>(animationSamplerList.size());//new sampler index
					samplerIndexMap[gltfSamplerIndex] = animationChannel.samplerIndex;
					animationSamplerList.push_back(animationSampler);

				}
				else
				{
					const int engineSamplerIndex = samplerIndexMap.find(gltfSamplerIndex)->second;
					animationChannel.samplerIndex = engineSamplerIndex;
				}


				animationChannelList.push_back(animationChannel);
				
				
				
			}

			Animation engineAnimation;
			engineAnimation.animationChannel = animationChannelList;
			engineAnimation.animationSamplers = animationSamplerList;
			engineAnimation.maxTime = maxTime;

			m_animationMap[animation.name] = engineAnimation;

		}


		animationData.inputs = inputList;
		animationData.outputs = outputList;
		m_animationData = animationData;

	}






	Transformation GLTFFlatParser::getTransformation(tinygltf::Node node)
	{

		if (!node.matrix.empty())
		{
			Transformation transformation;
			glm::mat4 matrix = convertDoubleToFloatMatrix4(node.matrix);

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

			return transformation;
		}

		Transformation transformation;

		if (!node.translation.empty())
		{
			transformation.position = convertDoubleToFloatVec3(node.translation);
		}

		if (!node.scale.empty())
		{
			transformation.scale = convertDoubleToFloatVec3(node.scale);
		}

		if (!node.rotation.empty())
		{
			transformation.rotation = convertDoubleToFloatQuat(node.rotation);
		}



		return transformation;

	}



	std::vector<Primitive> GLTFFlatParser::getPrimitives(tinygltf::Model& tinygltfModel, tinygltf::Mesh& mesh, size_t globalVertexBufferGlobalOffset)
	{

		std::vector<tinygltf::Accessor>& accessors = tinygltfModel.accessors;
		std::vector<tinygltf::BufferView>& bufferViews = tinygltfModel.bufferViews;

		std::vector<Primitive> primitives;
		primitives.reserve(mesh.primitives.size());
		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			//get position accessor
			if (primitive.attributes.find("POSITION") == primitive.attributes.end())
			{
				//no position attribute found skip this primitive
				continue;
			}
			const tinygltf::Accessor& positionAccessor = accessors[primitive.attributes.find("POSITION")->second];

			const tinygltf::BufferView& positionBufferView = bufferViews[positionAccessor.bufferView];
			const size_t positionBufferOffsetInBytes = positionBufferView.byteOffset + positionAccessor.byteOffset + globalVertexBufferGlobalOffset;

			//get normal accessor
			if (primitive.attributes.find("NORMAL") == primitive.attributes.end())
			{
				//no normal attribute found skip this primitive
				continue;
			}
			const tinygltf::Accessor& normalAccessor = accessors[primitive.attributes.find("NORMAL")->second];
			const tinygltf::BufferView& normalBufferView = bufferViews[normalAccessor.bufferView];
			const size_t normalBufferOffsetInBytes = normalBufferView.byteOffset + normalAccessor.byteOffset + globalVertexBufferGlobalOffset;

			//get texcoord accessor
			if (primitive.attributes.find("TEXCOORD_0") == primitive.attributes.end())
			{
				//no normal attribute found skip this primitive
				continue;
			}
			const tinygltf::Accessor& texCoordAccessor = accessors[primitive.attributes.find("TEXCOORD_0")->second];
			const tinygltf::BufferView& texCoordBufferView = bufferViews[texCoordAccessor.bufferView];
			const size_t texCoordBufferOffsetInBytes = texCoordBufferView.byteOffset + texCoordAccessor.byteOffset + globalVertexBufferGlobalOffset;




			Primitive ourPrimitive;
			if (positionAccessor.bufferView == normalAccessor.bufferView == texCoordAccessor.bufferView)
			{
				ourPrimitive.vertexInfo.vertexPackingFormat = VertexPackingFormat::INTERLEAVED;
				ourPrimitive.postionStride = positionBufferView.byteStride;
				ourPrimitive.normalStride = normalBufferView.byteStride;
				ourPrimitive.texCoordStride = texCoordBufferView.byteStride;


			}
			else
			{
				ourPrimitive.vertexInfo.vertexPackingFormat = VertexPackingFormat::NON_INTERLEAVED;
			}

			if (primitive.indices >= 0)
			{
				ourPrimitive.isIndexed = true;


				ourPrimitive.indexCount = accessors[primitive.indices].count;
				ourPrimitive.indexOffsetInBuffer = bufferViews[accessors[primitive.indices].bufferView].byteOffset + accessors[primitive.indices].byteOffset + globalVertexBufferGlobalOffset;
				ourPrimitive.indexFormat = (accessors[primitive.indices].componentType == 5125) ? IndexFormat::UNSIGNED_INT : IndexFormat::UNSIGNED_SHORT;

			}
			else
			{
				ourPrimitive.isIndexed = false;
			}




			ourPrimitive.positionOffsetInBuffer = positionBufferOffsetInBytes;
			ourPrimitive.normalOffsetInBuffer = normalBufferOffsetInBytes;
			ourPrimitive.texCoordOffsetInBuffer = texCoordBufferOffsetInBytes;
			ourPrimitive.vertexCount = positionAccessor.count;



			//material handling
			ourPrimitive.materialIndex = primitive.material;

			primitives.push_back(ourPrimitive);



		}




		return primitives;
	}

	std::vector<Mesh> GLTFFlatParser::getMeshList(tinygltf::Model& tinygltfModel)
	{
	std::vector<Mesh> m_meshList;
		for(auto& tinygltfMesh : tinygltfModel.meshes)
		{
			std::vector<Primitive> primitives = getPrimitives(tinygltfModel, tinygltfMesh, m_globalVertexBufferGlobalOffset);
			Mesh ourMesh;
			ourMesh.primitives = primitives;
			m_meshList.push_back(ourMesh);
		}


		return m_meshList;
	}







	Model GLTFFlatParser::parse(tinygltf::Model& tinygltfModel)
	{

		 size_t rootNodeIndex = 0;
		
		for(auto scene : tinygltfModel.scenes)
		{
			for(auto nodeIndex : scene.nodes)
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

		m_imageList = parseImages(tinygltfModel);
		m_textureList = parseTextures(tinygltfModel);	
		m_materialList = extractMaterial(tinygltfModel);

		model.materials = m_materialList;
		model.meshes = getMeshList(tinygltfModel);
		model.nodes = getNodeList(tinygltfModel);
		getAnimation(tinygltfModel);
		
		model.animationData = m_animationData;
		model.animations = m_animationMap;


		std::vector<tinygltf::Buffer> buffers = tinygltfModel.buffers;

		 m_globalVertexBufferGlobalOffset = m_bufferManagementSystem.uploadDataToVertexBuffer(buffers[0].data.data(), buffers[0].data.size());




		return model;

	}






	GLTFFlatParser::GLTFFlatParser(BufferManagementSystem& bufferManagementSystem, GPUTextureManager& gpuTextureManager) : m_bufferManagementSystem(bufferManagementSystem), m_gpuTextureManager(gpuTextureManager)
	{



	}

}