#include "CRCameraUI.h"
#include "Extras/ImGUI/imgui.h"
#include <Source/Object/Camera/CRCamera.h>


//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRCameraUI::Draw()
{
    if ( ImGui::Button( "Reset camera transform" ) )
    {
        Camera->GetTransform()->SetLocation( 0.f, 0.f, 15.0f );
        Camera->GetTransform()->SetRotation( CRQuaternion::Identity );
    }
}
