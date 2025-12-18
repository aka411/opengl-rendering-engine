#pragma once
#include <stack>
#include "ecs.h"


class ECSHierarchyTraverser
{


private:


	ECS::ECSEngine& m_ecsEngine;



	struct VisitInfo
	{
		size_t visitedChildIndex = 0;
		size_t totalChildCount = 0;
	};

	ECS::EntityId m_currentEntityId;
	ECS::EntityId m_parentEntityId;


	bool m_traversalComplete = false;

	//using vector would be more better casue it has easier reset
	std::stack <VisitInfo> m_visitInfoStack;
	std::stack<ECS::EntityId> m_visitedEntityStack;

	void reset();
	void reverse();


public:

	ECSHierarchyTraverser(ECS::ECSEngine& ecsEngine);
	void setRootEntity(ECS::EntityId rootEntityId);

	ECS::EntityId getCurrentEntityId();
	ECS::EntityId getParentEntityId();


	void traverse();

	bool isTraversalComplete() const;

};