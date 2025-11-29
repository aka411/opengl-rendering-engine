#pragma once
#include "../../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "ui_panel_builder.h"

namespace UI
{
    class UIBuilder
    {
    private:

        static TheEngine::ECS::ECSEngine* m_uiECSEngine; 

    public:
        UIBuilder(TheEngine::ECS::ECSEngine& ecsEngine);
        UIBuilder();

        static void setECSEngine(TheEngine::ECS::ECSEngine& ecsEngine);

        // Factory function for Panel
        static UIPanelBuilder createUIPanel();

        //static UIGraphBuilder createUIGraph();

        // Factory function for Text
        //UITextBuilder createUITextBox();

    };

}