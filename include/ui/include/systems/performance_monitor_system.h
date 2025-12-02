#pragma once
#include "../core/ui_component.h"
#include "../core/ui_core_system.h"
#include "../../../../external/the-engine/the-engine/ecs/include/ecs_engine.h"

class PerformanceMonitorSystem
{

private:

	UI::UICoreSystem& m_uiCoreSystem;
	TheEngine::ECS::ECSEngine& m_ecsEngine;

	//TODO :  REWORK
	UI::UIEntity m_fpsUIEntity;




public:
	PerformanceMonitorSystem(UI::UICoreSystem& uiCoreSystem);

	void setUp();

	void UpdateFPSMeter(const float deltaTime);
	void UpdateMemoryUsage(const size_t UsedBytes);



};
