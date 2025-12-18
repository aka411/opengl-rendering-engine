#include "../../include/animation/animation_system.h"
#include "../../include/animation/keyframe_animation_interpolator.h"
#include "../../include/animation/skeletal_animation_solver.h"



namespace Engine
{

	std::vector<EngineTransformationComponent*> AnimationSystem::getTransfomrationsOfEntities(const std::vector<ECS::EntityId>& entityIds)
	{
		std::vector<EngineTransformationComponent*> transformationComponents;

		transformationComponents.resize(entityIds.size());


		for (size_t i = 0 ; i < transformationComponents.size(); ++i)
		{
			ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(entityIds[i]);

			EngineTransformationComponent* engineTransformationComponent = entityChunkView.getComponent<EngineTransformationComponent>();

			assert(engineTransformationComponent != nullptr);

			transformationComponents[i] = engineTransformationComponent;
		}

		return transformationComponents;
	}





	AnimationSystem::AnimationSystem(ECS::ECSEngine& ecsEngine,GPUBufferManager& gpuBufferManager)
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

		
		ECS::Query query = m_ecsEngine.getQuery<RootEntityComponent, AnimationComponent, AnimationStateComponent>();

		for (auto& chunkArrayView : query.getChunkArrayViews())
		{
			const size_t count = chunkArrayView.getCount();

			const AnimationComponent* animationComponentArray = chunkArrayView.getComponentArray<AnimationComponent>();
			//const ECS::EntityId* entityIds = chunkArrayView.getChunkRecordArray();
			AnimationStateComponent* animationStateComponentArray = chunkArrayView.getComponentArray<AnimationStateComponent>();
			
			
			if (animationComponentArray == nullptr || animationStateComponentArray == nullptr) continue;

			for (size_t i = 0; i < count; ++i)
			{

				

				const AnimationCPUComponent& animationCPUComponent = m_keyframeAnimationManager.getAnimationCPUComponentFromId(animationComponentArray[i].id);

				

				const std::vector<ECS::EntityId>& animationTargetToEntityIds = animationCPUComponent.engineEntityIds;

				std::vector<EngineTransformationComponent*> animationTargetTransformationComponents = getTransfomrationsOfEntities(animationTargetToEntityIds);


				KeyframeAnimationInterpolator::animate(animationStateComponentArray[i], animationCPUComponent, animationTargetTransformationComponents, deltaTime);
					


			}


		}




		/***BONE ANIMATION SYSTEM***/

		//we will have to query again this time to get BoneComponent Also

		ECS::Query boneQuery = m_ecsEngine.getQuery<RootEntityComponent, AnimationComponent, AnimationStateComponent, BoneAnimationComponent>();

		for (auto& chunkArrayViewWithBone : boneQuery.getChunkArrayViews())
		{
			const size_t count = chunkArrayViewWithBone.getCount();

			BoneAnimationComponent* boneAnimationComponentArray = chunkArrayViewWithBone.getComponentArray<BoneAnimationComponent>();
			const AnimationComponent* animationComponentArray = chunkArrayViewWithBone.getComponentArray<AnimationComponent>();

			AnimationStateComponent* animationStateComponentArray = chunkArrayViewWithBone.getComponentArray<AnimationStateComponent>();


			if (boneAnimationComponentArray == nullptr || animationStateComponentArray == nullptr) continue;

			for (size_t i = 0; i < count; ++i)
			{

				const AnimationCPUComponent& animationCPUComponent = m_keyframeAnimationManager.getAnimationCPUComponentFromId(animationComponentArray[i].id);
				const BoneAnimationCPUComponent& boneCPUBoneAnimationCPUComponent = m_skeletalAnimationManager.getBoneAnimationCPUComponentFromId(boneAnimationComponentArray[i].boneDataId);
				



				const std::vector<ECS::EntityId>& animationTargetToEntityIds = animationCPUComponent.engineEntityIds;

				std::vector<EngineTransformationComponent*> animationTargetTransformationComponents = getTransfomrationsOfEntities(animationTargetToEntityIds);

				if (!animationStateComponentArray[i].isPlayingAnimation)
				{
					continue;
				}

				SkeletalAnimationSolver::animate(boneCPUBoneAnimationCPUComponent, boneAnimationComponentArray[i].jointMatrices, animationTargetTransformationComponents);

				m_skeletalAnimationManager.updateJointMatrixSetInSSBO(boneAnimationComponentArray[i].JointIdInSSBO, boneAnimationComponentArray[i].jointMatrices);
			}


		}




	}

}