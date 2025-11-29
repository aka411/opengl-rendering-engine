#include "../../include/builder/ui_base_builder.h"



namespace UI
{

	 TheEngine::ECS::ECSEngine* UIBaseBuilder::m_uiECSEngine = nullptr;

	void UIBaseBuilder::setECSEngine(TheEngine::ECS::ECSEngine* uiECSEngine)
	{
		m_uiECSEngine = uiECSEngine;
	}


	UIBaseBuilder& UIBaseBuilder::withPosition(glm::vec3 position)
	{
		/*
		 TheEngine::ECS::EntityChunkView entityChunkView= m_uiECSEngine->getEntityChunkView(m_entityId);


		 UITransformComponent* uiTransformComponentPtr = entityChunkView.getComponent<UITransformComponent>();

		if (uiTransformComponentPtr)
		{
			uiTransformComponentPtr->position = position;
			return *this;
		}
		*/
		UITransformComponent uiTransformComponent;
		uiTransformComponent.position = position;
		m_uiECSEngine->addComponentToEntity<UITransformComponent>(m_entityId, uiTransformComponent);

		return *this;
	}




	UIBaseBuilder& UIBaseBuilder::withRectDimensions(const float width,const float height)
	{
		/*
		TheEngine::ECS::EntityChunkView entityChunkView = m_uiECSEngine->getEntityChunkView(m_entityId);
		UIRectDimensionsComponent* uiRectDimensionsComponentPtr = entityChunkView.getComponent<UIRectDimensionsComponent>();
		if (uiRectDimensionsComponentPtr)
		{
			uiRectDimensionsComponentPtr->width = width;
			uiRectDimensionsComponentPtr->height = height;
			return *this;
		}
		*/
		UIRectDimensionsComponent uiRectDimensionsComponent;
		uiRectDimensionsComponent.width = width;
		uiRectDimensionsComponent.height = height;
		m_uiECSEngine->addComponentToEntity<UIRectDimensionsComponent>(m_entityId, uiRectDimensionsComponent);
		return *this;

	}


	UIEntity UIBaseBuilder::build()
	{
		m_uiECSEngine->processBufferedCommands();
		return m_entityId;
	}




}