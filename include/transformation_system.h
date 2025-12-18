#pragma once
#include "ecs.h"
#include "components.h"

class TransformationSystem
{

private:

	ECS::ECSEngine& m_ecsEngine;

	void buildLocalMatrix(EngineTransformationComponent& transform);
	EngineTransformationComponent& getTransformationComponent(ECS::EntityId& entityId);

	

public:

	TransformationSystem(ECS::ECSEngine& ecsEngine);


	void updateTransformation(ECS::EntityId rootEntityId);
	void updateTransformationsForAllEntities();
};