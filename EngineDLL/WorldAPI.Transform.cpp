#include "EngineDLL.h"
#include "WorldAPI.ActorInternal.h"
#include "WorldAPI.TransformInternal.h"


namespace CRTransformApiInternal
{
//---------------------------------------------------------------------------------------------------------------------
/// Convert native vector.
//---------------------------------------------------------------------------------------------------------------------
CRDllVector3 ToDllVector( const CRVector& Value )
{
    return { Value.x, Value.y, Value.z };
}

//---------------------------------------------------------------------------------------------------------------------
/// Convert native vector.
//---------------------------------------------------------------------------------------------------------------------
CRVector ToCRVector( const CRDllVector3& Value )
{
    return CRVector( Value.X, Value.Y, Value.Z );
}

//---------------------------------------------------------------------------------------------------------------------
/// Quaternion to euler degrees.
//---------------------------------------------------------------------------------------------------------------------
CRVector ToEulerDegrees( const CRQuaternion& Rotation )
{
    const CRVector eulerRadians = Rotation.ToEuler();

    return CRVector
    (
        DirectX::XMConvertToDegrees( eulerRadians.x ),
        DirectX::XMConvertToDegrees( eulerRadians.y ),
        DirectX::XMConvertToDegrees( eulerRadians.z )
    );
}

//---------------------------------------------------------------------------------------------------------------------
/// Euler degrees to quaternion.
//---------------------------------------------------------------------------------------------------------------------
CRQuaternion ToQuaternion( const CRDllVector3& EulerDegrees )
{
    const f32 pitch = DirectX::XMConvertToRadians( EulerDegrees.X );
    const f32 yaw   = DirectX::XMConvertToRadians( EulerDegrees.Y );
    const f32 roll  = DirectX::XMConvertToRadians( EulerDegrees.Z );

    return CRQuaternion::CreateFromYawPitchRoll( yaw, pitch, roll );
}

//---------------------------------------------------------------------------------------------------------------------
/// Write transform.
//---------------------------------------------------------------------------------------------------------------------
void WriteTransform( const CRTransformComponent& Transform, CRDllTransform& OutTransform )
{
    OutTransform.Position      = ToDllVector( Transform.GetLocation() );
    OutTransform.RotationEuler = ToDllVector( ToEulerDegrees( Transform.GetRotation() ) );
    OutTransform.Scale         = ToDllVector( Transform.GetScale() );
}

//---------------------------------------------------------------------------------------------------------------------
/// Apply transform.
//---------------------------------------------------------------------------------------------------------------------
void ApplyTransform( CRTransformComponent& Transform, const CRDllTransform& InTransform )
{
    Transform.SetLocation( ToCRVector  ( InTransform.Position      ) );
    Transform.SetRotation( ToQuaternion( InTransform.RotationEuler ) );
    Transform.SetScale   ( ToCRVector  ( InTransform.Scale         ) );
}
}


//---------------------------------------------------------------------------------------------------------------------
/// Get actor transform.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool GetActorTransform( CRIdentity::id_t Id, CRDllTransform* OutTransform )
{
    if ( !OutTransform ) return false;

    if ( CRTransformComponent* transform = CRActorApiInternal::GetTransform( Id ) )
    {
        CRTransformApiInternal::WriteTransform( *transform, *OutTransform );
        
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
/// Set actor transform.
//---------------------------------------------------------------------------------------------------------------------
CR_ENGINE_API bool SetActorTransform( CRIdentity::id_t Id, const CRDllTransform* InTransform )
{
    if ( !InTransform ) return false;

    if ( CRTransformComponent* transform = CRActorApiInternal::GetTransform( Id ) )
    {
        CRTransformApiInternal::ApplyTransform( *transform, *InTransform );
        
        return true;
    }

    return false;
}
