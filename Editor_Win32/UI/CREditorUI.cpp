#include "CREditorUI.h"
#include "Engine.h"
#include "Extras/ImGUI/imgui.h"
#include "Source/World/CRWorld.h"


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
void CREditorUI::Initialize()
{
    CameraUI.SetCamera( GWorld->GetCamera() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw.
//---------------------------------------------------------------------------------------------------------------------
void CREditorUI::Draw()
{
    ImGui::Begin( "Camera UI" );
        CameraUI.Draw();
    ImGui::End();

    ImGui::Begin( "Logs" );
        LogUI.Draw();
    ImGui::End();
}
