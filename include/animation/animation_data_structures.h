#pragma once
#include <vector>
#include <glm/ext/quaternion_float.hpp>



namespace Engine
{


	/*ANIMATION*/
	enum class AnimationInterpolationMode
	{
		LINEAR,
		STEP,
		CUBICSPLINE,
		UNKNOWN
	};

	enum class AnimationPathType
	{
		TRANSLATION,
		ROTATION,
		SCALE,
		UNKNOWN
	};



	/**
struct AnimationTranslationSampler
{
	AnimationInterpolationMode interpolationMode;
	std::vector<float> inputKeyframes; //time
	std::vector<glm::vec3> translations; //can be vec3 or vec4 depending on path type
};

struct AnimationRotationSampler
{
	AnimationInterpolationMode interpolationMode;
	std::vector<float> inputKeyframes; //time
	std::vector<glm::quat> rotations; //can be vec3 or vec4 depending on path type
};

struct AnimationScaleSampler
{
	AnimationInterpolationMode interpolationMode;
	std::vector<float> inputKeyframes; //time
	std::vector<glm::vec3> scales; //can be vec3 or vec4 depending on path type
};
*/



	struct AnimationTimeInput
	{
		std::vector<float> inputKeyframes; //time
	};
	struct AnimationOutputValue
	{
		std::vector<float> outputValues;
	};

	struct AnimationSampler
	{
		AnimationInterpolationMode interpolationMode = AnimationInterpolationMode::UNKNOWN;
		int inputIndex = -1;
		int outputIndex = -1;
	};



	struct AnimationChannel
	{
		int targetNodeIndex;
		AnimationPathType pathType;
		int samplerIndex; //index to sampler list
	};


	struct AnimationData
	{
		/** STAND ALONE **/
		//Master struct to hold all animation data
		std::vector<AnimationTimeInput> inputs;
		std::vector<AnimationOutputValue> outputs;

	};

	struct Animation
	{
		std::vector<AnimationChannel> animationChannel;
		std::vector<AnimationSampler> animationSamplers;

		float maxTime = 0.0f;
	};



	//struct 


	struct BoneAnimationData
	{
		std::vector<glm::mat4> inverseBindMatrices;
		std::vector<int> jointIndices; //

		std::vector<glm::mat4> jointMatrices;//important prereserve size

		bool isSkinned = false;
	};






}
