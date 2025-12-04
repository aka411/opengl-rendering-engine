#pragma once
#include "ui_base_builder.h"

namespace UI
{

	class UIWindowBuilder : public UIBaseBuilder
	{

	private:


	public:
		//psoition always at origin
		UIWindowBuilder(UICoreSystem& uiCoreSystem);


		UIWindowBuilder& withRectDimensions(const float width, const float height);


	};





}