#include "../../../../include/ui/include/builder/ui_graph_builder.h"
#include "../../../../include/ui/include/core/ui_component.h"


namespace UI
{

    UIGraphBuilder::UIGraphBuilder(UICoreSystem& uiCoreSystem) : UIBaseBuilder(uiCoreSystem)
    {
        UIRenderMeshComponent uiRenderMeshComponent;
        UIGraphComponent uiGraphComponent;

        uiRenderMeshComponent.bufferType = UIBufferType::DYNAMIC;

        //add compopnent 
        m_ecsEngine.addComponentToEntity<UIRenderMeshComponent>(m_uiElement.getEntityId(), uiRenderMeshComponent);
        m_ecsEngine.addComponentToEntity<UIGraphComponent>(m_uiElement.getEntityId(), uiGraphComponent);



        m_ecsEngine.processBufferedCommands();

    }


    UIGraphBuilder& UIGraphBuilder::withPosition(glm::vec3 position)
    {

        UIBaseBuilder::withPosition(position);
        return *this;
    }


    //dimension of bounds
    UIGraphBuilder& UIGraphBuilder::withRectDimensions(const float width, const float height)
    {
        UIBaseBuilder::withRectDimensions(width, height);
        return *this;
    }


    UIGraphBuilder& UIGraphBuilder::withColour(glm::vec4 color)
    {
   
        ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_uiElement.getEntityId());
        UIGraphComponent* uiGraphComponent = entityChunkView.getComponent<UIGraphComponent>();

        assert(uiGraphComponent != nullptr && "UIMaterialComponent not found in entity");

        uiGraphComponent->color = color;



        return *this;
    }
    


}