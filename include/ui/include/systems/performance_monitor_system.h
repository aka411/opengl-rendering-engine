#pragma once
#include "../core/ui_component.h"
#include "../core/ui_core_system.h"
#include "ecs.h"
#include "../builder/ui_element.h"

class PerformanceMonitorSystem
{

private:

	UI::UICoreSystem& m_uiCoreSystem;
	ECS::ECSEngine& m_ecsEngine;

	
	UI::UIElement* m_fpsGraphElement = nullptr;
	//change to ui element



public:
	PerformanceMonitorSystem(UI::UICoreSystem& uiCoreSystem);

	void setUp(UI::UIElement& fpsGraphElement);

	void UpdateFPSMeter(const float deltaTime);
	void UpdateMemoryUsage(const size_t UsedBytes);



};
