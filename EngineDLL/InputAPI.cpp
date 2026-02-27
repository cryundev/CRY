#include "EngineDLL.h"
#include "../EditorRuntime/EditorRuntime.h"


//---------------------------------------------------------------------------------------------------------------------
/// OnViewportMouseMove
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void OnViewportMouseMove( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH )
{
    CREditorRuntime::OnViewportMouseMove( PixelX, PixelY, ViewportW, ViewportH );
}

//---------------------------------------------------------------------------------------------------------------------
/// OnViewportMouseButton
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void OnViewportMouseButton( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, i32 Button, bool bPressed )
{
    CREditorRuntime::OnViewportMouseButton( PixelX, PixelY, ViewportW, ViewportH, Button, bPressed );
}

//---------------------------------------------------------------------------------------------------------------------
/// OnViewportKeyState
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void OnViewportKeyState( i32 VirtualKey, bool bPressed )
{
    CREditorRuntime::OnViewportKeyState( VirtualKey, bPressed );
}

//---------------------------------------------------------------------------------------------------------------------
/// OnViewportMouseWheel
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API void OnViewportMouseWheel( i32 WheelDelta )
{
    CREditorRuntime::OnViewportMouseWheel( WheelDelta );
}

