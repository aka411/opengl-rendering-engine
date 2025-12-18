#include "../../../../include/ui/include/systems/ui_transformation_system.h"
#include <glm/gtc/quaternion.hpp>
#include "../../../../include/ecs_hierarchy_traverser.h"




namespace UI
{


	UITransformationSystem::UITransformationSystem(UICoreSystem& uiCoreSystem) : m_uiCoreSystem(uiCoreSystem)
	{



	}

	EngineTransformationComponent& UITransformationSystem::getTransformationComponent(ECS::EntityId& entityId)
	{
		ECS::ECSEngine& ecsEngine = m_uiCoreSystem.getECSEngine();

		ECS::EntityChunkView entityChunkView = ecsEngine.getEntityChunkView(entityId);
		EngineTransformationComponent* engineTransformationComponentPtr = entityChunkView.getComponent<EngineTransformationComponent>();


		assert(engineTransformationComponentPtr != nullptr);


		return *engineTransformationComponentPtr;

	}


	void UITransformationSystem::buildLocalMatrix(EngineTransformationComponent& transform)
	{




		glm::mat4 S = glm::scale(glm::mat4(1.0f), transform.scale);
		glm::mat4 R = glm::mat4_cast(transform.rotation);
		glm::mat4 T = glm::translate(glm::mat4(1.0f), transform.position);

		transform.localTransformMatrix = T * R * S;


	}



	void UITransformationSystem::updateTransformation(ECS::EntityId rootEntityId)
	{


		ECS::ECSEngine& ecsEngine = m_uiCoreSystem.getECSEngine();

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

			ECS::EntityId parentEntityId = ecsHierarchyTraverser.getParentEntityId();
			ECS::EntityId childEntityId = ecsHierarchyTraverser.getCurrentEntityId();



			EngineTransformationComponent& parentTransformComponent = getTransformationComponent(parentEntityId);
			EngineTransformationComponent& childTransformComponent = getTransformationComponent(childEntityId);
			
			buildLocalMatrix(childTransformComponent);


			childTransformComponent.worldTransformMatrix = parentTransformComponent.worldTransformMatrix * childTransformComponent.localTransformMatrix;


			ecsHierarchyTraverser.traverse();

		}
		

	}








	void UITransformationSystem::updateTransformation(UIElement& rootUIElement)
	{



		updateTransformation(rootUIElement.getEntityId());


	}


}
