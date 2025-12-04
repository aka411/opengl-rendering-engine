#pragma once
#include <stack>
#include "../external/the-engine/the-engine/ecs/include/common_data_types.h"
#include "../external/the-engine/the-engine/ecs/include/ecs_engine.h"


class ECSHierarchyTraverser
{


private:


	TheEngine::ECS::ECSEngine& m_ecsEngine;



	struct VisitInfo
	{
		size_t visitedChildIndex = 0;
		size_t totalChildCount = 0;
	};

	TheEngine::ECS::EntityId m_currentEntityId;
	TheEngine::ECS::EntityId m_parentEntityId;


	bool m_traversalComplete = false;

	//using vector would be more better casue it has easier reset
	std::stack <VisitInfo> m_visitInfoStack;
	std::stack<TheEngine::ECS::EntityId> m_visitedEntityStack;

	void reset();
	void reverse();


public:

	ECSHierarchyTraverser(TheEngine::ECS::ECSEngine& ecsEngine);
	void setRootEntity(TheEngine::ECS::EntityId rootEntityId);

	TheEngine::ECS::EntityId getCurrentEntityId();
	TheEngine::ECS::EntityId getParentEntityId();


	void traverse();

	bool isTraversalComplete() const;

};