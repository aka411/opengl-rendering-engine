#pragma once
#include "../components.h"


class SkeletalAnimationSolver
{

private:



public:
	SkeletalAnimationSolver();

	static void animate(const BoneAnimationCPUComponent& boneAnimationCPUComponent, std::vector<glm::mat4>& jointMatrixSet, std::vector<EngineTransformationComponent*> animationTargetTransformationComponents);
	

};