#pragma once

#include <vector>
#include "low-level/gpu_material_system.h"
#include "low-level/rendering_system_data_types.h"
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
	glm::mat4 localTransformMatrix = glm::identity<glm::mat4>();
	glm::mat4 worldTransformMatrix = glm::identity<glm::mat4>();//used by rendersystem

	glm::vec3 position{ 0.0f,0.0f,0.0f };
	glm::vec3 scale{ 1.0f,1.0f,1.0f };
	glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
};







struct RootEntityComponent
{
	bool isRoot = true;
};






/******NOT COMPONENTS****/
struct PerObjectData
{
	uint64_t materialId;
	EngineTransformationComponent engineTransformationComponent;

};



struct RenderCommand
{

	size_t	vertexOffset = 0;//in bytes
	size_t	vertexCount = 0;

	IndexType indexType;

	bool isIndexed = false;
	size_t	indexOffset = 0;
	size_t	indexCount = 0;



	PerObjectData perObjectData;

};
