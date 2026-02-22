#pragma once


#include "Source/Core/CRTypes.h"
#include "Source/Core/Math/CRMath.h"


struct CRRay;


namespace UtilRay
{
    /// TryCreateRayFromPoints
    bool TryCreateRayFromPoints( const CRVector& OriginPoint, const CRVector& TargetPoint, CRRay& OutRay );

    /// TryCreateRayFromNDC
    bool TryCreateRayFromNDC( f32 NdcX, f32 NdcY, CRRay& OutRay );
}
