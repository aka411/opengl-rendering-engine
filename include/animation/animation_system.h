#pragma once
#include <vector>
#include "../components.h"
#include "keyframe_animation_manager.h"
#include "skeletal_animation_manager.h"
#include "ecs.h"

namespace Engine
{



	class AnimationSystem
	{

	private:

	
		KeyframeAnimationManager m_keyframeAnimationManager;//owner
		SkeletalAnimationManager m_skeletalAnimationManager;//owner, //needs GPUBufferManager

		//Uses KeyframeAnimationSolver and SkeletalAnimationSolver

		ECS::ECSEngine& m_ecsEngine;

		// A ECSEngine Coupled method
		std::vector<EngineTransformationComponent*> getTransfomrationsOfEntities(const std::vector<ECS::EntityId>& entityIds);


	public:

		AnimationSystem(ECS::ECSEngine& ecsEngine, GPUBufferManager& gpuBufferManager);

		KeyframeAnimationManager& getKeyframeAnimationManager();
		SkeletalAnimationManager& getSkeletalAnimationManager();



		void animateAll(const float deltaTime);



	};

}