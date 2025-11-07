#pragma once
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>
#include "gltf_flat_parser.h"
struct LinearData
{
	int currentFrameIndex;
	int nextFrameIndex;
};
class SkinAnimationSystem
{
private:

	
	int step(const std::vector<float>& input, const float currentTime);
	LinearData linear(const std::vector<float>& input, const float currentTime);

	glm::vec3 animateTranslate(Engine::AnimationInterpolationMode animationinterpolationMode, const float totalDuration, const std::vector<float>& input, const std::vector<float>& output, float currentTime);

	glm::quat animateRotate(Engine::AnimationInterpolationMode animationinterpolationMode, const float totalDuration, const std::vector<float>& input, const std::vector<float>& output, float currentTime);

public:

	SkinAnimationSystem();

	void animate(float deltaTime, Engine::Model& model, std::string animationName);

	~SkinAnimationSystem();



};