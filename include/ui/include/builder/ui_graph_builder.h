#pragma once
#include "ui_base_builder.h"
#include "../core/ui_core_system.h"
#include <glm/glm.hpp>

namespace UI
{

	class UIGraphBuilder : public UIBaseBuilder
	{

	private://would i need this?


	public:

		UIGraphBuilder(UICoreSystem& uiCoreSystem);


		UIGraphBuilder& withPosition(glm::vec3 position);
		UIGraphBuilder& withRectDimensions(const float width, const float height);


		//Panel specific methods
		UIGraphBuilder& withColour(glm::vec4 color);



	};


}