#include "CRD11ActorMarkerPass.h"
#include "../Actor/CRActorMarkerSystem.h"


namespace
{
static constexpr f32 MarkerDefaultPixelSize = 72.0f;
}


//---------------------------------------------------------------------------------------------------------------------
/// Get actor marker overlay config.
//---------------------------------------------------------------------------------------------------------------------
CROverlayPassConfig CRD11ActorMarkerPass::GetOverlayConfig() const
{
    CROverlayPassConfig config;
    config.DepthStateName     = "ActorMarkerDepth";
    config.RasterStateName    = "ActorMarkerCullNone";
    config.ShaderName         = "ActorMarker";
    config.ConstantBufferName = "ActorMarkerBuffer";
    config.LogPrefix          = "[CRD11ActorMarkerPass]";
    config.DefaultColor       = CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
    config.DefaultPixelSize   = MarkerDefaultPixelSize;

    return config;
}

//---------------------------------------------------------------------------------------------------------------------
/// Check whether actor marker overlay draw should run.
//---------------------------------------------------------------------------------------------------------------------
bool CRD11ActorMarkerPass::CanDrawOverlay() const
{
    return GActorMarkerSystem.IsVisible();
}

//---------------------------------------------------------------------------------------------------------------------
/// Draw marker elements from actor marker system.
//---------------------------------------------------------------------------------------------------------------------
void CRD11ActorMarkerPass::DrawOverlayElements()
{
    CRArray< CRGizmoRenderElement > markerElements;
    GActorMarkerSystem.GatherRenderElements( markerElements );
    if ( markerElements.empty() ) return;

    const CROverlayCameraParams cameraParams = BuildCameraParams();

    for ( const CRGizmoRenderElement& renderElement : markerElements )
    {
        DrawOverlayMesh( renderElement, cameraParams, MarkerDefaultPixelSize );
    }
}
