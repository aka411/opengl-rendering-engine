#pragma once
#include "ecs.h"
#include "../low-level/gpu_texture_manager.h"
#include "../low-level/gpu_material_system.h"
#include "../low-level/world_vertex_buffer_management_system.h"
#include "../model-loading/gltf_intermediate_mapper.h"
#include "../animation/animation_system.h"




class EngineLoader
{

private:
	GPUTextureManager& m_gpuTextureManager;
	GPUMaterialSystem& m_gpuMaterialSystem;

	WorldVertexBufferManagementSystem& m_worldVertexBufferManagementSystem;

	ECS::ECSEngine& m_ecsEngine;


	Engine::GLTFIntermediateMapper m_gltfIntermediateMapper;



	Engine::AnimationSystem& m_animationSystem;


	tinygltf::Model loadGLTFModel(const std::string& pathToModel);


public:
	//ecs , texture , material
	EngineLoader(ECS::ECSEngine& ecsEngine ,GPUTextureManager& gpuTextureManager, GPUMaterialSystem& gpuMaterialSystem , WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, Engine::AnimationSystem& animationSystem);

	ECS::EntityId createRootEntity(const std::string& pathToModel);



};