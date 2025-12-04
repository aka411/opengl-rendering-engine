#include "../../../../include/ui/include/builder/ui_window_builder.h"
#include "../../../../include/ui/include/core/ui_component.h"

namespace UI
{


	UIWindowBuilder::UIWindowBuilder(UICoreSystem& uiCoreSystem)  : UIBaseBuilder(uiCoreSystem)
	{

		UIRootComponent uiRootComponent;

		m_ecsEngine.addComponentToEntity<UIRootComponent>(m_uiElement.getEntityId(), uiRootComponent);
	

		//always at origin
		UIBaseBuilder::withPosition({ 0,0,0 });

		m_ecsEngine.processBufferedCommands();
	}





	UIWindowBuilder& UIWindowBuilder::withRectDimensions(const float width, const float height)
	{
		//should be window size or virtual window size in pixel units
		UIBaseBuilder::withRectDimensions(width, height);
		return *this;

	}


}