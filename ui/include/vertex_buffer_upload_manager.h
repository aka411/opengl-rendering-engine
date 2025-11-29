#pragma once
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "ui_vertex_buffer_system.h"

class VertexBufferUploadManager
{

private:

	TheEngine::ECS::ECSEngine& m_ecsEngine;
	UI::UIVertexBufferSystem& m_uiVertexBufferSystem;


public:

	VertexBufferUploadManager(TheEngine::ECS::ECSEngine& ecsEngine, UI::UIVertexBufferSystem& uiVertexBufferSystem);

	void uploadAllVertexBuffers();
};