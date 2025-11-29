#pragma once
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>
#include "../../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "../ui_component.h"
#include "ui_base_builder.h"

namespace UI
{
	class UIPanelBuilder : public UIBaseBuilder
	{

	protected:


	
		UIPanelBuilder(UIEntity m_enityId);
	public:

		static UIPanelBuilder createUIPanel();

		

		UIPanelBuilder& withPosition(glm::vec3 position);
		//UIPanelBuilder& withRectBounds(glm::vec3 topLeft, glm::vec3 bottomRigth);
		UIPanelBuilder& withRectDimensions(const float width, const float height);

		//Panel specific methods
		UIPanelBuilder& withColour(glm::vec4 color);
		UIPanelBuilder& withRoundConerRadius(float radius);


		//UIEntity build();

		
	};

}