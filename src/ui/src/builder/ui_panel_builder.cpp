#include "../../../../include/ui/include/builder/ui_panel_builder.h"
#include "../../../../include/ui/include/core/ui_component.h"




namespace UI
{


    UIPanelBuilder::UIPanelBuilder(UICoreSystem& uiCoreSystem) : UIBaseBuilder(uiCoreSystem)
    {

        UIMaterialComponent uiMaterialComponent;
        UIRenderMeshComponent uiRenderMeshComponent;

        m_ecsEngine.addComponentToEntity<UIMaterialComponent>(m_uiElement.getEntityId(), uiMaterialComponent);
        m_ecsEngine.addComponentToEntity<UIRenderMeshComponent>(m_uiElement.getEntityId(), uiRenderMeshComponent);

        m_ecsEngine.processBufferedCommands();
    }



    UIPanelBuilder& UIPanelBuilder::withColour(glm::vec4 color)
    {
  

        TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_uiElement.getEntityId());
        UIMaterialComponent* uiMaterialComponentPtr = entityChunkView.getComponent<UIMaterialComponent>();
        
        assert(uiMaterialComponentPtr != nullptr && "UIMaterialComponent not found in entity");

        uiMaterialComponentPtr->color = color;
        
        return *this;
    }



    UIPanelBuilder& UIPanelBuilder::withRoundConerRadius(float radius)
    {
        TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_uiElement.getEntityId());
        UIMaterialComponent* uiMaterialComponentPtr = entityChunkView.getComponent<UIMaterialComponent>();

        assert(uiMaterialComponentPtr != nullptr && "UIMaterialComponent not found in entity");

        uiMaterialComponentPtr->radius = radius;

        return *this;

    }



    UIPanelBuilder& UIPanelBuilder::withPosition(glm::vec3 position)
    {

		UIBaseBuilder::withPosition(position);
		return *this;
    }



    UIPanelBuilder& UIPanelBuilder::withRectDimensions(const float width, const float height)
    {
        UIBaseBuilder::withRectDimensions(width, height);
        return *this;
    }




}