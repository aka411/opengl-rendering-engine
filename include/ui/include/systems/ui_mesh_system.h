#pragma once
#include "../core/ui_core_system.h"
#include "../../../../external/the-engine/the-engine/ecs/include/ecs_engine.h"



//OBJECTIVE :  TO TRANSVERSE ALL MATERIAL AND UPDATE UIRENDERMESH 
//RUNS EVERY FRAME

namespace UI
{ 

class UIMeshSystem
{

private:

	UICoreSystem& m_uiCoreSystem;
	TheEngine::ECS::ECSEngine& m_ecsEngine;

public:
	UIMeshSystem(UICoreSystem& uiCoreSystem);

	void generateUIMeshesForDirty();



};



}