#include "../include/skin_animation_system.h"
#include "../include/gltf_flat_parser.h"
#include <glm/gtc/type_ptr.hpp>



SkinAnimationSystem::SkinAnimationSystem()
{


}




SkinAnimationSystem::~SkinAnimationSystem()
{
}




int SkinAnimationSystem::step(const std::vector<float>& input, const float currentTime)
{
	auto& it = std::upper_bound(input.begin(), input.end(), currentTime);


	if (it == input.end())
	{

		return input.size() - 1;
	}

	int index = std::distance(input.begin(), it);

	int finalIndex = index - 1;


	if (finalIndex < 0)
	{
		return 0;
	}

	return finalIndex;

}

LinearData SkinAnimationSystem::linear(const std::vector<float>& input, const float currentTime)
{
	auto& it = std::upper_bound(input.begin(), input.end(), currentTime);

	if (it == input.end()) {

		LinearData data{};
		data.currentFrameIndex = input.size() - 1; 
		data.nextFrameIndex = 0; 
		return data;

	}
	
	int index = std::distance(input.begin(), it);

	int current = index - 1;

	// Safety check: if currentTime is 0, index is 0, current becomes -1. 
	// It should wrap to the last frame.
	if (current < 0)
	{
		current = input.size() - 1;
	}



	LinearData data{};

	data.currentFrameIndex = current;

	
	data.nextFrameIndex = (current + 1 >= input.size()) ? 0 : current + 1;

	return data;

}


glm::vec3 SkinAnimationSystem::animateTranslate(Engine::AnimationInterpolationMode animationinterpolationMode, const float totalDuration, const std::vector<float>& input,const std::vector<float>& output,float currentTime )
{
	LinearData data = linear(input, currentTime);

	float startTime = input[data.currentFrameIndex];
	float endTime = input[data.nextFrameIndex];


	if (data.nextFrameIndex == 0 && data.currentFrameIndex == input.size() - 1)
	{
		
		endTime = totalDuration;
	}


	const float duration = endTime - startTime;
	const float factor = (duration > 0.0f) ? (currentTime - startTime) / duration : 0.0f;
	

	switch (animationinterpolationMode)
	{
	case Engine::AnimationInterpolationMode::LINEAR:
	{
		glm::vec3 start = glm::make_vec3(&output[data.currentFrameIndex * 3]);
		glm::vec3 end = glm::make_vec3(&output[data.nextFrameIndex * 3]);
		glm::vec3 interpolated = glm::mix(start, end, factor);
		return interpolated;
	}
		break;

	case Engine::AnimationInterpolationMode::STEP:
	{
		int index = step(input, currentTime);
		glm::vec3 finalTranslation = glm::make_vec3(&output[index * 3]);
		return finalTranslation;
	}
		break;

	default:
		return glm::vec3(0.0f);
		break;
	}





}


glm::quat SkinAnimationSystem::animateRotate(Engine::AnimationInterpolationMode animationinterpolationMode,const float totalDuration, const std::vector<float>& input, const std::vector<float>& output, float currentTime)
{


	LinearData data = linear(input, currentTime);

	float startTime = input[data.currentFrameIndex];
	float endTime = input[data.nextFrameIndex];


	if (data.nextFrameIndex == 0 && data.currentFrameIndex == input.size() - 1)
	{

		endTime = totalDuration;
	}


	const float duration = endTime - startTime;
	const float factor = (duration > 0.0f) ? (currentTime - startTime) / duration : 0.0f;




	switch (animationinterpolationMode)
	{
	case Engine::AnimationInterpolationMode::LINEAR:
	{
		glm::quat startRotation = glm::make_quat(&output[data.currentFrameIndex * 4]);
		glm::quat endRotation = glm::make_quat(&output[data.nextFrameIndex * 4]);
		glm::quat interpolatedRotation = glm::slerp(startRotation, endRotation, factor);
		return interpolatedRotation;
	}
		break;

	case Engine::AnimationInterpolationMode::STEP:
	{
		int index = step(input, currentTime);
		glm::quat finalRotation = glm::make_quat(&output[index * 4]);
		return finalRotation;
	}
		break;

	default:
		return glm::quat(1.0f,0.0f,0.0f,0.0f);
		break;
	}



}




void SkinAnimationSystem::animate(float deltaTime, Engine::Model& model, std::string animationName)
{

	// this task is highly parallizable



	auto it = model.animations.find(animationName);
	if (it == model.animations.end()) return;

	auto& animation = it->second;
	float totalDuration = animation.maxTime ;

	if (totalDuration <= 0.0f) return; // No animation to process

	float animationDuration = 0.0f;
	Engine::Model::AnimationState* state = nullptr;
	if (model.playingAnimations.find(animationName) != model.playingAnimations.end())
	{
		state = &model.playingAnimations.find(animationName)->second;
		
		
	}
	else
	{
		Engine::Model::AnimationState  animationState;
		model.playingAnimations[animationName] = animationState;
		state = &model.playingAnimations[animationName];
	}





	state->currentTime += deltaTime;

	
	state->currentTime = std::fmod(state->currentTime, animation.maxTime);


	float currentAnimTime = state->currentTime;


	
	
	for (auto& channel : animation.animationChannel)
	{


		


		Engine::AnimationSampler& sampler = animation.animationSamplers[channel.samplerIndex];

		const int inputIndex = sampler.inputIndex;	
		const auto& inputKeyframes = model.animationData.inputs[inputIndex].inputKeyframes;
		size_t keyframeCount = inputKeyframes.size();

		const int outputIndex = sampler.outputIndex;
		std::vector<float>& outputValues = model.animationData.outputs[outputIndex].outputValues;
	
		switch (channel.pathType)
		{
		case Engine::AnimationPathType::TRANSLATION:
		{

			model.nodes[channel.targetNodeIndex].transformation.position = animateTranslate(sampler.interpolationMode,totalDuration,inputKeyframes, outputValues, currentAnimTime);
		}
		break;
		case Engine::AnimationPathType::ROTATION:
		{

			model.nodes[channel.targetNodeIndex].transformation.rotation = animateRotate(sampler.interpolationMode, totalDuration, inputKeyframes, outputValues, currentAnimTime);
		}
		break;
		case Engine::AnimationPathType::SCALE:
		{
	
			model.nodes[channel.targetNodeIndex].transformation.scale = animateTranslate(sampler.interpolationMode, totalDuration, inputKeyframes, outputValues, currentAnimTime);
		}
		break;
		default:
			break;

		}


		
	}

}
