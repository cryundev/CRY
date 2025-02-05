#include "CRLogUI.h"
#include "Extras/ImGUI/imgui.h"
#include <Source/Utility/Log/CRLog.h>


//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRLogUI::Draw()
{
    for ( const auto& log : GLog.GetLogs() )
    {
        ImGui::Text( log.c_str() );
    }
}
