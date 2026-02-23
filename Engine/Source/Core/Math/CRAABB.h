#pragma once


#include "CRMath.h"


struct CRRay;


//---------------------------------------------------------------------------------------------------------------------
/// CRAABB
//---------------------------------------------------------------------------------------------------------------------
struct CRAABB
{
public:
    CRVector Min = CRVector::Zero;
    CRVector Max = CRVector::Zero;

public:
    /// Build AABB from center/extents.
    static CRAABB FromCenterExtents( const CRVector& Center, const CRVector& HalfExtents );

    /// Get center.
    CRVector GetCenter() const;

    /// Get extents.
    CRVector GetExtents() const;

    /// Check whether min/max ordering is valid.
    bool IsValid() const;

    /// Ray intersection test.
    bool Intersects( const CRRay& Ray, f32& OutT ) const;
};
