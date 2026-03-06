#include "CRAxisGizmoActor.h"
#include "Engine.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Core/Math/CRAABB.h"
#include "Source/Core/Math/CRRay.h"
#include "Source/Object/Camera/CRCamera.h"
#include "Source/RHI/CRRHI.h"
#include "Source/RHI/ICRRHIRenderer.h"
#include "Source/World/CRWorld.h"


namespace
{
static constexpr f32 GizmoPixelSize     = 96.0f;
static constexpr f32 GizmoHitProxyScale = 1.0f;

//---------------------------------------------------------------------------------------------------------------------
/// Get axis rotation.
//---------------------------------------------------------------------------------------------------------------------
CRMatrix _GetAxisRotation( ECRAxis Axis )
{
    switch ( Axis )
    {
        case ECRAxis::X: return CRMatrix::Identity;
        case ECRAxis::Y: return CRMatrix::CreateFromAxisAngle( CRVector( 0.f,  0.f, 1.f ), DirectX::XM_PIDIV2 );
        case ECRAxis::Z: return CRMatrix::CreateFromAxisAngle( CRVector( 0.f, -1.f, 0.f ), DirectX::XM_PIDIV2 );
        default:         return CRMatrix::Identity;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Build rotated axis bounds preset.
//---------------------------------------------------------------------------------------------------------------------
void _BuildRotatedAABBPreset( const CRAABB& SourceBounds, ECRAxis Axis, CRVector& OutCenter, CRVector& OutHalfExtents )
{
    const CRMatrix rotation = _GetAxisRotation( Axis );

    CRVector rotatedMin( CRMath::f32_max, CRMath::f32_max, CRMath::f32_max );
    CRVector rotatedMax( CRMath::f32_min, CRMath::f32_min, CRMath::f32_min );

    for ( u32 cornerIndex = 0; cornerIndex < 8; ++cornerIndex )
    {
        const f32 x = ( cornerIndex & 1 ) ? SourceBounds.Max.x : SourceBounds.Min.x;
        const f32 y = ( cornerIndex & 2 ) ? SourceBounds.Max.y : SourceBounds.Min.y;
        const f32 z = ( cornerIndex & 4 ) ? SourceBounds.Max.z : SourceBounds.Min.z;

        const CRVector rotated = CRVector::Transform( CRVector( x, y, z ), rotation );

        rotatedMin.x = CRMath::Min( rotatedMin.x, rotated.x );
        rotatedMin.y = CRMath::Min( rotatedMin.y, rotated.y );
        rotatedMin.z = CRMath::Min( rotatedMin.z, rotated.z );

        rotatedMax.x = CRMath::Max( rotatedMax.x, rotated.x );
        rotatedMax.y = CRMath::Max( rotatedMax.y, rotated.y );
        rotatedMax.z = CRMath::Max( rotatedMax.z, rotated.z );
    }

    OutCenter      = ( rotatedMin + rotatedMax ) * 0.5f;
    OutHalfExtents = ( rotatedMax - rotatedMin ) * 0.5f;
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Initialize gizmo actor.
//---------------------------------------------------------------------------------------------------------------------
bool CRAxisGizmoActor::InitializeGizmo( const CRString& AssetPath, const CRPrimitiveAsset& PrimitiveAsset )
{
    bool bInitialized = true;

    for ( u32 axisIndex = 0; axisIndex < (u32)ECRAxis::Max; ++axisIndex )
    {
        CRPrimitiveComponent* primitive = Add< CRPrimitiveComponent >();
        if ( !primitive )
        {
            bInitialized = false;
            continue;
        }

        primitive->SetRenderEnabled( false );
        primitive->LoadAsset( AssetPath );

        if ( !primitive->GetMesh() )
        {
            bInitialized = false;
            continue;
        }
    }

    const CRAABB localBounds = PrimitiveAsset.CalculateBounds();
    if ( !localBounds.IsValid() ) return bInitialized;

    for ( u32 axisIndex = 0; axisIndex < (u32)ECRAxis::Max; ++axisIndex )
    {
        CRVector presetCenter      = CRVector::Zero;
        CRVector presetHalfExtents = CRVector::Zero;

        _BuildRotatedAABBPreset( localBounds, (ECRAxis)axisIndex, presetCenter, presetHalfExtents );

        CRCollisionComponent* collision = Add< CRCollisionComponent >();
        if ( !collision )
        {
            bInitialized = false;
            continue;
        }

        collision->SetLocalCenter     ( presetCenter      * GizmoHitProxyScale );
        collision->SetLocalHalfExtents( presetHalfExtents * GizmoHitProxyScale );
        collision->SetQueryEnabled    ( false );
    }

    return bInitialized;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get render element.
//---------------------------------------------------------------------------------------------------------------------
bool CRAxisGizmoActor::GetRenderElement( u32 ElementIndex, CRGizmoRenderElement& OutRenderElement ) const
{
    const ECRAxis axis = (ECRAxis)ElementIndex;
    CRPrimitiveComponent* primitive = GetAxisPrimitive( axis );
    if ( !primitive ) return false;

    ICRRHIMeshSPtr mesh = primitive->GetMesh();
    if ( !mesh ) return false;

    OutRenderElement.Mesh      = mesh;
    OutRenderElement.Transform = _GetAxisTransform( axis );
    OutRenderElement.Color     = _GetAxisColor( axis );
    OutRenderElement.Pivot     = GetPivot();
    OutRenderElement.PixelSize = GizmoPixelSize;
    OutRenderElement.Type      = (f32)ElementIndex;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get axis primitive component.
//---------------------------------------------------------------------------------------------------------------------
CRPrimitiveComponent* CRAxisGizmoActor::GetAxisPrimitive( ECRAxis Axis ) const
{
    const u32 axisIndex = (u32)Axis;
    if ( axisIndex >= (u32)ECRAxis::Max ) return nullptr;

    CRArray< CRPrimitiveComponent* > primitives = GetComponents< CRPrimitiveComponent >();
    if ( axisIndex >= primitives.size() ) return nullptr;

    return primitives[ axisIndex ];
}

//---------------------------------------------------------------------------------------------------------------------
/// Get axis collision component.
//---------------------------------------------------------------------------------------------------------------------
CRCollisionComponent* CRAxisGizmoActor::_GetAxisCollision( ECRAxis Axis ) const
{
    const u32 axisIndex = (u32)Axis;
    if ( axisIndex >= (u32)ECRAxis::Max ) return nullptr;

    CRArray< CRCollisionComponent* > collisions = GetComponents< CRCollisionComponent >();
    if ( axisIndex >= collisions.size() ) return nullptr;

    return collisions[ axisIndex ];
}

//---------------------------------------------------------------------------------------------------------------------
/// Test axis hit.
//---------------------------------------------------------------------------------------------------------------------
ECRAxis CRAxisGizmoActor::HitTestAxis( const CRVector& RayOrigin, const CRVector& RayDir ) const
{
    CRRay ray;
    ray.Origin    = RayOrigin;
    ray.Direction = RayDir;

    ECRAxis hitAxis  = ECRAxis::Max;
    f32     nearestT = CRMath::f32_max;

    const f32 hitProxyScale = _ComputeGizmoWorldScale() * GizmoHitProxyScale;
    const CRVector hitProxyBoundsScale( hitProxyScale, hitProxyScale, hitProxyScale );

    for ( u32 axisIndex = 0; axisIndex < (u32)ECRAxis::Max; ++axisIndex )
    {
        const CRCollisionComponent* collision = _GetAxisCollision( (ECRAxis)axisIndex );
        if ( !collision || !collision->IsQueryEnabled() ) continue;

        const CRAABB worldBounds = collision->CalculateWorldBounds( hitProxyBoundsScale );
        f32 outT = 0.0f;
        if ( worldBounds.Intersects( ray, outT ) && outT < nearestT )
        {
            nearestT = outT;
            hitAxis  = (ECRAxis)axisIndex;
        }
    }

    return hitAxis;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set axis query enabled.
//---------------------------------------------------------------------------------------------------------------------
void CRAxisGizmoActor::SetAxisQueryEnabled( bool bEnabled )
{
    for ( u32 axisIndex = 0; axisIndex < (u32)ECRAxis::Max; ++axisIndex )
    {
        if ( CRCollisionComponent* collision = _GetAxisCollision( (ECRAxis)axisIndex ) )
        {
            collision->SetQueryEnabled( bEnabled );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Compute current gizmo world scale used by shader.
//---------------------------------------------------------------------------------------------------------------------
f32 CRAxisGizmoActor::_ComputeGizmoWorldScale() const
{
    if ( !GWorld ) return 1.0f;

    CRCamera* camera = GWorld->GetCamera();
    if ( !camera ) return 1.0f;

    f32 viewportHeight = 1080.0f;
    if ( ICRRHIRenderer* renderer = GRHI.GetRenderer() )
    {
        viewportHeight = CRMath::Max( 1.0f, (f32)renderer->GetViewportHeight() );
    }

    const f32 projectionCotHalfFovY = camera->GetProjectionMatrix()._22;
    if ( CRMath::Abs( projectionCotHalfFovY ) < CRMath::Epsilon ) return 1.0f;

    const CRVector pivot = GetPivot();
    const CRMatrix view = camera->GetViewMatrix();
    const CRVector4D pivotView = CRVector4D::Transform( CRVector4D( pivot.x, pivot.y, pivot.z, 1.0f ), view );

    const f32 safeDistance = CRMath::Max( CRMath::Abs( pivotView.z ), 0.01f );

    f32 scale = ( 2.0f * safeDistance / CRMath::Abs( projectionCotHalfFovY ) ) * ( GizmoPixelSize / viewportHeight );
    scale = CRMath::Clamp( scale, 0.0001f, 32.0f );

    return scale;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get axis transform.
//---------------------------------------------------------------------------------------------------------------------
CRMatrix CRAxisGizmoActor::_GetAxisTransform( ECRAxis Axis ) const
{
    const CRMatrix rotation = _GetAxisRotation( Axis );

    return rotation * CRMatrix::CreateTranslation( GetPivot() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Get axis color.
//---------------------------------------------------------------------------------------------------------------------
CRVector4D CRAxisGizmoActor::_GetAxisColor( ECRAxis Axis ) const
{
    switch ( Axis )
    {
        case ECRAxis::X: return CRVector4D( 1.0f, 0.25f, 0.25f, 1.0f );
        case ECRAxis::Y: return CRVector4D( 0.30f, 1.0f, 0.30f, 1.0f );
        case ECRAxis::Z: return CRVector4D( 0.30f, 0.55f, 1.0f, 1.0f );
    }

    return CRVector4D( 1.0f, 1.0f, 1.0f, 1.0f );
}
