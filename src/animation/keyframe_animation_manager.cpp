#include "../../include/animation/keyframe_animation_manager.h"






AnimationCPUComponent& KeyframeAnimationManager::getAnimationCPUComponentFromId(KeyframeAnimationId keyframeAnimationId)
{

	assert(m_animationCPUComponents.size() < keyframeAnimationId);

	return m_animationCPUComponents[keyframeAnimationId];


}


KeyframeAnimationId KeyframeAnimationManager::storeAnimationCPUComponent(AnimationCPUComponent&& animationCPUComponent)
{
	//NOTE : consider adding a stack to keep track of free slots and also a method to delete aniamtions


	const KeyframeAnimationId keyframeAnimationId = m_animationCPUComponents.size();
	m_animationCPUComponents.push_back(std::move(animationCPUComponent));

	return keyframeAnimationId;
}