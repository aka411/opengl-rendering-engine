#include "../../../../include/ui/include/builder/ui_text_label_builder.h"
#include <glm/ext/vector_float3.hpp>
#include "../../../../include/ui/include/core/ui_component.h"

namespace UI
{


	UITextLabelBuilder::UITextLabelBuilder(UICoreSystem& uiCoreSystem) : UIBaseBuilder(uiCoreSystem)
	{
		UITextComponent uiTextComponent;
		m_ecsEngine.addComponentToEntity<UITextComponent>(m_uiElement.getEntityId(), uiTextComponent);

		m_ecsEngine.processBufferedCommands();

	}


	UITextLabelBuilder& UITextLabelBuilder::withPosition(glm::vec3 position)
	{

		UIBaseBuilder::withPosition(position);
		return *this;

	}



	UITextLabelBuilder& UITextLabelBuilder::withRectDimensions(const float width, const float height)
	{

		UIBaseBuilder::withRectDimensions(width, height);
		return *this;
	}


	//Text specific methods
	UITextLabelBuilder& UITextLabelBuilder::withColour(glm::vec4 color)
	{
		ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_uiElement.getEntityId());
		UITextComponent* uiTextComponent = entityChunkView.getComponent<UITextComponent>();

		assert(uiTextComponent != nullptr && "UITextComponent not found in entity");

		uiTextComponent->color = color;

		return *this;


	}


	UITextLabelBuilder& UITextLabelBuilder::withText(const std::string& text)
	{
		ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_uiElement.getEntityId());
		UITextComponent* uiTextComponent = entityChunkView.getComponent<UITextComponent>();

		assert(uiTextComponent != nullptr && "UITextComponent not found in entity");

		uiTextComponent->text = text;

		return *this;
	}



}