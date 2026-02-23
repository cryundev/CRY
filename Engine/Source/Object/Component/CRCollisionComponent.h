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
    static constexpr ECRComponentPriority Priority = ECRComponentPriority::Collision; /// Component priority.

private:
    bool     bQueryEnabled     = true;
    CRVector LocalCenter       = CRVector::Zero;
    CRVector LocalHalfExtents  = CRVector( 0.5f, 0.5f, 0.5f );
    CRAABB   CachedWorldBounds = CRAABB::FromCenterExtents( CRVector::Zero, CRVector( 0.5f, 0.5f, 0.5f ) );

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

    /// Get world bounds.
    const CRAABB& GetWorldBounds() const { return CachedWorldBounds; }
};
