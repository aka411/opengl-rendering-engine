#pragma once
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"



namespace UI
{ 

class UIMeshGenerationSystem
{

private:

	TheEngine::ECS::ECSEngine* m_ecsEngine = nullptr;

public:
	UIMeshGenerationSystem(TheEngine::ECS::ECSEngine* ecsEngine);

	void generateUIMeshes();



};



}