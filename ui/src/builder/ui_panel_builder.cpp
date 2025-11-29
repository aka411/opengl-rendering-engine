
#include <glm/ext/vector_float3.hpp>
#include "../../include/ui_component.h"
#include "../../include/builder/ui_panel_builder.h"

namespace UI
{

    UIPanelBuilder::UIPanelBuilder(UIEntity enityId) :UIBaseBuilder(enityId)
    {

	}


    UIPanelBuilder UIPanelBuilder::createUIPanel()
    {

        UIEntity enityId = m_uiECSEngine->createEntity();
		

        return UIPanelBuilder(enityId);
    }




    UIPanelBuilder& UIPanelBuilder::withColour(glm::vec4 color)
    {
        /*
        TheEngine::ECS::EntityChunkView entityChunkView = m_uiECSEngine->getEntityChunkView(m_entityId);
        UIMaterialComponent* uiMaterialComponentPtr = entityChunkView.getComponent<UIMaterialComponent>();
        
        if(uiMaterialComponentPtr)
        {
            uiMaterialComponentPtr->color = color;
            return *this;
		}

        */
        UIMaterialComponent uiMaterialComponent;
		uiMaterialComponent.color = color;

        UIRenderMeshComponent uiRenderMeshComponent;

        m_uiECSEngine->addComponentToEntity<UIMaterialComponent>(m_entityId, uiMaterialComponent);
        m_uiECSEngine->addComponentToEntity<UIRenderMeshComponent>(m_entityId, uiRenderMeshComponent);
        return *this;
    }



    UIPanelBuilder& UIPanelBuilder::withRoundConerRadius(float radius)
    {
        /*
        TheEngine::ECS::EntityChunkView entityChunkView = m_uiECSEngine->getEntityChunkView(m_entityId);
        UIMaterialComponent* uiMaterialComponentPtr = entityChunkView.getComponent<UIMaterialComponent>();

		UIRenderMeshComponent* uiRenderMeshComponentPtr = entityChunkView.getComponent<UIRenderMeshComponent>();

        if (uiMaterialComponentPtr)
        {
            uiMaterialComponentPtr->radius = radius;
            return *this;
		}
        
        */
        UIMaterialComponent uiMaterialComponent;
		uiMaterialComponent.radius = radius;

        m_uiECSEngine->addComponentToEntity<UIMaterialComponent>(m_entityId, uiMaterialComponent);

		UIRenderMeshComponent uiRenderMeshComponent;
		//uiRenderMeshComponent.meshType = UIRenderMeshComponent::MeshType::RECTANGLE;

		m_uiECSEngine->addComponentToEntity<UIRenderMeshComponent>(m_entityId, uiRenderMeshComponent);



        return *this;

    }


    
    UIPanelBuilder& UIPanelBuilder::withPosition(glm::vec3 position)
    {

		UIBaseBuilder::withPosition(position);
		return *this;
    }
   /*
    PanelUIBuilder& PanelUIBuilder::withRectBounds(glm::vec3 topLeft, glm::vec3 bottomRigth)
    {

		BaseUIBuilder::withRectBounds(topLeft, bottomRigth);
		return *this;

    }
  */

    UIPanelBuilder& UIPanelBuilder::withRectDimensions(const float width, const float height)
    {
        UIBaseBuilder::withRectDimensions(width, height);
        return *this;
    }




}