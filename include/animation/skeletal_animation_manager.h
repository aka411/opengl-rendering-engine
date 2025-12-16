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

	//std::vector<BoneAnimationCPUComponent> m_boneAnimationCPUComponents;
	//std::vector<size_t> idToJointMatrixSetSize;// A book keeping to not allow overwrite beyond the alloacted area
	
	//owner of BoneAnimationCPUComponent , maybe i should use unique pointer
	std::unordered_map<BoneAnimationId, BoneAnimationCPUComponent> m_boneIdToBoneAnimationCPUComponents;
	
	GPUBufferInfo m_jointMatrixSSBO;

	size_t m_currentByteOffset = 0;

	//Actually this is not id but a offset
	BoneAnimationId uploadToJointMatrixSSBO(const BoneAnimationCPUComponent& boneAnimationCPUComponent);

public:


	SkeletalAnimationManager(GPUBufferManager& gpuBufferManager);//need gpubuffer manager

	BoneAnimationCPUComponent& getBoneAnimationCPUComponentFromId(const BoneAnimationId boneAnimationId);

	//The Id is actually a offset to a reserved area in ssbo
	BoneAnimationId storeBoneAnimationCPUComponent(BoneAnimationCPUComponent&& boneAnimationCPUComponent);

	void updateJointMatrixSet(const BoneAnimationId boneAnimationId,const BoneAnimationCPUComponent& boneAnimationCPUComponent);
	

	//The SSBO with all joint matrices
	GPUBufferInfo getJointMatrixSSBO();

};