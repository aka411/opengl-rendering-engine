#include "../../../../include/ui/include/builder/ui_builder.h"

namespace UI
{




	UIBuilder::UIBuilder(UICoreSystem& uiCoreSystem)
		: m_uiCoreSystem(uiCoreSystem), m_ecsEngine(uiCoreSystem.getECSEngine())
	{


	}





	UIWindowBuilder UIBuilder::createUIWindow()
	{

		return UIWindowBuilder(m_uiCoreSystem);

	}

	UIPanelBuilder UIBuilder::createUIPanel()
	{

		return UIPanelBuilder(m_uiCoreSystem);
	}



	UIGraphBuilder UIBuilder::createUIGraph()
	{
		return UIGraphBuilder(m_uiCoreSystem);
	}




UITextLabelBuilder UIBuilder::createUITextLabel()
{




	return UITextLabelBuilder(m_uiCoreSystem);
}




}