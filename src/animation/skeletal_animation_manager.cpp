#include "../../include/animation/skeletal_animation_manager.h"




BoneJointMatrixId SkeletalAnimationManager::uploadNewJointMatrixSetToSSBO(const std::vector<glm::mat4>& jointMatrixSet)
{


    const size_t bytesToCopy = jointMatrixSet.size() * sizeof(jointMatrixSet[0]);


    assert(m_jointMatrixSSBO.size > (bytesToCopy + m_currentByteOffset));

    
    

    
    std::byte* absoluteByteOffset = reinterpret_cast<std::byte*>(m_jointMatrixSSBO.mappedPtr) + m_currentByteOffset ;

    memcpy(absoluteByteOffset, jointMatrixSet.data(), bytesToCopy);


    const size_t lastByteOffset = m_currentByteOffset;

    m_currentByteOffset += bytesToCopy;

    assert(lastByteOffset % sizeof(jointMatrixSet[0]) == 0);

    return (lastByteOffset / sizeof(jointMatrixSet[0]));
}




SkeletalAnimationManager::SkeletalAnimationManager(GPUBufferManager& gpuBufferManager)
{

    m_jointMatrixSSBO = gpuBufferManager.createMappedSSBO(20*1024*1024,nullptr);//20 MiB
   
}

BoneAnimationCPUComponent& SkeletalAnimationManager::getBoneAnimationCPUComponentFromId(const BoneAnimationId boneAnimationId)
{

  
    //Maybe i should use shared pointer or unique pointer to store the actually data 
    //my belief is that map does not move old data when adding new 
    // ones like vector does when it needs more space to add 
    //new elements
    assert(m_boneAnimationCPUComponents.size() > boneAnimationId);
    
    return m_boneAnimationCPUComponents[boneAnimationId];

}



BoneAnimationId SkeletalAnimationManager::storeBoneAnimationCPUComponent(BoneAnimationCPUComponent&& boneAnimationCPUComponent)
{

    assert(boneAnimationCPUComponent.inverseBindMatrices.size() == boneAnimationCPUComponent.jointIndices.size());

    const BoneAnimationId boneAnimationId = m_boneAnimationCPUComponents.size();

    m_boneAnimationCPUComponents.push_back(std::move(boneAnimationCPUComponent));//creating copy is ok 


    return boneAnimationId;
}


void SkeletalAnimationManager::updateJointMatrixSetInSSBO(const BoneJointMatrixId boneJointMatrixId, const std::vector<glm::mat4>& jointMatrixSet)
{

    //May need book keeping to track and validate its writing to correct size 

    const size_t bytesToCopy = jointMatrixSet.size() * sizeof(jointMatrixSet[0]);

    std::byte* absoluteOffsetAddressPtr = reinterpret_cast<std::byte*>(m_jointMatrixSSBO.mappedPtr) + (boneJointMatrixId * sizeof(jointMatrixSet[0]));

    memcpy(absoluteOffsetAddressPtr, jointMatrixSet.data(), bytesToCopy);

}

GPUBufferInfo SkeletalAnimationManager::getJointMatrixSSBO()
{
    return m_jointMatrixSSBO;
}