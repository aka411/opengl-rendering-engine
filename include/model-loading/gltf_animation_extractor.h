#pragma once
#include <unordered_map>
#include "engine_data_structures.h"
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



		static ExtractedAnimationData extractAnimation(tinygltf::Model& tinygltfModel);

	};

}