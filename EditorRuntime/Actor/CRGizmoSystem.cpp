#include "CRGizmoSystem.h"
#include "CRAxisGizmoActor.h"
#include "Engine.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Core/Math/CRRay.h"
#include "Source/Object/CRActor.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/Object/Component/CRTransformComponent.h"
#include "Source/Utility/Log/CRLog.h"
#include "Source/Utility/UtilPath.h"
#include "Source/World/CRWorld.h"
#include <filesystem>


CRGizmoSystem GGizmoSystem;


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Resolve gizmo arrow asset path.
//---------------------------------------------------------------------------------------------------------------------
CRPath _ResolveGizmoArrowAssetPath()
{
    return UtilPath::ResolveExistingEnginePath( "Asset/Gizmo/Arrow.cra" );
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Initialize.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::Initialize()
{
    Shutdown();

    const CRPath assetPath = _ResolveGizmoArrowAssetPath();
    if ( assetPath.empty() ) return false;

    CRPrimitiveAsset primitiveAsset;
    primitiveAsset.Load( assetPath );

    if ( primitiveAsset.VertexCount == 0 )
    {
        GLog.AddLog( "[CRGizmoSystem] Gizmo/Arrow.cra has no vertices. Gizmo rendering disabled." );
        return false;
    }

    if ( primitiveAsset.Positions.size() != primitiveAsset.VertexCount ||
         primitiveAsset.Normals  .size() != primitiveAsset.VertexCount ||
         primitiveAsset.UVs      .size() != primitiveAsset.VertexCount )
    {
        GLog.AddLog( "[CRGizmoSystem] Gizmo/Arrow.cra channel size mismatch. Gizmo rendering disabled." );
        return false;
    }

    if ( !GWorld )
    {
        GLog.AddLog( "[CRGizmoSystem] GWorld is not available. Gizmo actor not spawned." );
        return true;
    }

    GizmoActor = GWorld->SpawnActor< CRAxisGizmoActor >();
    if ( !GizmoActor )
    {
        GLog.AddLog( "[CRGizmoSystem] Failed to spawn CRAxisGizmoActor." );
        return true;
    }

    if ( !GizmoActor->InitializeGizmo( assetPath, primitiveAsset ) )
    {
        GLog.AddLog( "[CRGizmoSystem] Failed to initialize gizmo actor resources." );
    }

    bVisible = false;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Shutdown.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoSystem::Shutdown()
{
    if ( GizmoActor && GWorld )
    {
        GWorld->DespawnActor( GizmoActor->GetObjectId() );
        GizmoActor = nullptr;
    }

    bVisible = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set visibility.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoSystem::SetVisible( bool bInVisible )
{
    bVisible = bInVisible;

    if ( GizmoActor )
    {
        GizmoActor->SetAxisQueryEnabled( bInVisible );
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Is ready.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::IsReady() const
{
    if ( !GizmoActor ) return false;

    const u32 renderElementCount = GizmoActor->GetRenderElementCount();
    if ( renderElementCount == 0 ) return false;

    for ( u32 elementIndex = 0; elementIndex < renderElementCount; ++elementIndex )
    {
        CRGizmoRenderElement renderElement;
        if ( !GizmoActor->GetRenderElement( elementIndex, renderElement ) ) return false;
        if ( !renderElement.Mesh ) return false;
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set pivot.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoSystem::SetPivot( const CRVector& InPivot )
{
    if ( GizmoActor )
    {
        GizmoActor->SetPivot( InPivot );
    }

    if ( IsReady() )
    {
        bVisible = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Get pivot.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRGizmoSystem::GetPivot() const
{
    return GizmoActor ? GizmoActor->GetPivot() : CRVector::Zero;
}

//---------------------------------------------------------------------------------------------------------------------
/// Begin drag.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::BeginDrag( ECRAxis Axis, const CRRay& Ray )
{
    if ( Axis >= ECRAxis::Max ) return false;

    const CRVector axisDir = _GetAxisDirection( Axis );

    DragAxis = Axis;
    ActorStartLocation = GetPivot();

    if ( !_TryComputeAxisTFromRay( Ray, ActorStartLocation, axisDir, DragStartAxisT ) ) return false;

    bDragging = true;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Try begin drag from ray.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::TryBeginDragFromRay( const CRRay& Ray )
{
    if ( !IsVisible() ) return false;
    if ( !GizmoActor  ) return false;

    const ECRAxis hitAxis = GizmoActor->HitTestAxis( Ray.Origin, Ray.Direction );
    if ( hitAxis == ECRAxis::Max ) return false;

    return BeginDrag( hitAxis, Ray );
}

//---------------------------------------------------------------------------------------------------------------------
/// Update drag.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::UpdateDrag( const CRRay& Ray, CRVector& OutNewLocation )
{
    if ( !bDragging ) return false;

    const CRVector axisDir = _GetAxisDirection( DragAxis );

    f32 currentAxisT = 0.0f;
    if ( !_TryComputeAxisTFromRay( Ray, ActorStartLocation, axisDir, currentAxisT ) ) return false;

    const f32 projectedDelta = currentAxisT - DragStartAxisT;

    OutNewLocation = ActorStartLocation + axisDir * projectedDelta;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply drag from ray.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::ApplyDragFromRay( const CRRay& Ray, CRActor* TargetActor )
{
    CRVector newLocation;
    if ( !UpdateDrag( Ray, newLocation ) ) return false;

    if ( TargetActor )
    {
        if ( CRTransformComponent* transform = TargetActor->GetTransform() )
        {
            transform->SetLocation( newLocation );
        }
    }

    SetPivot( newLocation );

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// End drag.
//---------------------------------------------------------------------------------------------------------------------
void CRGizmoSystem::EndDrag()
{
    bDragging          = false;
    DragAxis           = ECRAxis::Max;
    ActorStartLocation = CRVector::Zero;
    DragStartAxisT     = 0.0f;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get axis direction.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRGizmoSystem::_GetAxisDirection( ECRAxis Axis )
{
    switch ( Axis )
    {
        case ECRAxis::X: return CRVector::Right;
        case ECRAxis::Y: return CRVector::Up;
        case ECRAxis::Z: return CRVector::Forward;
        default:         return CRVector::Zero;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Try compute axis parameter from ray.
//---------------------------------------------------------------------------------------------------------------------
bool CRGizmoSystem::_TryComputeAxisTFromRay( const CRRay& Ray, const CRVector& AxisOrigin, const CRVector& AxisDir, f32& OutAxisT ) const
{
    const f32 a = AxisDir.Dot( AxisDir );
    const f32 b = AxisDir.Dot( Ray.Direction );
    const f32 c = Ray.Direction.Dot( Ray.Direction );

    const CRVector w0 = AxisOrigin - Ray.Origin;
    const f32 d = AxisDir.Dot( w0 );
    const f32 e = Ray.Direction.Dot( w0 );

    const f32 denom = a * c - b * b;
    if ( CRMath::Abs( denom ) >= CRMath::Epsilon )
    {
        OutAxisT = ( b * e - c * d ) / denom;
        return true;
    }

    // Fallback for parallel axis and ray.
    if ( !GWorld ) return false;

    CRCamera* camera = GWorld->GetCamera();
    if ( !camera ) return false;

    const CRVector planeNormal = camera->GetLookDirection();
    const f32 planeDenom = planeNormal.Dot( Ray.Direction );
    if ( CRMath::Abs( planeDenom ) < CRMath::Epsilon ) return false;

    const f32 rayT = planeNormal.Dot( AxisOrigin - Ray.Origin ) / planeDenom;
    if ( rayT < 0.0f ) return false;

    const CRVector hitPoint = Ray.Origin + Ray.Direction * rayT;
    OutAxisT = ( hitPoint - AxisOrigin ).Dot( AxisDir );

    return true;
}

