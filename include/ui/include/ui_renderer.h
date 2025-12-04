#pragma once
#include "core/ui_core_system.h"
#include "../../camera.h"


namespace UI
{
	class UIRenderer
	{
	private:

		UICoreSystem& m_uiCoreSystem;//not owner


		GLuint m_dummyVAO = 0;

		Engine::Camera m_uiCamera;
	
	public:
	
		UIRenderer(UICoreSystem& uiCoreSystem);

		void renderUI();


	};


}