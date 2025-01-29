#include "CRCameraUI.h"
#include "Core/ImGUI/imgui.h"
#include "Object/Camera/CRCamera.h"


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
