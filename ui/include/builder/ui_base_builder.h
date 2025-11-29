#pragma once
#include "../ui_component.h"
#include "../../../external/the-engine/the-engine/ecs/include/ecs_engine.h"


namespace UI
{


	class UIBaseBuilder
	{
	protected:

		static TheEngine::ECS::ECSEngine* m_uiECSEngine;

		UIEntity m_entityId;


	public:
		UIBaseBuilder(UIEntity entityId) : m_entityId(entityId) {};



		UIBaseBuilder& withPosition(glm::vec3 position);
		//UIBaseBuilder& withRectBounds(glm::vec3 topLeft, glm::vec3 bottomRigth);
		UIBaseBuilder& withRectDimensions(const float width,const float height);


		UIEntity build();


		static void setECSEngine(TheEngine::ECS::ECSEngine* uiECSEngine);


	};


	


}