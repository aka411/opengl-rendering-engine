#pragma once
//#include "gltf_parser.h"
//#include "hierarchy_traverser.h"
#include "hierarchy_traverser_flat.h"


class TransformationSystem
{
private:

	//HierarchyTraverser m_hierarchyTraverser;
	 HierarchyTraverserFlat m_hierarchyTraverser;
public:
	TransformationSystem();

	//void updateWorldTransforms(Engine::EngineNode& rootNode);
	void updateWorldTransforms(Engine::Model& engineModel);

	void buildLocalMatrix(Engine::Transformation& transform);

};