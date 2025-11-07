#include "../include/hierarchy_traverser_flat.h"


HierarchyTraverserFlat::HierarchyTraverserFlat()
{

}

void HierarchyTraverserFlat::setModel(Engine::Model& model)
{
	m_model = &model;

	m_currentIndex = static_cast<int>(model.rootNodeIndex);

	m_traversalComplete = false;
}

Engine::EngineNode* HierarchyTraverserFlat::getCurrentNode()
{
	assert(m_currentIndex < static_cast<int>(m_model->nodes.size()));
	if (m_currentIndex < 0) return nullptr;
	return &m_model->nodes[m_currentIndex];	
}

Engine::EngineNode* HierarchyTraverserFlat::getParentNode()
{
	assert(m_parentIndex < static_cast<int>(m_model->nodes.size()));
	if (m_parentIndex < 0) return nullptr;	
	return &m_model->nodes[m_parentIndex];
}



void HierarchyTraverserFlat::reset()
{
	m_currentIndex = -1;
	m_parentIndex = -1;
	

	while (!m_visitInfoStack.empty())
	{
		m_visitInfoStack.pop();
	}
	while (!m_visitedNodeIndexStack.empty())
	{
		m_visitedNodeIndexStack.pop();
	}

	m_traversalComplete = true;

	return;
}



void HierarchyTraverserFlat::reverse()
{
	while (m_visitInfoStack.top().visitedChildIndex >= m_visitInfoStack.top().totalChildCount)
	{
		m_visitInfoStack.pop();
		m_visitedNodeIndexStack.pop();

		if(m_visitedNodeIndexStack.empty())
		{
			reset();
			return;
		}

	}


	m_parentIndex = m_visitedNodeIndexStack.top();
	assert(m_model->nodes.size() > m_parentIndex);
    assert (m_model->nodes[m_parentIndex].children.size() > m_visitInfoStack.top().visitedChildIndex);
	m_currentIndex = m_model->nodes[m_parentIndex].children[m_visitInfoStack.top().visitedChildIndex];

	m_visitInfoStack.top().visitedChildIndex++;

	return;
}


void HierarchyTraverserFlat::traverse()
{


	if (m_traversalComplete)
	{
		return;
	}


	std::vector<int> childrenList = m_model->nodes[m_currentIndex].children;

	VisitInfo visitInfo;
	visitInfo.totalChildCount = childrenList.size();
	visitInfo.visitedChildIndex = 0;

	m_visitInfoStack.push(visitInfo);
	m_visitedNodeIndexStack.push(m_currentIndex);

	if (childrenList.size() == 0) // leaf reached
	{
		reverse();
		return;
	}


	m_parentIndex = m_currentIndex;
	m_currentIndex = childrenList[0];
	m_visitInfoStack.top().visitedChildIndex++;
	return;




}

bool HierarchyTraverserFlat::isTraversalComplete() const
{
	return m_traversalComplete;
}