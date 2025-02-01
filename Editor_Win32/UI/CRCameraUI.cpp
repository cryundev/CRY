#include "CRCameraUI.h"
#include "Extras/ImGUI/imgui.h"
#include <Source/Object/Camera/CRCamera.h>


//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CRCameraUI::Draw()
{
    if ( CameraPtr.expired() ) return;

    if ( ImGui::Button( "Reset camera transform" ) )
    {
        CameraPtr.lock()->Transform.SetLocation( 0.f, 0.f, 15.0f );
        CameraPtr.lock()->Transform.SetRotation( CRQuaternion::Identity );
    }
}
