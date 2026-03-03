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
}

//---------------------------------------------------------------------------------------------------------------------
/// Set local half extents.
//---------------------------------------------------------------------------------------------------------------------
void CRCollisionComponent::SetLocalHalfExtents( const CRVector& InHalfExtents )
{
    LocalHalfExtents = CRVector( CRMath::Max( 0.0f, InHalfExtents.x ), CRMath::Max( 0.0f, InHalfExtents.y ), CRMath::Max( 0.0f, InHalfExtents.z ) );
}

//---------------------------------------------------------------------------------------------------------------------
/// Calculate world bounds from local bounds and optional external bounds scale.
/// //---------------------------------------------------------------------------------------------------------------------
CRAABB CRCollisionComponent::CalculateWorldBounds( const CRVector& BoundsScale ) const
{
    const CRVector absBoundsScale = CRVector( CRMath::Abs( BoundsScale.x ), CRMath::Abs( BoundsScale.y ), CRMath::Abs( BoundsScale.z ) );

    const CRVector scaledLocalCenter      = CRVector( LocalCenter.x      * BoundsScale.x,    LocalCenter.y      * BoundsScale.y,    LocalCenter.z      * BoundsScale.z    );
    const CRVector scaledLocalHalfExtents = CRVector( LocalHalfExtents.x * absBoundsScale.x, LocalHalfExtents.y * absBoundsScale.y, LocalHalfExtents.z * absBoundsScale.z );

    const CRTransformComponent* transform = CRTransformComponent::Get( ObjectId );
    if ( !transform )
    {
        return CRAABB::FromCenterExtents( scaledLocalCenter, scaledLocalHalfExtents );
    }

    const CRVector& actorScale    = transform->GetScale();
    const CRVector  absActorScale = CRVector( CRMath::Abs( actorScale.x ), CRMath::Abs( actorScale.y ), CRMath::Abs( actorScale.z ) );

    const CRVector worldCenter      = transform->GetLocation() + scaledLocalCenter;
    const CRVector worldHalfExtents = CRVector( scaledLocalHalfExtents.x * absActorScale.x, scaledLocalHalfExtents.y * absActorScale.y, scaledLocalHalfExtents.z * absActorScale.z );

    return CRAABB::FromCenterExtents( worldCenter, worldHalfExtents );
}
