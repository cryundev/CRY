#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Identify/CRIdentity.h"
#include "Source/Core/Math/CRMath.h"


struct CRRay;


namespace UtilRay
{
    /// TryCreateRayFromPoints
    bool TryCreateRayFromPoints( const CRVector& OriginPoint, const CRVector& TargetPoint, CRRay& OutRay );

    /// TryCreateRayFromNDC
    bool TryCreateRayFromNDC( f32 NdcX, f32 NdcY, CRRay& OutRay );

    /// Pick actor at screen pixel position.
    CRIdentity::id_t PickActorAtScreen( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH );
}
