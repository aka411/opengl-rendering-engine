#pragma once
#include <vector>
#include "../components.h"
#include "../low-level/rendering_system_data_types.h"
#include <unordered_map>


class SkeletalAnimationManager
{
	/*
	Owner of a ssbo which is managed such that
	we can use a offset and size to upload to specific area to allow
	reupload without changing offset

	*/
private:


	std::vector<BoneAnimationCPUComponent> m_boneAnimationCPUComponents;

	GPUBufferInfo m_jointMatrixSSBO;

	size_t m_currentByteOffset = 0;

	//Actually this is not id but a offset

	

public:


	SkeletalAnimationManager(GPUBufferManager& gpuBufferManager);//need gpubuffer manager

	BoneAnimationCPUComponent& getBoneAnimationCPUComponentFromId(const BoneAnimationId boneAnimationId);

	//The Id is actually a offset to a reserved area in ssbo
	BoneAnimationId storeBoneAnimationCPUComponent(BoneAnimationCPUComponent&& boneAnimationCPUComponent);
	BoneJointMatrixId uploadNewJointMatrixSetToSSBO(const std::vector<glm::mat4>& jointMatrixSet);


	void updateJointMatrixSetInSSBO(const BoneJointMatrixId boneJointMatrixId, const std::vector<glm::mat4>& jointMatrixSet);


	//The SSBO with all joint matrices
	GPUBufferInfo getJointMatrixSSBO();

};