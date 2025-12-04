#pragma once
#include "../core/ui_core_system.h"
#include "ui_transformation_system.h"

namespace UI
{


	class UILayoutSystem
	{

	private:

		UICoreSystem& m_uiCoreSystem;

		UITransformationSystem m_uiTransformationSystem;

	public:
		UILayoutSystem(UICoreSystem& uiCoreSystem);

		void updateLayout();
	};




}