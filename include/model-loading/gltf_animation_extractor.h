#pragma once
#include <unordered_map>
#include "tiny_gltf.h"

namespace Engine
{

	struct ExtractedAnimationData
	{
		AnimationData animationData;
		std::unordered_map<std::string, Animation> animationsMap;
	};

	class GltfAnimationExtractor
	{
	private:

		static AnimationPathType getAnimationType(std::string& animationPath);

		static AnimationInterpolationMode getAnimationInterpolationMode(std::string& tinygltfAnimationInterpolationMode);

	public:



		static ExtractedAnimationData extractAnimation(const tinygltf::Model& tinygltfModel);

		static BoneAnimationData getBoneAnimationData(const tinygltf::Model& tinygltfModel);


	};

}