#pragma once
#include <vector>
#include "../components.h"
#include "keyframe_animation_manager.h"
#include "skeletal_animation_manager.h"
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"

namespace Engine
{



	class AnimationSystem
	{

	private:

	
		KeyframeAnimationManager m_keyframeAnimationManager;//owner
		SkeletalAnimationManager m_skeletalAnimationManager;//owner, //needs GPUBufferManager

		//Uses KeyframeAnimationSolver and SkeletalAnimationSolver

		TheEngine::ECS::ECSEngine& m_ecsEngine;

		// A ECSEngine Coupled method
		std::vector<EngineTransformationComponent*> getTransfomrationsOfEntities(const std::vector<TheEngine::ECS::EntityId>& entityIds);


	public:

		AnimationSystem(TheEngine::ECS::ECSEngine& ecsEngine, GPUBufferManager& gpuBufferManager);

		KeyframeAnimationManager& getKeyframeAnimationManager();
		SkeletalAnimationManager& getSkeletalAnimationManager();



		void animateAll(const float deltaTime);



	};

}