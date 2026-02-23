#pragma once


#include "CRMath.h"


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
};
