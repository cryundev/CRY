#include "CRInputProcessorCamera.h"
#include "../EditorRuntime.h"


namespace
{
static constexpr i32 LeftMouseButton  = 0;
static constexpr i32 RightMouseButton = 1;


//---------------------------------------------------------------------------------------------------------------------
/// Try get current client viewport size.
//---------------------------------------------------------------------------------------------------------------------
bool _TryGetViewportSize( HWND hWnd, i32& OutViewportW, i32& OutViewportH )
{
    OutViewportW = 0;
    OutViewportH = 0;

    RECT clientRect = {};
    if ( !GetClientRect( hWnd, &clientRect ) ) return false;

    OutViewportW = clientRect.right  - clientRect.left;
    OutViewportH = clientRect.bottom - clientRect.top;

    return OutViewportW > 0 && OutViewportH > 0;
}

//---------------------------------------------------------------------------------------------------------------------
/// Extract signed client X coordinate from LPARAM.
//---------------------------------------------------------------------------------------------------------------------
i32 _GetPixelXFromLParam( LPARAM lParam )
{
    return (i32)(i16)LOWORD( lParam );
}

//---------------------------------------------------------------------------------------------------------------------
/// Extract signed client Y coordinate from LPARAM.
//---------------------------------------------------------------------------------------------------------------------
i32 _GetPixelYFromLParam( LPARAM lParam )
{
    return (i32)(i16)HIWORD( lParam );
}

//---------------------------------------------------------------------------------------------------------------------
/// Extract signed wheel delta from WPARAM.
//---------------------------------------------------------------------------------------------------------------------
i32 _GetWheelDeltaFromWParam( WPARAM wParam )
{
    return (i32)(i16)HIWORD( wParam );
}

//---------------------------------------------------------------------------------------------------------------------
/// Release movement key states.
//---------------------------------------------------------------------------------------------------------------------
void _ReleaseMovementKeys()
{
    CREditorRuntime::OnViewportKeyState( 'W', false );
    CREditorRuntime::OnViewportKeyState( 'A', false );
    CREditorRuntime::OnViewportKeyState( 'S', false );
    CREditorRuntime::OnViewportKeyState( 'D', false );
    CREditorRuntime::OnViewportKeyState( 'Q', false );
    CREditorRuntime::OnViewportKeyState( 'E', false );
}
}


namespace CREditorRuntimeWin32Input
{

//---------------------------------------------------------------------------------------------------------------------
/// Handle mouse-related window message.
//---------------------------------------------------------------------------------------------------------------------
void OnMouseMessage( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    if ( Message == WM_MOUSEWHEEL )
    {
        const i32 wheelDelta = _GetWheelDeltaFromWParam( wParam );
        if ( wheelDelta != 0 )
        {
            CREditorRuntime::OnViewportMouseWheel( wheelDelta );
        }
        return;
    }

    i32 viewportW = 0;
    i32 viewportH = 0;
    if ( !_TryGetViewportSize( hWnd, viewportW, viewportH ) ) return;

    const i32 pixelX = _GetPixelXFromLParam( lParam );
    const i32 pixelY = _GetPixelYFromLParam( lParam );

    switch ( Message )
    {
        case WM_MOUSEMOVE:   CREditorRuntime::OnViewportMouseMove  ( pixelX, pixelY, viewportW, viewportH ); break;
        case WM_LBUTTONDOWN: CREditorRuntime::OnViewportMouseButton( pixelX, pixelY, viewportW, viewportH, LeftMouseButton,  true  ); break;
        case WM_LBUTTONUP:   CREditorRuntime::OnViewportMouseButton( pixelX, pixelY, viewportW, viewportH, LeftMouseButton,  false ); break;
        case WM_RBUTTONDOWN: CREditorRuntime::OnViewportMouseButton( pixelX, pixelY, viewportW, viewportH, RightMouseButton, true  ); break;
        case WM_RBUTTONUP:   CREditorRuntime::OnViewportMouseButton( pixelX, pixelY, viewportW, viewportH, RightMouseButton, false ); break;
        default: break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Handle keyboard-related window message.
//---------------------------------------------------------------------------------------------------------------------
void OnKeyboardMessage( WPARAM wParam, LPARAM lParam, bool bPressed )
{
    if ( bPressed )
    {
        const bool bRepeatKey = ( (u64)lParam & ( 1ULL << 30 ) ) != 0;
        if ( bRepeatKey ) return;
    }

    CREditorRuntime::OnViewportKeyState( (i32)wParam, bPressed );
}

//---------------------------------------------------------------------------------------------------------------------
/// Release cached input states on focus loss.
//---------------------------------------------------------------------------------------------------------------------
void OnKillFocus( HWND hWnd )
{
    i32 viewportW = 1;
    i32 viewportH = 1;
   
    _TryGetViewportSize( hWnd, viewportW, viewportH );

    CREditorRuntime::OnViewportMouseButton( 0, 0, viewportW, viewportH, LeftMouseButton,  false );
    CREditorRuntime::OnViewportMouseButton( 0, 0, viewportW, viewportH, RightMouseButton, false );
    
    _ReleaseMovementKeys();
}


//---------------------------------------------------------------------------------------------------------------------
/// Tick.
//---------------------------------------------------------------------------------------------------------------------
void TickCamera( f32 DeltaTime )
{
    CREditorRuntime::ApplyViewportCameraInput( DeltaTime );
}
}
