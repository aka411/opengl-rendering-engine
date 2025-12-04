#include "../include/ecs_hierarchy_traverser.h"
#include "../include/components.h"





void ECSHierarchyTraverser::reset()
{
	//m_currentIndex = -1;
	//m_parentIndex = -1;


	while (!m_visitInfoStack.empty())
	{
		m_visitInfoStack.pop();
	}
	while (!m_visitedEntityStack.empty())
	{
		m_visitedEntityStack.pop();
	}

	m_traversalComplete = true;

	return;

}


void ECSHierarchyTraverser::reverse()
{
	if (m_visitInfoStack.empty())
	{

		reset();
		return;
	}

	while (m_visitInfoStack.top().visitedChildIndex >= m_visitInfoStack.top().totalChildCount)
	{
		m_visitInfoStack.pop();
		m_visitedEntityStack.pop();

		if (m_visitedEntityStack.empty())
		{
			reset();
			return;
		}

	}


	m_parentEntityId = m_visitedEntityStack.top();

	//assert(m_model->nodes.size() > m_parentIndex);
	//assert(m_model->nodes[m_parentIndex].children.size() > m_visitInfoStack.top().visitedChildIndex);
	TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_parentEntityId);

	m_currentEntityId = entityChunkView.getComponent<EngineChildrenComponent>()->childrenEntities[m_visitInfoStack.top().visitedChildIndex];
	m_visitInfoStack.top().visitedChildIndex++;

	return;
}

ECSHierarchyTraverser::ECSHierarchyTraverser(TheEngine::ECS::ECSEngine& ecsEngine) : m_ecsEngine(ecsEngine)
{

}




void ECSHierarchyTraverser::setRootEntity(TheEngine::ECS::EntityId rootEntityId)
{
	m_currentEntityId = rootEntityId;
	m_traversalComplete = false;
}

TheEngine::ECS::EntityId ECSHierarchyTraverser::getCurrentEntityId()
{


	return m_currentEntityId;
}

TheEngine::ECS::EntityId ECSHierarchyTraverser::getParentEntityId()
{
	return m_parentEntityId;
}

void ECSHierarchyTraverser::traverse()
{
	if (m_traversalComplete)
	{
		return;
	}


	//std::vector<int> childrenList = m_model->nodes[m_currentIndex].children;
	TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_currentEntityId);
	EngineChildrenComponent* engineChildrenComponent = entityChunkView.getComponent<EngineChildrenComponent>();

	if (engineChildrenComponent == nullptr)
	{
		//feels hacky though
		reverse();
		return;
	}

	std::vector<TheEngine::ECS::EntityId>& childrenList = engineChildrenComponent->childrenEntities;



	VisitInfo visitInfo;
	visitInfo.totalChildCount = childrenList.size();
	visitInfo.visitedChildIndex = 0;

	m_visitInfoStack.push(visitInfo);
	m_visitedEntityStack.push(m_currentEntityId);

	if (childrenList.size() == 0) // leaf reached
	{
		reverse();
		return;
	}


	m_parentEntityId = m_currentEntityId;
	m_currentEntityId = childrenList[0];
	m_visitInfoStack.top().visitedChildIndex++;

	return;


}

bool ECSHierarchyTraverser::isTraversalComplete() const
{



	return m_traversalComplete;
	
}














