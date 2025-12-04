#include "../../../../include/ui/include/core/ui_core_system.h"
#include "../../../../include/ui/include/core/ui_component.h"




namespace UI
{

    
    UICoreSystem::UICoreSystem(TheEngine::ECS::ECSEngine& ecsEngine, GPUBufferManager& gpuBufferManager) : m_uiShaderManager(), m_ecsEngine(ecsEngine), m_uiVertexBufferManagementSystem(gpuBufferManager)
    {

        m_ecsEngine.registerComponent<UIRenderMeshComponent>();
        m_ecsEngine.registerComponent<UIMaterialComponent>();
        m_ecsEngine.registerComponent<UIRectDimensionsComponent>();
       // m_ecsEngine.registerComponent<UI::UIRectBoundsComponent>();

        //m_ecsEngine.registerComponent<UITransformComponent>();

        m_ecsEngine.registerComponent<UITextComponent>();
        m_ecsEngine.registerComponent<UITextMeshComponent>();
        m_ecsEngine.registerComponent<UIGraphComponent>();
        m_ecsEngine.registerComponent<UIRootComponent>();
       // m_ecsEngine.registerComponent<UIHierarchyComponent>();










    }

   
    TheEngine::ECS::ECSEngine& UICoreSystem::getECSEngine()
    {
      
        return m_ecsEngine;

    }



    UIVertexBufferManagementSystem& UICoreSystem::getUIVertexBufferManagementSystem()
    {
       

        return m_uiVertexBufferManagementSystem;

    }

    UIFontManager& UICoreSystem::getUIFontManager()
    {


        return m_uiFontManager;
  
    }

    UIShaderManager& UICoreSystem::getUIShaderManager()
    {
        return m_uiShaderManager;
    }

}