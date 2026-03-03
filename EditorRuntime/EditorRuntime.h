#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Identify/CRIdentity.h"


//---------------------------------------------------------------------------------------------------------------------
/// CREditorRuntime
//---------------------------------------------------------------------------------------------------------------------
namespace CREditorRuntime
{
    bool InitializeRuntime();
    void ShutdownRuntime();

    void OnViewportMouseMove  ( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH );
    void OnViewportMouseButton( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, i32 Button, bool bPressed );
    void OnViewportKeyState   ( i32 VirtualKey, bool bPressed );
    void OnViewportMouseWheel ( i32 WheelDelta );
    void OnActorPicked        ( CRIdentity::id_t ActorId );

    CRIdentity::id_t PickActorOrBeginGizmoDrag( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH );

    void ApplyViewportCameraInput( f32 DeltaSeconds );
}
