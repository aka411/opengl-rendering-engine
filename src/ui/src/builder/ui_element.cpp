#include "../../../../include/ui/include/builder/ui_element.h"


namespace UI
{


	UIElement::UIElement(const TheEngine::ECS::EntityId& entityId, TheEngine::ECS::ECSEngine& ecsEngine) : m_entityId(entityId), m_ecsEngine(ecsEngine)
	{



	}


	TheEngine::ECS::EntityId UIElement::getEntityId()
	{
		return m_entityId;
	}


	void UIElement::addChild(UIElement& uiChildElement)
	{

		

	}



}