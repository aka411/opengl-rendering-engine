#include "../include/bone_animation_system.h"

void BoneAnimationSystem::animate(Engine::Model& engineModel) const
{

	if (!engineModel.currentlyAnimated) return;
	assert(engineModel.jointMatrices.size() == engineModel.jointIndices.size());
	for (int i = 0; i < engineModel.jointIndices.size(); ++i)
	{
		const int jointIndex = engineModel.jointIndices[i];
	
		glm::mat4& nodeWorldMatrix = engineModel.nodes[jointIndex].transformation.worldTransformMatrix;

		glm::mat4& bindInversematrix = engineModel.inverseBindMatrices[i];


		engineModel.jointMatrices[i] = nodeWorldMatrix * bindInversematrix;

	}


}
