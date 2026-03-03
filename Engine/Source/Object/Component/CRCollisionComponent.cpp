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
