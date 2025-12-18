#pragma once
#include "../core/ui_core_system.h"
#include "ecs.h"



//OBJECTIVE :  TO TRANSVERSE ALL MATERIAL AND UPDATE UIRENDERMESH 
//RUNS EVERY FRAME

namespace UI
{ 

class UIMeshSystem
{

private:

	UICoreSystem& m_uiCoreSystem;
	ECS::ECSEngine& m_ecsEngine;

public:
	UIMeshSystem(UICoreSystem& uiCoreSystem);

	void generateUIMeshesForDirty();



};



}