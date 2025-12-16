#pragma once
#include "../builder/ui_element.h"
#include "../core/ui_core_system.h"
#include "../../../../include/components.h"




namespace UI
{


	class UITransformationSystem
	{


	private:

		UICoreSystem& m_uiCoreSystem;

	public:


		UITransformationSystem(UICoreSystem& uiCoreSystem);

		EngineTransformationComponent& getTransformationComponent(ECS::EntityId& entityId);

		void buildLocalMatrix(EngineTransformationComponent& transform);
		
		void updateTransformation(ECS::EntityId rootEntityId);
		void updateTransformation(UIElement& rootUIElement);

	};


}