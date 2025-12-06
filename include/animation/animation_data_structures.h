#pragma once
#include <vector>
#include <glm/mat4x4.hpp>
#include "../../external/the-engine/the-engine/ecs/include/common_data_types.h"




namespace Engine
{




	enum class AnimationPathType
	{
		TRANSLATION,
		ROTATION,
		SCALE,
		UNKNOWN
	};

	/*ANIMATION*/
	enum class AnimationInterpolationMode
	{
		LINEAR,
		STEP,
		CUBICSPLINE,
		UNKNOWN
	};



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






	//Need to be in root, common for all
	struct AnimationData
	{

		/** STAND ALONE **/
		//Master struct to hold all animation data
		std::vector<AnimationTimeInput> inputs;
		std::vector<AnimationOutputValue> outputs;



	};


	//Per animation, need a unordered map from name to this
	//
	struct Animation
	{
		std::vector<AnimationChannel> animationChannel;
		std::vector<AnimationSampler> animationSamplers;

		float maxTime = 0.0f;
	};



	


	struct BoneAnimationData
	{
		std::vector<glm::mat4> inverseBindMatrices;
		std::vector<int> jointIndices; //Indice into nodes, we need to map local index to entity id

		//upload this per frame
		std::vector<glm::mat4> jointMatrices;//important pre-reserve size

		bool isSkinned = false;
	};






}
