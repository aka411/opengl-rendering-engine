#pragma once

#include <vector>
#include "low-level/gpu_material_system.h"

#include <glm/glm.hpp> 
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../external/the-engine/the-engine/ecs/include/common_data_types.h"










struct EngineRenderComponent
{
	VertexFormat vertexFormat = 0;
	size_t vertexCount = 0;
	size_t vertexOffset = 0;

	bool isIndexed = false;

	IndexType indexType = IndexType::UNKNOWN;
	size_t indexCount = 0;
	size_t indexOffset = 0;

	MaterialId materialId = 0;
};


struct EngineFatRenderComponent
{
	std::vector<EngineRenderComponent> engineRenderComponents;

};

struct EngineChildrenComponent
{
	std::vector<TheEngine::ECS::EntityId> childrenEntities;

};





struct EngineTransformationComponent
{
	glm::mat4 localTransformMatrix = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	glm::mat4 worldTransformMatrix = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);//used by rendersystem

	glm::vec3 position{ 0.0f,0.0f,0.0f };
	glm::vec3 scale{ 1.0f,1.0f,1.0f };
	glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
};







struct RootEntityComponent
{
	bool isRoot = true;
};







/*
struct UIEngineRenderComponent
{

};
*/