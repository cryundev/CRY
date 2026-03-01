#include "CRAxisGizmoActor.h"
#include "Source/Asset/CRPrimitiveAsset.h"
#include "Source/Core/Math/CRAABB.h"
#include "Source/Core/Math/CRRay.h"


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

    const CRVector localCenter      = localBounds.GetCenter();
    const CRVector localHalfExtents = localBounds.GetExtents() * 0.5f;

    struct AxisPreset { CRVector Center; CRVector HalfExtents; };
    
    const AxisPreset presets[ (u32)ECRAxis::Max ] =
    {
        { localCenter, localHalfExtents }, // X
        { CRVector( -localCenter.y, localCenter.x,  localCenter.z ), CRVector( localHalfExtents.y, localHalfExtents.x, localHalfExtents.z ), }, // Y
        { CRVector(  localCenter.z, localCenter.y, -localCenter.x ), CRVector( localHalfExtents.z, localHalfExtents.y, localHalfExtents.x ), }, // Z
    };

    for ( u32 axisIndex = 0; axisIndex < (u32)ECRAxis::Max; ++axisIndex )
    {
        CRCollisionComponent* collision = Add< CRCollisionComponent >();
        if ( !collision )
        {
            bInitialized = false;
            continue;
        }

        collision->SetLocalCenter     ( presets[ axisIndex ].Center      );
        collision->SetLocalHalfExtents( presets[ axisIndex ].HalfExtents );
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

    for ( u32 axisIndex = 0; axisIndex < (u32)ECRAxis::Max; ++axisIndex )
    {
        const CRCollisionComponent* collision = _GetAxisCollision( (ECRAxis)axisIndex );
        if ( !collision || !collision->IsQueryEnabled() ) continue;

        f32 outT = 0.0f;
        if ( collision->GetWorldBounds().Intersects( ray, outT ) )
        {
            return (ECRAxis)axisIndex;
        }
    }

    return ECRAxis::Max;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set axis query enabled.
//---------------------------------------------------------------------------------------------------------------------
void CRAxisGizmoActor::SetAxisQueryEnabled( bool bEnabled )
{
    for ( u32 axisIndex = 0; axisIndex < (u32)ECRAxis::Max; ++axisIndex )
    {
        CRCollisionComponent* collision = _GetAxisCollision( (ECRAxis)axisIndex );
        if ( collision )
        {
            collision->SetQueryEnabled( bEnabled );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/// Get axis transform.
//---------------------------------------------------------------------------------------------------------------------
CRMatrix CRAxisGizmoActor::_GetAxisTransform( ECRAxis Axis ) const
{
    CRMatrix rotation = CRMatrix::Identity;

    switch ( Axis )
    {
        case ECRAxis::X: rotation = CRMatrix::Identity; break;
        case ECRAxis::Y: rotation = CRMatrix::CreateFromAxisAngle( CRVector( 0.f,  0.f, 1.f ), DirectX::XM_PIDIV2 ); break;
        case ECRAxis::Z: rotation = CRMatrix::CreateFromAxisAngle( CRVector( 0.f, -1.f, 0.f ), DirectX::XM_PIDIV2 ); break;
        default: break;
    }

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
