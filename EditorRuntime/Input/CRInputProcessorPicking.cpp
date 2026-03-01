#include "CRInputProcessorPicking.h"
#include "../EditorRuntime.h"
#include "../Actor/CRAxisGizmoActor.h"
#include "../Actor/CRGizmoSystem.h"
#include "Source/Core/Math/CRRay.h"
#include "Source/Utility/UtilRay.h"


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

    if ( GGizmoSystem.IsVisible() )
    {
        CRGizmoActor* gizmoActor = GGizmoSystem.GetGizmoActor();
        if ( CRAxisGizmoActor* gizmo = dynamic_cast< CRAxisGizmoActor* >( gizmoActor ) )
        {
            f32 ndcX = ( (f32)pixelX / (f32)viewportW ) * 2.0f - 1.0f;
            f32 ndcY = 1.0f - ( (f32)pixelY / (f32)viewportH ) * 2.0f;

            CRRay ray;
            if ( UtilRay::TryCreateRayFromNDC( ndcX, ndcY, ray ) )
            {
                const ECRAxis hitAxis = gizmo->HitTestAxis( ray.Origin, ray.Direction );
                if ( hitAxis != ECRAxis::Max )
                {
                    // TODO: Phase 3 axis drag handling.
                    return;
                }
            }
        }
    }

    const CRIdentity::id_t pickedActorId = UtilRay::PickActorAtScreen( pixelX, pixelY, viewportW, viewportH );

    CREditorRuntime::OnActorPicked( pickedActorId );
}
}
