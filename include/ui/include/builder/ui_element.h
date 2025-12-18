#pragma once
#include "ecs.h"



namespace UI
{


	//need to use a facade no other way 
	class UIElement
	{

	private:

		ECS::ECSEngine& m_ecsEngine;

		const ECS::EntityId m_entityId;


	public:

		UIElement(const ECS::EntityId& entityId, ECS::ECSEngine& ecsEngine);

		ECS::EntityId getEntityId();


		//component access method
		template <typename ComponentType>
		ComponentType* getComponent();
	

		void addChild(UIElement& uiChildElement);



	};


	template <typename ComponentType>
	ComponentType* UIElement::getComponent()
	{
		ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_entityId);

		ComponentType* componentPtr = entityChunkView.getComponent<ComponentType>();

		return componentPtr;
	}



}