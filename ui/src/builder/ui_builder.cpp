#include "../../include/builder/ui_builder.h"
#include "../../include/builder/ui_base_builder.h"

namespace UI
{


	TheEngine::ECS::ECSEngine* UIBuilder::m_uiECSEngine = nullptr;

	void UIBuilder::setECSEngine(TheEngine::ECS::ECSEngine& ecsEngine)
	{
		m_uiECSEngine = &ecsEngine;
		UIBaseBuilder::setECSEngine(&ecsEngine);

	}


	UIPanelBuilder UIBuilder::createUIPanel()
	{



		return UIPanelBuilder::createUIPanel();
	}

	//UIGraphBuilder UIBuilder::createUIGraph()
	//{



	//	return UIGraphBuilder();
	//}


	/**
UITextBuilder UIBuilder::createUITextBox()
{




	return UITextBuilder();
}
*/



}