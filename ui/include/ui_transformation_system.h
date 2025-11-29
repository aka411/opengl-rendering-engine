#pragma once
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"

class UITransformationSystem
{


private:

	static TheEngine::ECS::ECSEngine* m_ecsEngine;

	UITransformationSystem();
public:
	
	static void setECSEngine(TheEngine::ECS::ECSEngine& ecsEngine);

	static void updateLocalTransformation();

};