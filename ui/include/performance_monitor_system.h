#pragma once
#include "ui_component.h"
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"


class PerformanceMonitorSystem
{

private:

	TheEngine::ECS::ECSEngine* m_ecsEngine = nullptr;

	UI::UIEntity m_fpsUIEntity;


public:
	PerformanceMonitorSystem(TheEngine::ECS::ECSEngine* ecsEngine);

	void setUp();
	void UpdateFPSMeter(const float deltaTime);
	void UpdateMemoryUsage(const size_t UsedBytes);



};
