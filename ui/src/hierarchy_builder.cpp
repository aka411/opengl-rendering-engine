#include "../include/ui_component.h"
#include "../include/hierarchy_builder.h"



TheEngine::ECS::ECSEngine* HierarchyBuilder::m_ecsEngine = nullptr;

void HierarchyBuilder::makeEntityRoot(UI::UIEntity entity)
{
	UI::UIRootComponent uiRootComponent;

	m_ecsEngine->addComponentToEntity<UI::UIRootComponent>(entity,uiRootComponent);

}

void HierarchyBuilder::addChildEntity(UI::UIEntity parentEntity, UI::UIEntity childEntity)
{


	TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine->getEntityChunkView(parentEntity);
	UI::UIChildrenComponent* uiChildrenComponentPtr = entityChunkView.getComponent<UI::UIChildrenComponent>();


	if(uiChildrenComponentPtr)
	{
		uiChildrenComponentPtr->children.push_back(childEntity);
		
		return;
	}

	UI::UIChildrenComponent uiChildrenComponent;
	uiChildrenComponent.children.push_back(childEntity);

	m_ecsEngine->addComponentToEntity<UI::UIChildrenComponent>(parentEntity, uiChildrenComponent);


}

void HierarchyBuilder::setECSEngine(TheEngine::ECS::ECSEngine& ecsEngine)
{
	m_ecsEngine = &ecsEngine;
}
