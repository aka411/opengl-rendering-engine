#pragma once
#include "../../../../external/the-engine/the-engine/ecs/include/common_data_types.h"
#include "../../../../external/the-engine/the-engine/ecs/include/ecs_engine.h"


namespace UI
{


	//need to use a facade no other way 
	class UIElement
	{

	private:

		TheEngine::ECS::ECSEngine& m_ecsEngine;

		const TheEngine::ECS::EntityId m_entityId;


	public:

		UIElement(const TheEngine::ECS::EntityId& entityId, TheEngine::ECS::ECSEngine& ecsEngine);

		TheEngine::ECS::EntityId getEntityId();


		//component access method
		template <typename ComponentType>
		ComponentType& getComponent();
	

		void addChild(UIElement& uiChildElement);



	};






}