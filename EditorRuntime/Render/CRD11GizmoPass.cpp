#include "CRD11GizmoPass.h"
#include "../Actor/CRGizmoSystem.h"


namespace
{
static constexpr f32 GizmoDefaultPixelSize = 96.0f;
}


//---------------------------------------------------------------------------------------------------------------------
/// Get gizmo overlay config.
//---------------------------------------------------------------------------------------------------------------------
CROverlayPassConfig CRD11GizmoPass::GetOverlayConfig() const
{
    CROverlayPassConfig config;
    config.DepthStateName     = "GizmoDepth";
    config.RasterStateName    = "GizmoCullNone";
    config.ShaderName         = "Gizmo";
    config.ConstantBufferName = "GizmoBuffer";
    config.LogPrefix          = "[CRD11GizmoPass]";
    config.DefaultColor       = CRVector4D( 1.0f, 1.0f, 0.0f, 1.0f );
    config.DefaultPixelSize   = GizmoDefaultPixelSize;

    return config;
}

//---------------------------------------------------------------------------------------------------------------------
/// Check whether gizmo overlay draw should run.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11GizmoPass::CanDrawOverlay() const
{
    return GGizmoSystem.IsVisible() && GGizmoSystem.GetGizmoActor() != nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw gizmo elements from gizmo system.
//---------------------------------------------------------------------------------------------------------------------
void CRD11GizmoPass::DrawOverlayElements()
{
    CRGizmoActor* gizmoActor = GGizmoSystem.GetGizmoActor();
    if ( !gizmoActor ) return;

    const CROverlayCameraParams cameraParams = BuildCameraParams();

    const u32 renderElementCount = gizmoActor->GetRenderElementCount();
    for ( u32 elementIndex = 0; elementIndex < renderElementCount; ++elementIndex )
    {
        CRGizmoRenderElement renderElement;
        if ( !gizmoActor->GetRenderElement( elementIndex, renderElement ) ) continue;

        DrawOverlayMesh( renderElement, cameraParams, GizmoDefaultPixelSize );
    }
}
