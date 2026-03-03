#pragma once


#include "CRComponent.h"
#include "CRTransformComponent.h"
#include "Source/Core/Math/CRAABB.h"


//---------------------------------------------------------------------------------------------------------------------
/// CRCollisionComponent
//---------------------------------------------------------------------------------------------------------------------
class CRCollisionComponent : public CRComponent< CRCollisionComponent >
{
public:
    static constexpr ECRComponentPriority Priority    = ECRComponentPriority::Collision; /// Component priority.
    static constexpr bool                 IsExclusive = false;                           /// Allows multiple collision components per actor.

private:
    bool     bQueryEnabled     = true;
    CRVector LocalCenter       = CRVector::Zero;
    CRVector LocalHalfExtents  = CRVector( 0.5f, 0.5f, 0.5f );

public:
    /// Constructor.
    CRCollisionComponent() = default;

    /// Called when component is disabled.
    virtual void OnDisabled() override;

    /// Update component.
    virtual void UpdateComponent( float DeltaSeconds ) override;

    /// Set query enabled.
    void SetQueryEnabled( bool bEnabled ) { bQueryEnabled = bEnabled; }

    /// Is query enabled.
    bool IsQueryEnabled() const { return IsEnabled() && bQueryEnabled; }

    /// Set local center.
    void SetLocalCenter( const CRVector& InCenter ) { LocalCenter = InCenter; }

    /// Get local center.
    const CRVector& GetLocalCenter() const { return LocalCenter; }

    /// Set local half extents.
    void SetLocalHalfExtents( const CRVector& InHalfExtents );

    /// Get local half extents.
    const CRVector& GetLocalHalfExtents() const { return LocalHalfExtents; }

    /// Calculate world bounds from local bounds and optional external bounds scale.
    CRAABB CalculateWorldBounds( const CRVector& BoundsScale = CRVector::One ) const
    {
        const CRVector absBoundsScale = CRVector( CRMath::Abs( BoundsScale.x ), CRMath::Abs( BoundsScale.y ), CRMath::Abs( BoundsScale.z ) );
        const CRVector scaledLocalCenter = CRVector( LocalCenter.x * BoundsScale.x, LocalCenter.y * BoundsScale.y, LocalCenter.z * BoundsScale.z );
        const CRVector scaledLocalHalfExtents = CRVector( LocalHalfExtents.x * absBoundsScale.x, LocalHalfExtents.y * absBoundsScale.y, LocalHalfExtents.z * absBoundsScale.z );

        const CRTransformComponent* transform = CRTransformComponent::Get( ObjectId );
        if ( !transform )
        {
            return CRAABB::FromCenterExtents( scaledLocalCenter, scaledLocalHalfExtents );
        }

        const CRVector& actorScale = transform->GetScale();
        const CRVector absActorScale = CRVector( CRMath::Abs( actorScale.x ), CRMath::Abs( actorScale.y ), CRMath::Abs( actorScale.z ) );

        const CRVector worldCenter = transform->GetLocation() + scaledLocalCenter;
        const CRVector worldHalfExtents = CRVector( scaledLocalHalfExtents.x * absActorScale.x, scaledLocalHalfExtents.y * absActorScale.y, scaledLocalHalfExtents.z * absActorScale.z );

        return CRAABB::FromCenterExtents( worldCenter, worldHalfExtents );
    }
};
