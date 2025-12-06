#include "../../include/animation/skeletal_animation_manager.h"




BoneAnimationId SkeletalAnimationManager::uploadToJointMatrixSSBO(const BoneAnimationCPUComponent& boneAnimationCPUComponent)
{


    const size_t bytesToCopy = boneAnimationCPUComponent.jointMatrices.size() * sizeof(boneAnimationCPUComponent.jointMatrices[0]);


    assert(m_jointMatrixSSBO.size > (bytesToCopy + m_currentByteOffset));

    
    

    
    std::byte* absoluteByteOffset = reinterpret_cast<std::byte*>(m_jointMatrixSSBO.mappedPtr) + m_currentByteOffset ;

    memcpy(absoluteByteOffset, boneAnimationCPUComponent.jointMatrices.data(), bytesToCopy);


    const size_t lastByteOffset = m_currentByteOffset;

    m_currentByteOffset += bytesToCopy;


    return (lastByteOffset / sizeof(boneAnimationCPUComponent.jointMatrices[0]));
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

    
    return  m_boneIdToBoneAnimationCPUComponents.at(boneAnimationId);

}



BoneAnimationId SkeletalAnimationManager::storeBoneAnimationCPUComponent(BoneAnimationCPUComponent&& boneAnimationCPUComponent)
{

    assert(boneAnimationCPUComponent.jointMatrices.size() == boneAnimationCPUComponent.jointIndices.size());

    const BoneAnimationId boneAnimationId = uploadToJointMatrixSSBO(boneAnimationCPUComponent);

    m_boneIdToBoneAnimationCPUComponents[boneAnimationId] = std::move(boneAnimationCPUComponent);//creating copy is ok 


    return boneAnimationId;
}


void SkeletalAnimationManager::updateJointMatrixSet(const BoneAnimationId boneAnimationId, const BoneAnimationCPUComponent& boneAnimationCPUComponent)
{

    

    const size_t bytesToCopy = boneAnimationCPUComponent.jointMatrices.size() * sizeof(boneAnimationCPUComponent.jointMatrices[0]);

    memcpy(m_jointMatrixSSBO.mappedPtr, boneAnimationCPUComponent.jointMatrices.data(), bytesToCopy);

}

GPUBufferInfo SkeletalAnimationManager::getJointMatrixSSBO()
{
    return m_jointMatrixSSBO;
}