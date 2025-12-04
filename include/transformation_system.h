#pragma once
#include "../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "components.h"

class TransformationSystem
{

private:

	TheEngine::ECS::ECSEngine& m_ecsEngine;

	void buildLocalMatrix(EngineTransformationComponent& transform);
	EngineTransformationComponent& getTransformationComponent(TheEngine::ECS::EntityId& entityId);

	

public:

	TransformationSystem(TheEngine::ECS::ECSEngine& ecsEngine);


	void updateTransformation(TheEngine::ECS::EntityId rootEntityId);
	void updateTransformationsForAllEntities();
};