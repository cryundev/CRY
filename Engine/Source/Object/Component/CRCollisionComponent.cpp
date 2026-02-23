#include "CRCollisionComponent.h"


//---------------------------------------------------------------------------------------------------------------------
/// Called when component is disabled.
//---------------------------------------------------------------------------------------------------------------------
void CRCollisionComponent::OnDisabled()
{
}

//---------------------------------------------------------------------------------------------------------------------
/// Update component.
//---------------------------------------------------------------------------------------------------------------------
void CRCollisionComponent::UpdateComponent( float DeltaSeconds )
{
    (void)DeltaSeconds;

    CRTransformComponent* transform = CRTransformComponent::Get( ObjectId );
    if ( !transform ) return;

    const CRVector& scale    = transform->GetScale();
    const CRVector& absScale = CRVector( CRMath::Abs( scale.x ), CRMath::Abs( scale.y ), CRMath::Abs( scale.z ) );

    const CRVector& worldCenter      = transform->GetLocation() + LocalCenter;
    const CRVector& worldHalfExtents = CRVector( LocalHalfExtents.x * absScale.x, LocalHalfExtents.y * absScale.y, LocalHalfExtents.z * absScale.z );

    CachedWorldBounds = CRAABB::FromCenterExtents( worldCenter, worldHalfExtents );
}

//---------------------------------------------------------------------------------------------------------------------
/// Set local half extents.
//---------------------------------------------------------------------------------------------------------------------
void CRCollisionComponent::SetLocalHalfExtents( const CRVector& InHalfExtents )
{
    LocalHalfExtents = CRVector( CRMath::Max( 0.0f, InHalfExtents.x ), CRMath::Max( 0.0f, InHalfExtents.y ), CRMath::Max( 0.0f, InHalfExtents.z ) );
}
