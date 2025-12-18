#pragma once

#include "ui_panel_builder.h"
#include "ui_graph_builder.h"
#include "ui_text_label_builder.h"
#include "ui_window_builder.h"

namespace UI
{
    class UIBuilder
    {
    private:
        
        ECS::ECSEngine& m_ecsEngine;

        UICoreSystem& m_uiCoreSystem;
  

    public:

        UIBuilder(UICoreSystem& uiCoreSystem);

    
        UIWindowBuilder createUIWindow();
    
        // Factory function for Panel
        UIPanelBuilder createUIPanel();

        UIGraphBuilder createUIGraph();

        // Factory function for Text
        UITextLabelBuilder createUITextLabel();

    };

}