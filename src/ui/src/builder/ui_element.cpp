#include "../../../../include/ui/include/builder/ui_element.h"
#include "../../../../include/components.h"


namespace UI
{


	UIElement::UIElement(const ECS::EntityId& entityId, ECS::ECSEngine& ecsEngine) : m_entityId(entityId), m_ecsEngine(ecsEngine)
	{



	}


	ECS::EntityId UIElement::getEntityId()
	{
		return m_entityId;
	}


	void UIElement::addChild(UIElement& uiChildElement)
	{

		EngineChildrenComponent* engineChildrenComponentPtr = getComponent<EngineChildrenComponent>();
		if (engineChildrenComponentPtr == nullptr)
		{
			EngineChildrenComponent engineChildrenComponent;
			engineChildrenComponent.childrenEntities.push_back(uiChildElement.getEntityId());
			m_ecsEngine.addComponentToEntity<>(m_entityId, engineChildrenComponent);
			m_ecsEngine.processBufferedCommands();
			return;
		}
		engineChildrenComponentPtr->childrenEntities.push_back(uiChildElement.getEntityId());
	}



}