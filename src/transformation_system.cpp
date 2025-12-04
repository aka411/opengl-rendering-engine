#include "../include/transformation_system.h"
#include "../include/ecs_hierarchy_traverser.h"



TransformationSystem::TransformationSystem(TheEngine::ECS::ECSEngine& ecsEngine) : m_ecsEngine(ecsEngine)
{



}









EngineTransformationComponent& TransformationSystem::getTransformationComponent(TheEngine::ECS::EntityId& entityId)
{
	TheEngine::ECS::ECSEngine& ecsEngine = m_ecsEngine;

	TheEngine::ECS::EntityChunkView entityChunkView = ecsEngine.getEntityChunkView(entityId);
	EngineTransformationComponent* engineTransformationComponentPtr = entityChunkView.getComponent<EngineTransformationComponent>();


	assert(engineTransformationComponentPtr != nullptr);


	return *engineTransformationComponentPtr;

}


void TransformationSystem::buildLocalMatrix(EngineTransformationComponent& transform)
{




	glm::mat4 S = glm::scale(glm::mat4(1.0f), transform.scale);
	glm::mat4 R = glm::mat4_cast(transform.rotation);
	glm::mat4 T = glm::translate(glm::mat4(1.0f), transform.position);

	transform.localTransformMatrix = T * R * S;


}



void TransformationSystem::updateTransformation(TheEngine::ECS::EntityId rootEntityId)
{


	TheEngine::ECS::ECSEngine& ecsEngine = m_ecsEngine;

	ECSHierarchyTraverser ecsHierarchyTraverser(ecsEngine);


	ecsHierarchyTraverser.setRootEntity(rootEntityId);

	EngineTransformationComponent& rootTransformComponent = getTransformationComponent(rootEntityId);

	buildLocalMatrix(rootTransformComponent);

	//Need more thought here
	rootTransformComponent.worldTransformMatrix = rootTransformComponent.localTransformMatrix;


	ecsHierarchyTraverser.traverse(); // get next after root

	while (!ecsHierarchyTraverser.isTraversalComplete())
	{



		//get parent world transform and get current local transfrom
		// Wc = Wp * Lc;

		TheEngine::ECS::EntityId parentEntityId = ecsHierarchyTraverser.getParentEntityId();
		TheEngine::ECS::EntityId childEntityId = ecsHierarchyTraverser.getCurrentEntityId();



		EngineTransformationComponent& parentTransformComponent = getTransformationComponent(parentEntityId);
		EngineTransformationComponent& childTransformComponent = getTransformationComponent(childEntityId);

		buildLocalMatrix(childTransformComponent);


		childTransformComponent.worldTransformMatrix = parentTransformComponent.worldTransformMatrix * childTransformComponent.localTransformMatrix;


		ecsHierarchyTraverser.traverse();

	}

}


void TransformationSystem::updateTransformationsForAllEntities()
{
	//gets all entities with root and updates
	TheEngine::ECS::Query query = m_ecsEngine.getQuery<RootEntityComponent, EngineTransformationComponent>();



	for (auto& chunkArrayView : query.getChunkArrayViews())
	{
		const size_t count = chunkArrayView.getCount();


		RootEntityComponent* rootEntityComponentPtr = chunkArrayView.getComponentArray<RootEntityComponent>();
		const TheEngine::ECS::EntityId* rootEntityIdPtr = chunkArrayView.getChunkRecordArray();



		//use the geometry generator to create a quad mesh based on rect bounds

		if (rootEntityComponentPtr == nullptr || rootEntityIdPtr == nullptr)
		{
			continue;
		}

		for (size_t i = 0; i < count; ++i)
		{


			updateTransformation(rootEntityIdPtr[i]);



		}

	}



}




