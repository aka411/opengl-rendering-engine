
#include <unordered_map>
#include "../../include/model-loading/tiny_gltf.h"
#include "../../include/animation/animation_data_structures.h"
#include "../../include/model-loading/gltf_animation_extractor.h"

namespace Engine
{


	AnimationPathType GltfAnimationExtractor::getAnimationType(const std::string& animationPath)
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



	AnimationInterpolationMode GltfAnimationExtractor::getAnimationInterpolationMode(const std::string& tinygltfAnimationInterpolationMode)
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




	 ExtractedAnimation GltfAnimationExtractor::extractAnimation(const tinygltf::Model& tinygltfModel)
	{  //gltf Sampler accessor Index , Engine Sampler Index


		ExtractedAnimation extractedAnimation;




		/*COVERS EVERY ANIMATION*/
		std::unordered_map<int, int> inputIndexMap;
		std::unordered_map<int, int> outputIndexMap;


		std::vector< AnimationTimeInput> inputList;
		std::vector< AnimationOutputValue> outputList;

		if (tinygltfModel.animations.size() != 0)
		{
			extractedAnimation.hasAnimations = true;
		}

		for (auto& animation : tinygltfModel.animations)
		{
			extractedAnimation.animationsMap[animation.name] = Animation{}; //create empty animation entry
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
						const tinygltf::Accessor& inputAccessor = tinygltfModel.accessors[gltfInputIndex];
						const tinygltf::BufferView& inputBufferView = tinygltfModel.bufferViews[inputAccessor.bufferView];
						const tinygltf::Buffer& inputBuffer = tinygltfModel.buffers[inputBufferView.buffer];

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


						if (inputKeyframes.back() > maxTime)
						{
							maxTime = inputKeyframes.back();
						}


					}
					else
					{
						const int engineInputIndex = inputIndexMap.find(gltfInputIndex)->second;
						animationSampler.inputIndex = engineInputIndex;

						//update max time
						if (inputList[engineInputIndex].inputKeyframes.back() > maxTime)
						{
							maxTime = inputList[engineInputIndex].inputKeyframes.back();
						}
					}





					//For output
					const int gltfOutputIndex = animation.samplers[gltfSamplerIndex].output;
					if (outputIndexMap.find(gltfOutputIndex) == outputIndexMap.end())
					{
						//need to create new output
						const tinygltf::Accessor& outputAccessor = tinygltfModel.accessors[gltfOutputIndex];
						const tinygltf::BufferView& outputBufferView = tinygltfModel.bufferViews[outputAccessor.bufferView];
						const tinygltf::Buffer& outputBuffer = tinygltfModel.buffers[outputBufferView.buffer];
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

			extractedAnimation.animationsMap[animation.name] = engineAnimation;

		}


		extractedAnimation.animationData.inputs= inputList;
		extractedAnimation.animationData.outputs = outputList;
		
		return extractedAnimation;
	}










	BoneAnimationData GltfAnimationExtractor::getBoneAnimationData(const tinygltf::Model& tinygltfModel)
	{
		const std::vector<tinygltf::Accessor>& accessors = tinygltfModel.accessors;
		const std::vector<tinygltf::BufferView>& bufferViews = tinygltfModel.bufferViews;

		BoneAnimationData boneAnimationData;

		if (tinygltfModel.skins.size() == 0 /**!(tinygltfModel.skins[0].inverseBindMatrices >= 0)*/)
		{
			boneAnimationData.isSkinned = false;
			return boneAnimationData;
		}
		boneAnimationData.isSkinned = true;

		boneAnimationData.jointIndices = tinygltfModel.skins[0].joints;

		boneAnimationData.inverseBindMatrices.resize(boneAnimationData.jointIndices.size());

		boneAnimationData.jointMatrices.resize(boneAnimationData.jointIndices.size(), glm::mat4(1.0f));



		const std::vector<unsigned char>& buffer = tinygltfModel.buffers[0].data;

		const size_t srcIndex = accessors[tinygltfModel.skins[0].inverseBindMatrices].byteOffset + bufferViews[accessors[tinygltfModel.skins[0].inverseBindMatrices].bufferView].byteOffset;

		memcpy(boneAnimationData.inverseBindMatrices.data(), &buffer[srcIndex], bufferViews[accessors[tinygltfModel.skins[0].inverseBindMatrices].bufferView].byteLength);


		return boneAnimationData;
	}





}