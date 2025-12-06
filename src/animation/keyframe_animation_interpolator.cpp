

#include  <algorithm>
#include "../../include/animation/keyframe_animation_interpolator.h"
#include <glm/gtc/type_ptr.hpp>


KeyframeAnimationInterpolator::KeyframeAnimationInterpolator()
{


}








int KeyframeAnimationInterpolator::step(const std::vector<float>& input, const float currentTime)
{
	const auto& it = std::upper_bound(input.begin(), input.end(), currentTime);


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



KeyframeAnimationInterpolator::LinearData KeyframeAnimationInterpolator::linear(const std::vector<float>& input, const float currentTime)
{
	const auto& it = std::upper_bound(input.begin(), input.end(), currentTime);

	if (it == input.end())
	{

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


glm::vec3 KeyframeAnimationInterpolator::animateTranslate(Engine::AnimationInterpolationMode animationinterpolationMode, const float totalDuration, const std::vector<float>& input, const std::vector<float>& output, float currentTime)
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


glm::quat KeyframeAnimationInterpolator::animateRotate(Engine::AnimationInterpolationMode animationinterpolationMode, const float totalDuration, const std::vector<float>& input, const std::vector<float>& output, float currentTime)
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
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		break;
	}



}




void KeyframeAnimationInterpolator::animate(AnimationStateComponent& animationStateComponent, const AnimationCPUComponent& animationCPUComponent, std::vector<EngineTransformationComponent*>& engineTransformationComponents,const float deltaTime)
{

	// this task is highly parallizable

	if (!animationStateComponent.isPlayingAnimation)
	{ 
		return;
	}



	const auto& it = animationCPUComponent.animationsMap.find(animationStateComponent.currentPlayingAnimationName);

	if (it == animationCPUComponent.animationsMap.end())
	{
		return;
	}



	const auto& animation = it->second;
	const float totalDuration = animation.maxTime;

	//if (totalDuration <= 0.0f) return; // No animation to process

	if (totalDuration < animationStateComponent.timeElapsedInSeconds)
	{
		//Or should i clamp once and then at end set playing to false
		animationStateComponent.isPlayingAnimation = false;
		animationStateComponent.timeElapsedInSeconds = 0;
		return;
	}







	animationStateComponent.timeElapsedInSeconds += deltaTime;
	//what the heck is fmod
	float currentAnimationTime = std::fmod(animationStateComponent.timeElapsedInSeconds, totalDuration);

	if (totalDuration < animationStateComponent.timeElapsedInSeconds)
	{
		
		animationStateComponent.isPlayingAnimation = false;
		animationStateComponent.timeElapsedInSeconds = 0;
	}









	for (const auto& channel : animation.animationChannel)
	{





		const Engine::AnimationSampler& sampler = animation.animationSamplers[channel.samplerIndex];

		const int inputIndex = sampler.inputIndex;
		const auto& inputKeyframes = animationCPUComponent.animationData.inputs[inputIndex].inputKeyframes;
		const size_t keyframeCount = inputKeyframes.size();

		const int outputIndex = sampler.outputIndex;
		const std::vector<float>& outputValues = animationCPUComponent.animationData.outputs[outputIndex].outputValues;




		switch (channel.pathType)
		{
		case Engine::AnimationPathType::TRANSLATION:
		{

			engineTransformationComponents[channel.targetNodeIndex]->position = animateTranslate(sampler.interpolationMode, totalDuration, inputKeyframes, outputValues, currentAnimationTime);
		}
		break;
		case Engine::AnimationPathType::ROTATION:
		{

			engineTransformationComponents[channel.targetNodeIndex]->rotation = animateRotate(sampler.interpolationMode, totalDuration, inputKeyframes, outputValues, currentAnimationTime);
		}
		break;
		case Engine::AnimationPathType::SCALE:
		{

			engineTransformationComponents[channel.targetNodeIndex]->scale = animateTranslate(sampler.interpolationMode, totalDuration, inputKeyframes, outputValues, currentAnimationTime);
		}
		break;
		default:
			break;

		}



	}

}

