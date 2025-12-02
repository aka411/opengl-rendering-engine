#pragma once

namespace UI
{
	class UISystem
	{

	private:

		UICoreSystem& m_uiCoreSystem;//not owner

		//UI Internal Systems
		UIHierarchySystem m_uiHierarchySystem;//used mostly by layout system 



	public:

		const UIBuilder uiBuilder;



		UISystem(UICoreSystem& uiCoreSystem);

	};

}