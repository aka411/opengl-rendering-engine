#pragma once
#include <unordered_map>
#include "tiny_gltf.h"
#include "../animation/animation_data_structures.h"
#include <string>



namespace Engine
{

	struct ExtractedAnimation
	{
		AnimationData animationData;
		std::unordered_map<std::string, Animation> animationsMap;
		bool hasAnimations = false;
	};




	class GltfAnimationExtractor
	{
	private:

		static AnimationPathType getAnimationType(const std::string& animationPath);

		static AnimationInterpolationMode getAnimationInterpolationMode(const std::string& tinygltfAnimationInterpolationMode);

	public:



		static ExtractedAnimation extractAnimation(const tinygltf::Model& tinygltfModel);

		static BoneAnimationData getBoneAnimationData(const tinygltf::Model& tinygltfModel);


	};

}