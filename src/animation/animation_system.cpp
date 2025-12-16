#include "../../include/animation/animation_system.h"
#include "../../include/animation/keyframe_animation_interpolator.h"



namespace Engine
{

	std::vector<EngineTransformationComponent*> AnimationSystem::getTransfomrationsOfEntities(const std::vector<TheEngine::ECS::EntityId>& entityIds)
	{
		std::vector<EngineTransformationComponent*> transformationComponents;

		transformationComponents.reserve(entityIds.size());


		for (size_t i = 0 ; i < transformationComponents.size(); ++i)
		{
			TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(entityIds[i]);

			EngineTransformationComponent* engineTransformationComponent = entityChunkView.getComponent<EngineTransformationComponent>();

			assert(engineTransformationComponent != nullptr);

			transformationComponents[i] = engineTransformationComponent;
		}

		return transformationComponents;
	}





	AnimationSystem::AnimationSystem(TheEngine::ECS::ECSEngine& ecsEngine,GPUBufferManager& gpuBufferManager)
		:
		m_keyframeAnimationManager(),
		m_skeletalAnimationManager(gpuBufferManager),
		m_ecsEngine(ecsEngine)
	{

	}



	KeyframeAnimationManager& AnimationSystem::getKeyframeAnimationManager()
	{
		return m_keyframeAnimationManager;
	}


	SkeletalAnimationManager& AnimationSystem::getSkeletalAnimationManager()
	{
		return m_skeletalAnimationManager;
	}



	void AnimationSystem::animateAll(const float deltaTime)
	{

		/***KEYFRAME ANIAMTION***/


		TheEngine::ECS::Query query = m_ecsEngine.getQuery<RootEntityComponent, AnimationComponent, AnimationStateComponent>();

		for (auto& chunkArrayView : query.getChunkArrayViews())
		{
			const size_t count = chunkArrayView.getCount();

			const AnimationComponent* animationComponentArray = chunkArrayView.getComponentArray<AnimationComponent>();
			//const TheEngine::ECS::EntityId* entityIds = chunkArrayView.getChunkRecordArray();
			AnimationStateComponent* animationStateComponentArray = chunkArrayView.getComponentArray<AnimationStateComponent>();
			
			
			if (animationComponentArray == nullptr || animationStateComponentArray == nullptr) continue;

			for (size_t i = 0; i < count; ++i)
			{

				

				const AnimationCPUComponent& animationCPUComponent = m_keyframeAnimationManager.getAnimationCPUComponentFromId(animationComponentArray[i].id);

				

				const std::vector<TheEngine::ECS::EntityId>& animationTargetToEntityIds = animationCPUComponent.engineEntityIds;

				std::vector<EngineTransformationComponent*> animationTargetTransformationComponents = getTransfomrationsOfEntities(animationTargetToEntityIds);


				KeyframeAnimationInterpolator::animate(animationStateComponentArray[i], animationCPUComponent, animationTargetTransformationComponents, deltaTime);
					


			}


		}




		/***BONE ANIMATION SYSTEM***/

		//we will have to query again this time to get BoneComponent Also






	}

}