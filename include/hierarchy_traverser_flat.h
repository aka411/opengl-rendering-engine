#pragma once
#include "gltf_flat_parser.h"
#include <stack>

struct VisitInfo
{
	size_t visitedChildIndex = 0;
	size_t totalChildCount = 0;
};

class HierarchyTraverserFlat
{
private:
	Engine::Model* m_model = nullptr;

	using Index = int;

	Index m_currentIndex = - 1;
	Index m_parentIndex = -1;


	bool m_traversalComplete = false;

	//using vector would be more better casue it has easier reset
	std::stack <VisitInfo> m_visitInfoStack;
	std::stack<int> m_visitedNodeIndexStack;

	void reset();
	void reverse();
public:

	HierarchyTraverserFlat();

	void setModel(Engine::Model& model);

	Engine::EngineNode* getCurrentNode();
	Engine::EngineNode* getParentNode();


	void traverse();

	bool isTraversalComplete() const;
};