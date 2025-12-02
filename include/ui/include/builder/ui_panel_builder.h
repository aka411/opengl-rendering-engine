#pragma once
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>

#include "ui_base_builder.h"

namespace UI
{

	class UIPanelBuilder : public UIBaseBuilder
	{
		
	protected:


	

	public:

		UIPanelBuilder(UICoreSystem& uiCoreSystem);

		

		UIPanelBuilder& withPosition(glm::vec3 position);
		UIPanelBuilder& withRectDimensions(const float width, const float height);


		//Panel specific methods
		UIPanelBuilder& withColour(glm::vec4 color);
		UIPanelBuilder& withRoundConerRadius(float radius);


	

		
	};

}