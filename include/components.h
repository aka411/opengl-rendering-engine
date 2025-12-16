#pragma once

#include <vector>
#include "low-level/gpu_material_system.h"
#include "low-level/rendering_system_data_types.h"
#include <glm/glm.hpp> 
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../external/the-engine/the-engine/ecs/include/common_data_types.h"
#include "animation/animation_data_structures.h"
#include "animation/animation_data_structures.h"










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





/***ANIMATION RELATED**/

using BoneAnimationId = uint32_t;
using KeyframeAnimationId = uint32_t;




//COMBINE BONE AND KEYFRAME ANIMATION STRUCTS


struct AnimationStateComponent
{
	float timeElapsedInSeconds = 0.0f;
	std::string currentPlayingAnimationName;
	bool isPlayingAnimation = false;
};


struct AnimationComponent
{
	//we may need a local to entity mapping in here also
	KeyframeAnimationId id;
};


//all entites with bone attributes get this
struct BoneAnimationComponent
{
	BoneAnimationId id;//has cpu side and gpu side 
};










/**TO BE STORED IN CPU SIDE MANAGER ONLY AND GET IT BACK USING IDs**/


//only in root
//The name is misleading , but by "CPU" i meant that it is not 
// inside ECSEngine but instead it is stored in a separate animation manager.
struct AnimationCPUComponent
{
	//Mapping local Node Index to EntityId

	/*
	Why this mapping is not stored as a component in root entity ?
	Because gltf can have multiple roots in a single file,which can be loaded as separate root entities
	But all the data will be in global arrays which are referenced as indices , 
	so this can be shared by different roots,

	BUT THERE IS A PROBLEM : When we create copies of same character model the mapping being in here is not gonna work
	each copy needs its own mapping
	*/
	std::vector<TheEngine::ECS::EntityId> engineEntityIds;


	Engine::AnimationData animationData;
	std::unordered_map<std::string, Engine::Animation> animationsMap;

};




//to be stored in CPU side Manager and not in gpu
struct BoneAnimationCPUComponent
{
	//Data for Bone animation system to work with
	std::vector<glm::mat4> inverseBindMatrices;
	std::vector<int> jointIndices; //Indice into nodes, we need to map local index to entity id

	//upload this per frame
	std::vector<glm::mat4> jointMatrices;//WARNING : pre-reserve size
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
