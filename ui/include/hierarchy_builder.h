#pragma once
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "ui_component.h"



class HierarchyBuilder
{

private:

	static TheEngine::ECS::ECSEngine* m_ecsEngine;// = nullptr;
	// Private constructor to prevent instantiation
	HierarchyBuilder() {}

public:
 
	static void makeEntityRoot(UI::UIEntity entity);

	static void addChildEntity(UI::UIEntity parentEntity, UI::UIEntity childEntity);

	static void setECSEngine(TheEngine::ECS::ECSEngine& ecsEngine);

};