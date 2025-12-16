

#include <cassert>
#include <glm/ext/matrix_float4x4.hpp>
#include "../../include/animation/skeletal_animation_solver.h"


SkeletalAnimationSolver::SkeletalAnimationSolver()
{

}


void SkeletalAnimationSolver::animate(const BoneAnimationCPUComponent& boneAnimationCPUComponent, std::vector<glm::mat4>& jointMatrixSet, std::vector<EngineTransformationComponent*> animationTargetTransformationComponents)
{



	const std::vector<glm::mat4>& inverseBindMatrices = boneAnimationCPUComponent.inverseBindMatrices;
	const std::vector<int>& jointIndices = boneAnimationCPUComponent.jointIndices; //Indice into nodes, we need to map local index to entity id


	std::vector<glm::mat4>& jointMatrices = jointMatrixSet;

	assert(inverseBindMatrices.size() == jointIndices.size() && jointIndices.size() == jointMatrices.size());

	for (int i = 0; i < jointMatrices.size(); ++i)
	{
		const int jointIndex = jointIndices[i];

		glm::mat4& nodeWorldMatrix = animationTargetTransformationComponents[jointIndex]->worldTransformMatrix;

		const glm::mat4& bindInverseMatrix = inverseBindMatrices[i];


		jointMatrices[i] = nodeWorldMatrix * bindInverseMatrix;

	}


}
