#include "../include/transformation_system.h"
#include <glm/gtc/quaternion.hpp>
#include "../include/gltf_flat_parser.h"

TransformationSystem::TransformationSystem()
{


}





void TransformationSystem::updateWorldTransforms(Engine::Model& engineModel)
{

	m_hierarchyTraverser.setModel(engineModel);
	//TODO: might be buggy not tested
	while (!m_hierarchyTraverser.isTraversalComplete())
	{
		Engine::EngineNode* currentNode = m_hierarchyTraverser.getCurrentNode();

		Engine::EngineNode* parentNode = m_hierarchyTraverser.getParentNode();

		if (currentNode != nullptr)
		{
			buildLocalMatrix(currentNode->transformation);
		}
		if (parentNode == nullptr && currentNode != nullptr)
		{
			currentNode->transformation.worldTransformMatrix = currentNode->transformation.localTransformMatrix;
		}

		if (parentNode != nullptr && currentNode != nullptr)
		{
			currentNode->transformation.worldTransformMatrix = parentNode->transformation.worldTransformMatrix * currentNode->transformation.localTransformMatrix;
		}


		m_hierarchyTraverser.traverse();
	}


}

void TransformationSystem::buildLocalMatrix(Engine::Transformation& transform)
{


	

	glm::mat4 S = glm::scale(glm::mat4(1.0f), transform.scale);
	glm::mat4 R = glm::mat4_cast(transform.rotation);
	glm::mat4 T = glm::translate(glm::mat4(1.0f), transform.position);

	transform.localTransformMatrix = T * R * S ;


}