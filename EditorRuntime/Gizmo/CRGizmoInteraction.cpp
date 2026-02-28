#include "CRGizmoInteraction.h"
#include "Engine.h"
#include "Source/Object/CRActor.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/Object/Component/CRTransformComponent.h"
#include "Source/World/CRWorld.h"


namespace
{
static constexpr f32 GizmoAxisLength              = 1.0f;
static constexpr f32 GizmoAxisHitRadiusMin        = 0.10f;
static constexpr f32 GizmoAxisHitRadiusMax        = 1.25f;
static constexpr f32 GizmoAxisHitRadiusScaleRatio = 0.035f;


//---------------------------------------------------------------------------------------------------------------------
/// Get selected actor transform.
//---------------------------------------------------------------------------------------------------------------------
CRTransformComponent* _TryGetSelectedActorTransform( CRIdentity::id_t SelectedActorId )
{
    if ( !GWorld ) return nullptr;
    if ( !CRIdentity::IsValid( SelectedActorId ) ) return nullptr;

    CRActor* selectedActor = GWorld->GetActor( CRObjectId( SelectedActorId ) );
    if ( !selectedActor ) return nullptr;

    return selectedActor->GetTransform();
}

//---------------------------------------------------------------------------------------------------------------------
/// Get gizmo axis direction.
//---------------------------------------------------------------------------------------------------------------------
CRVector _GetGizmoAxisDirection( ECRGizmoAxis Axis )
{
    switch ( Axis )
    {
        case ECRGizmoAxis::X: return CRVector::Right;
        case ECRGizmoAxis::Y: return CRVector::Up;
        case ECRGizmoAxis::Z: return CRVector::Forward;
        default: break;
    }

    return CRVector::Right;
}

//---------------------------------------------------------------------------------------------------------------------
/// Intersect ray and plane.
//---------------------------------------------------------------------------------------------------------------------
bool _TryIntersectRayPlane( const CRRay& Ray, const CRVector& PlanePoint, const CRVector& PlaneNormal, CRVector& OutHitPoint )
{
    const f32 denominator = CRVector::Dot( PlaneNormal, Ray.Direction );
    if ( CRMath::IsNearlyZero( denominator ) ) return false;

    const f32 t = CRVector::Dot( PlanePoint - Ray.Origin, PlaneNormal ) / denominator;
    if ( t < 0.0f ) return false;

    OutHitPoint = Ray.Origin + Ray.Direction * t;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Try hit-test gizmo axis with ray.
//---------------------------------------------------------------------------------------------------------------------
bool _TryHitTestGizmoAxis( const CRRay& Ray, ECRGizmoAxis& OutAxis, CRVector& OutAxisHitPoint )
{
    if ( !GGizmoSystem.IsVisible() ) return false;

    const CRVector& pivot = GGizmoSystem.GetPivot();

    f32 cameraDistance = ( pivot - Ray.Origin ).Length();
    f32 hitRadius = cameraDistance * GizmoAxisHitRadiusScaleRatio;
    hitRadius = CRMath::Clamp( hitRadius, GizmoAxisHitRadiusMin, GizmoAxisHitRadiusMax );

    bool bFound = false;
    f32 bestRayT = CRMath::f32_max;

    for ( u32 axisIndex = 0; axisIndex < (u32)ECRGizmoAxis::Max; ++axisIndex )
    {
        const ECRGizmoAxis axis = (ECRGizmoAxis)axisIndex;

        CRVector axisDir = _GetGizmoAxisDirection( axis );
        axisDir.Normalize();

        // Ray P( t ) = O + tD
        // Axis Q( s ) = Pivot + sA
        const CRVector& O = Ray.Origin;
        const CRVector& D = Ray.Direction;
        const CRVector& P = pivot;
        const CRVector& A = axisDir;

        const CRVector w0 = O - P;

        const f32 a = CRVector::Dot( D, D );
        const f32 b = CRVector::Dot( D, A );
        const f32 c = CRVector::Dot( A, A );
        const f32 d = CRVector::Dot( D, w0 );
        const f32 e = CRVector::Dot( A, w0 );

        const f32 denominator = a * c - b * b;
        if ( CRMath::IsNearlyZero( denominator ) ) continue;

        f32 rayT  = ( b * e - c * d ) / denominator;
        f32 axisS = ( a * e - b * d ) / denominator;

        if ( rayT < 0.0f ) continue;
        if ( axisS < 0.0f || axisS > GizmoAxisLength ) continue;

        const CRVector rayPoint  = O + D * rayT;
        const CRVector axisPoint = P + A * axisS;

        const f32 distance = ( rayPoint - axisPoint ).Length();
        if ( distance > hitRadius ) continue;

        if ( rayT < bestRayT )
        {
            bestRayT = rayT;
            OutAxis = axis;
            OutAxisHitPoint = axisPoint;
            bFound = true;
        }
    }

    return bFound;
}
}


namespace CREditorRuntimeGizmo
{
//---------------------------------------------------------------------------------------------------------------------
/// Try begin gizmo drag.
//---------------------------------------------------------------------------------------------------------------------
bool TryBeginDrag( CRGizmoDragState& InOutDragState, const CRGizmoInteractionContext& Context )
{
    if ( !Context.bHasMouseRay ) return false;
    if ( !GGizmoSystem.IsVisible() ) return false;

    CRTransformComponent* selectedTransform = _TryGetSelectedActorTransform( Context.SelectedActorId );
    if ( !selectedTransform ) return false;

    ECRGizmoAxis axis = ECRGizmoAxis::X;
    CRVector axisHitPoint = CRVector::Zero;

    if ( !_TryHitTestGizmoAxis( Context.MouseRay, axis, axisHitPoint ) ) return false;

    InOutDragState.bDragging               = true;
    InOutDragState.ActiveAxis              = axis;
    InOutDragState.DragStartActorLocation  = selectedTransform->GetLocation();
    InOutDragState.DragStartHitPoint       = axisHitPoint;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// End gizmo drag.
//---------------------------------------------------------------------------------------------------------------------
void EndDrag( CRGizmoDragState& InOutDragState )
{
    InOutDragState.bDragging = false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply gizmo drag.
//---------------------------------------------------------------------------------------------------------------------
void ApplyDrag( CRGizmoDragState& InOutDragState, const CRGizmoInteractionContext& Context )
{
    if ( !InOutDragState.bDragging ) return;
    if ( !Context.bHasMouseRay ) return;

    CRTransformComponent* selectedTransform = _TryGetSelectedActorTransform( Context.SelectedActorId );
    if ( !selectedTransform )
    {
        EndDrag( InOutDragState );
        return;
    }

    const CRVector& pivot = GGizmoSystem.GetPivot();

    CRVector axisDir = _GetGizmoAxisDirection( InOutDragState.ActiveAxis );
    axisDir.Normalize();

    if ( !GWorld || !GWorld->GetCamera() || !GWorld->GetCamera()->GetTransform() ) return;

    CRTransformComponent* cameraTransform = GWorld->GetCamera()->GetTransform();

    CRVector cameraForward = cameraTransform->GetForward();
    cameraForward.Normalize();

    CRVector planeNormal = CRVector::Cross( axisDir, cameraForward );
    if ( planeNormal.LengthSquared() <= CRMath::Epsilon )
    {
        CRVector cameraUp = cameraTransform->GetUp();
        planeNormal = CRVector::Cross( axisDir, cameraUp );

        if ( planeNormal.LengthSquared() <= CRMath::Epsilon )
        {
            CRVector cameraRight = cameraTransform->GetRight();
            planeNormal = CRVector::Cross( axisDir, cameraRight );
        }
    }

    if ( planeNormal.LengthSquared() <= CRMath::Epsilon ) return;
    planeNormal.Normalize();

    CRVector currentHitPoint = CRVector::Zero;
    if ( !_TryIntersectRayPlane( Context.MouseRay, pivot, planeNormal, currentHitPoint ) ) return;

    const CRVector dragDelta = currentHitPoint - InOutDragState.DragStartHitPoint;
    const f32 axisDelta = CRVector::Dot( dragDelta, axisDir );

    selectedTransform->SetLocation( InOutDragState.DragStartActorLocation + axisDir * axisDelta );
}
}
