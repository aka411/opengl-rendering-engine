#include "../../../../include/ui/include/core/ui_system.h"


namespace UI
{


	UI::UISystem::UISystem(UICoreSystem& uiCoreSystem) : m_uiCoreSystem(uiCoreSystem),m_uiLayoutSystem(uiCoreSystem), m_uiMeshSystem(uiCoreSystem)
	{


	}

	void UISystem::update()
	{

		m_uiMeshSystem.generateUIMeshesForDirty();
		m_uiLayoutSystem.updateLayout();



	}

}