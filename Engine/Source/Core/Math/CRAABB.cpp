#include "CRAABB.h"
#include "CRRay.h"
#include "Source/Core/CRGeneric.h"


//---------------------------------------------------------------------------------------------------------------------
/// Build AABB from center/extents.
//---------------------------------------------------------------------------------------------------------------------
CRAABB CRAABB::FromCenterExtents( const CRVector& Center, const CRVector& HalfExtents )
{
    CRAABB bounds;
    bounds.Min = Center - HalfExtents;
    bounds.Max = Center + HalfExtents;
    return bounds;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get center.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRAABB::GetCenter() const
{
    return ( Min + Max ) * 0.5f;
}

//---------------------------------------------------------------------------------------------------------------------
/// Get extents.
//---------------------------------------------------------------------------------------------------------------------
CRVector CRAABB::GetExtents() const
{
    return Max - Min;
}

//---------------------------------------------------------------------------------------------------------------------
/// Check whether min/max ordering is valid.
//---------------------------------------------------------------------------------------------------------------------
bool CRAABB::IsValid() const
{
    return Min.x <= Max.x && Min.y <= Max.y && Min.z <= Max.z;
}

//---------------------------------------------------------------------------------------------------------------------
/// Ray intersection test.
//---------------------------------------------------------------------------------------------------------------------
bool CRAABB::Intersects( const CRRay& Ray, f32& OutT ) const
{
    if ( !IsValid() ) return false;

    f32 tMin = CRMath::f32_min;
    f32 tMax = CRMath::f32_max;

    auto testAxis = [ & ] ( f32 Origin, f32 Direction, f32 AxisMin, f32 AxisMax ) -> bool
    {
        if ( CRMath::IsNearlyZero( Direction ) )
        {
            return Origin >= AxisMin && Origin <= AxisMax;
        }

        f32 invDirection = 1.0f / Direction;
        f32 t1 = ( AxisMin - Origin ) * invDirection;
        f32 t2 = ( AxisMax - Origin ) * invDirection;

        if ( t1 > t2 )
        {
            CRSwap( t1, t2 );
        }

        tMin = CRMath::Max( tMin, t1 );
        tMax = CRMath::Min( tMax, t2 );

        return tMin <= tMax;
    };

    if ( !testAxis( Ray.Origin.x, Ray.Direction.x, Min.x, Max.x ) ) return false;
    if ( !testAxis( Ray.Origin.y, Ray.Direction.y, Min.y, Max.y ) ) return false;
    if ( !testAxis( Ray.Origin.z, Ray.Direction.z, Min.z, Max.z ) ) return false;

    if ( tMax < 0.0f ) return false;

    OutT = ( tMin >= 0.0f ) ? tMin : tMax;

    return true;
}
