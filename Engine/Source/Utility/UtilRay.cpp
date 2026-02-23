#include "UtilRay.h"
#include "Engine.h"
#include "Source/Core/Math/CRRay.h"
#include "Source/Object/CRActor.h"
#include "Source/Object/Component/CRCollisionComponent.h"
#include "Source/World/CRWorld.h"


namespace
{
//---------------------------------------------------------------------------------------------------------------------
/// Convert pixel coordinates to NDC.
//---------------------------------------------------------------------------------------------------------------------
bool TryConvertToNdc( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH, f32& OutNdcX, f32& OutNdcY )
{
    if ( ViewportW <= 0 || ViewportH <= 0 ) return false;

    OutNdcX = ( (f32)PixelX / (f32)ViewportW ) * 2.0f - 1.0f;
    OutNdcY = 1.0f - ( (f32)PixelY / (f32)ViewportH ) * 2.0f;

    return true;
}
}


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

//---------------------------------------------------------------------------------------------------------------------
/// Pick actor at screen pixel position.
//---------------------------------------------------------------------------------------------------------------------
CRIdentity::id_t UtilRay::PickActorAtScreen( i32 PixelX, i32 PixelY, i32 ViewportW, i32 ViewportH )
{
    if ( !GWorld ) return CRIdentity::IdMask;

    f32 ndcX = 0.0f;
    f32 ndcY = 0.0f;
    
    if ( !TryConvertToNdc( PixelX, PixelY, ViewportW, ViewportH, ndcX, ndcY ) ) return CRIdentity::IdMask;

    CRRay ray;
    
    if ( !UtilRay::TryCreateRayFromNDC( ndcX, ndcY, ray ) ) return CRIdentity::IdMask;

    const CRActor* cameraActor = GWorld->GetCamera();

    bool     bHasHit  = false;
    f32      nearestT = 0.0f;
    CRActor* nearest  = nullptr;

    for ( CRActor* actor : GWorld->GetAllActors() )
    {
        if ( !actor ) continue;
        if ( actor == cameraActor ) continue;

        CRCollisionComponent* collision = actor->GetCollision();
        if ( !collision ) continue;
        if ( !collision->IsEnabled() ) continue;
        if ( !collision->IsQueryEnabled() ) continue;

        f32 hitT = 0.0f;
        if ( !collision->GetWorldBounds().Intersects( ray, hitT ) ) continue;

        if ( !bHasHit || hitT < nearestT )
        {
            bHasHit  = true;
            nearestT = hitT;
            nearest  = actor;
        }
    }

    if ( !bHasHit || !nearest ) return CRIdentity::IdMask;

    return nearest->GetObjectId();
}
