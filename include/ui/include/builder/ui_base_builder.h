#pragma once

#include "../../../../include/ui/include/core/ui_core_system.h"
#include "ui_element.h"
#include <glm/glm.hpp>


namespace UI
{


	class UIBaseBuilder
	{

	protected:

		UICoreSystem& m_uiCoreSystem;// has ECSEngine and UIVertexBufferManagementSystem

		ECS::ECSEngine& m_ecsEngine;

	
		UIElement m_uiElement;


	public:

		UIBaseBuilder(UICoreSystem& uiCoreSystem);



		UIBaseBuilder& withPosition(glm::vec3 position);

		UIBaseBuilder& withRectDimensions(const float width, const float height);


		UIElement build();




	};


	


}