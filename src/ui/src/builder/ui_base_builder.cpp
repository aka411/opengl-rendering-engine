#include "../../../../include/ui/include/builder/ui_base_builder.h"
#include "../../../../include/ui/include/core/ui_component.h"
#include "../../../../include/components.h"




namespace UI
{



	UIBaseBuilder::UIBaseBuilder(UICoreSystem& uiCoreSystem): m_uiCoreSystem(uiCoreSystem),m_ecsEngine(m_uiCoreSystem.getECSEngine()),
		 m_uiElement(m_uiCoreSystem.getECSEngine().createEntity(), m_ecsEngine)
	{

		
		EngineTransformationComponent engineTransformationComponent;
		UIRectDimensionsComponent uiRectDimensionsComponent;

		m_ecsEngine.addComponentToEntity<EngineTransformationComponent>(m_uiElement.getEntityId(), engineTransformationComponent);
		m_ecsEngine.addComponentToEntity<UIRectDimensionsComponent>(m_uiElement.getEntityId(), uiRectDimensionsComponent);
		
		m_ecsEngine.processBufferedCommands();//or add a direct add method to bypass buffering

	}




	UIBaseBuilder& UIBaseBuilder::withPosition(glm::vec3 position)
	{
		//these base methods are always retrival

		TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_uiElement.getEntityId());
		EngineTransformationComponent* engineTransformationComponent = entityChunkView.getComponent<EngineTransformationComponent>();
		
		assert(engineTransformationComponent != nullptr);

		engineTransformationComponent->position = position;

		return *this;
	}




	UIBaseBuilder& UIBaseBuilder::withRectDimensions(const float width,const float height)
	{

		TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_uiElement.getEntityId());
		UIRectDimensionsComponent* uiRectDimensionsComponent = entityChunkView.getComponent<UIRectDimensionsComponent>();
		
		assert(uiRectDimensionsComponent != nullptr);

		uiRectDimensionsComponent->height = height;
		uiRectDimensionsComponent->width = width;

		return *this;
	}


	UIElement UIBaseBuilder::build()
	{

		//m_ecsEngine.processBufferedCommands();
		return m_uiElement;
	}




}