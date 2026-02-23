#include "UtilRay.h"
#include "Engine.h"
#include "Source/Core/Math/CRRay.h"
#include "Source/World/CRWorld.h"


//---------------------------------------------------------------------------------------------------------------------
/// TryCreateRayFromPoints
//---------------------------------------------------------------------------------------------------------------------
bool UtilRay::TryCreateRayFromPoints( const CRVector& OriginPoint, const CRVector& TargetPoint, CRRay& OutRay )
{
    CRVector direction = TargetPoint - OriginPoint;
    
    if ( direction.LengthSquared() <= CRMath::Epsilon ) return false;
    direction.Normalize();

    OutRay.Origin    = OriginPoint;
    OutRay.Direction = direction;

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/// TryCreateRayFromNDC
//---------------------------------------------------------------------------------------------------------------------
bool UtilRay::TryCreateRayFromNDC( f32 NdcX, f32 NdcY, CRRay& OutRay )
{
    if ( !GWorld ) return false;

    CRCamera* camera = GWorld->GetCamera();
    if ( !camera ) return false;

    CRVector nearWorldPoint;
    CRVector farWorldPoint;
    
    if ( !camera->TryConvertNdcToWorld( NdcX, NdcY, 0.0f, nearWorldPoint ) ) return false;
    if ( !camera->TryConvertNdcToWorld( NdcX, NdcY, 1.0f, farWorldPoint  ) ) return false;

    return UtilRay::TryCreateRayFromPoints( nearWorldPoint, farWorldPoint, OutRay );
}
