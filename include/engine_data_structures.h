#pragma once
#include <vector>
#include <glm/ext/matrix_float4x4.hpp>




struct EngineNode
{
	TheEngine::EntityId entityId;

	std::vector<EngineNode> m_children;

};







