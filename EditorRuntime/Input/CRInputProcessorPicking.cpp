#include "CRInputProcessorPicking.h"
#include "../EditorRuntime.h"


namespace CREditorRuntimeWin32Input
{
//---------------------------------------------------------------------------------------------------------------------
/// Handle mouse message.
//---------------------------------------------------------------------------------------------------------------------
void OnPickingMouseMessage( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
    (void)wParam;

    if ( Message != WM_LBUTTONDOWN ) return;

    RECT clientRect = {};
    if ( !GetClientRect( hWnd, &clientRect ) ) return;

    const i32 viewportW = clientRect.right - clientRect.left;
    const i32 viewportH = clientRect.bottom - clientRect.top;
    if ( viewportW <= 0 || viewportH <= 0 ) return;

    const i32 pixelX = (i32)(i16)LOWORD( lParam );
    const i32 pixelY = (i32)(i16)HIWORD( lParam );

    CREditorRuntime::PickActorOrBeginGizmoDrag( pixelX, pixelY, viewportW, viewportH );
}
}
